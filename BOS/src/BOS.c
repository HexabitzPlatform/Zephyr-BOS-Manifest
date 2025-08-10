#include "BOS.h"

uart_rx_all_port_t uart_ring_buffer[NUM_OF_PORTS];
uart_event_port_len_t uart_data_info;
// BOS_Message_t bos_msg;

BOS_MessageCode_t code;
BOSOptionByte_t OptionByte = {0};

uint16_t Array[MAX_NUM_OF_MODULES][MAX_NUM_OF_PORTS + 1] = {{0}}; /* Array topology */
uint8_t NumberofModulesinArray = 1;
uint16_t ArrayPortsDir[MAX_NUM_OF_MODULES];
uint8_t myID = 2;

uint8_t QisEmpty(uint8_t *Q);
uint8_t minArr(uint8_t *arr, uint8_t *Q);

// Define the LED using device tree node
const struct gpio_dt_spec indicator_led = GPIO_DT_SPEC_GET(DT_ALIAS(module_led), gpios);
// const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(module_led), gpios);
/***************************************************************************/
int led_init(void)
{
    if (!gpio_is_ready_dt(&indicator_led))
    {
        printk("Error: LED GPIO device %s not ready\n", indicator_led.port->name);
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&indicator_led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        printk("Error: Failed to configure LED GPIO (err %d)\n", ret);
        return ret;
    }

    return 0;
}
/***************************************************************************/
int led_on(void)
{
    return gpio_pin_set_dt(&indicator_led, 1);
}
/***************************************************************************/
int led_off(void)
{
    return gpio_pin_set_dt(&indicator_led, 0);
}
/***************************************************************************/
int led_toggle(void)
{
    return gpio_pin_toggle_dt(&indicator_led);
}
/***************************************************************************/
int led_ping(uint32_t delay)
{
    led_toggle();
    k_msleep(delay);

    return 0;
}
/***************************************************************************/
/**
 * @brief Calculates an 8-bit CRC using polynomial 0x4C11DB7 over 4-byte reversed data chunks.
 *
 * @param data Pointer to the input data array
 * @param length Length of the input data in bytes
 * @return 8-bit CRC result
 */
uint8_t calculate_crc8(const uint8_t *data, size_t length)
{
    uint32_t crc = 0xFFFFFFFF;

    // Calculate ceil(length / 4)
    size_t l = (length + 3) / 4;
    if (l <= 2)
        l = 2;

    size_t padded_len = l * 4;

    // Allocate buffer for reversed data (4-byte reversed blocks)
    uint8_t reversed[padded_len];
    memset(reversed, 0, padded_len); // Zero padding

    // Reorder bytes in each 4-byte block: reverse order
    size_t shift = 0;
    for (size_t i = 0; i < l; ++i)
    {
        for (int jj = 3; jj >= 0; --jj)
        {
            size_t src_index = shift + jj;
            size_t dst_index = shift + (3 - jj);

            if (src_index < length)
                reversed[dst_index] = data[src_index];
            else
                reversed[dst_index] = 0;
        }
        shift += 4;
    }

    // Compute CRC using polynomial 0x4C11DB7 (standard Ethernet CRC32)
    for (size_t i = 0; i < padded_len; ++i)
    {
        crc ^= ((uint32_t)reversed[i]) << 24;
        for (int j = 0; j < 8; ++j)
        {
            uint32_t msb = crc >> 31;
            crc <<= 1;
            if (msb)
                crc ^= 0x4C11DB7;
        }
    }

    return (uint8_t)(crc & 0xFF);
}

/***************************************************************************/
/* Find the shortest route from sourceID to desID using Dijkstra's algorithm */
uint8_t FindRoute(uint8_t sourceID, uint8_t desID)
{

    uint8_t RouteDist[MAX_NUM_OF_MODULES] = {0};
    uint8_t RoutePrev[MAX_NUM_OF_MODULES] = {0};
    uint8_t Route[MAX_NUM_OF_MODULES] = {0};

#ifdef __N
    uint8_t Q[__N] = {0}; /* Unvisited nodes (0 = unvisited, 1 = visited) */
#else
    uint8_t Q[50] = {0}; /* Default size if N not defined */
#endif

    uint8_t u, v, alt, j = 0;

    /* Input validation */
    if (sourceID < 1 || sourceID > NumberofModulesinArray || desID < 1 || desID > NumberofModulesinArray)
    {
        return 0; /* Invalid node IDs */
    }

    /* Initialize arrays */
    memset(Route, 0, sizeof(Route));
    for (int i = 0; i < NumberofModulesinArray; i++)
    {
        RouteDist[i] = 0xFF; /* Infinity for all nodes */
        RoutePrev[i] = 0;    /* Undefined predecessor */
        Q[i] = 0;            /* All nodes unvisited */
    }
    RouteDist[sourceID - 1] = 0; /* Distance from source to source */

    /* Check direct neighbors */
    for (int col = 1; col <= 6; col++)
    {
        if (Array[sourceID - 1][col] && ((Array[sourceID - 1][col] >> 3) == desID))
        {
            RouteDist[desID - 1] = 1;
            RoutePrev[desID - 1] = sourceID;
            Route[0] = desID;
            return col; /* Direct connection found */
        }
    }

    /* Dijkstra's algorithm */
    while (!QisEmpty(Q))
    {
        u = minArr(RouteDist, Q);
        if (u == 0xFF)
            break;    /* No more reachable unvisited nodes */
        u += 1;       /* Convert index to node ID */
        Q[u - 1] = 1; /* Mark as visited */

        if (u == desID)
            break; /* Destination reached */

        /* Check neighbors */
        for (uint8_t n = 1; n <= 6; n++)
        {
            if (Array[u - 1][n])
            {
                v = (Array[u - 1][n] >> 3); /* Get neighbor ID */
                if (!Q[v - 1])
                {                               /* Neighbor is unvisited */
                    alt = RouteDist[u - 1] + 1; /* Edge weight = 1 */
                    if (alt < RouteDist[v - 1])
                    {
                        RouteDist[v - 1] = alt;
                        RoutePrev[v - 1] = u;
                    }
                }
            }
        }
    }

    /* Check if destination is unreachable */
    if (RouteDist[desID - 1] == 0xFF)
    {
        return 0; /* No path exists */
    }

    /* Reconstruct path */
    u = desID;
    while (u != sourceID && RoutePrev[u - 1] != 0)
    {
        Route[j++] = u;
        u = RoutePrev[u - 1];
    }
    if (u != sourceID)
    {
        return 0; /* Path reconstruction failed */
    }

    /* Find port from source to first node in path */
    for (int col = 1; col <= 6; col++)
    {
        if (Array[sourceID - 1][col] && ((Array[sourceID - 1][col] >> 3) == Route[j - 1]))
        {
            return col;
        }
    }

    return 0; /* No valid port found */
}

/***************************************************************************/
/* Find index of unvisited node with smallest distance */
uint8_t minArr(uint8_t *arr, uint8_t *Q)
{
    uint8_t smallest = 0xFF;
    uint8_t index = 0xFF; /* Invalid index if no unvisited nodes */
    uint8_t found = 0;

    for (int i = 0; i < NumberofModulesinArray; i++)
    {
        if (!Q[i] && arr[i] < smallest)
        {
            smallest = arr[i];
            index = i;
            found = 1;
        }
    }

    return index;
}

/***************************************************************************/
/* Check if Q is empty (all nodes visited) */
uint8_t QisEmpty(uint8_t *Q)
{
    for (int i = 0; i < NumberofModulesinArray; i++)
    {
        if (!Q[i])
            return 0; /* Q is not empty */
    }
    return 1; /* Q is empty */
}