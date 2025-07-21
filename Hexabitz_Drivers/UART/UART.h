#ifndef UART_H
#define UART_H

/* Includes ****************************************************************/
#include <BOS.h>

#define PORT_MAPPING_NODE DT_NODELABEL(port_mapping)
#define NUM_OF_PORTS DT_PROP_LEN(PORT_MAPPING_NODE, ports)

#if DT_NODE_EXISTS(DT_NODELABEL(usart1))
#define UART_1_DEVICE_NODE DT_NODELABEL(usart1)
// static const struct device *const uart1_dev = DEVICE_DT_GET(UART_1_DEVICE_NODE);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart2))
#define UART_2_DEVICE_NODE DT_NODELABEL(usart2)
// static const struct device *const uart2_dev = DEVICE_DT_GET(UART_2_DEVICE_NODE);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart3))
#define UART_3_DEVICE_NODE DT_NODELABEL(usart3)
// static const struct device *const uart3_dev = DEVICE_DT_GET(UART_3_DEVICE_NODE);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart4))
#define UART_4_DEVICE_NODE DT_NODELABEL(usart4)
// static const struct device *const uart4_dev = DEVICE_DT_GET(UART_4_DEVICE_NODE);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart5))
#define UART_5_DEVICE_NODE DT_NODELABEL(usart5)
// static const struct device *const uart5_dev = DEVICE_DT_GET(UART_5_DEVICE_NODE);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart6))
#define UART_6_DEVICE_NODE DT_NODELABEL(usart6)
// static const struct device *const uart6_dev = DEVICE_DT_GET(UART_6_DEVICE_NODE);
#endif

/*
#define NUM_OF_PORTS                        \
    (DT_NODE_EXISTS(DT_NODELABEL(usart1)) + \
     DT_NODE_EXISTS(DT_NODELABEL(usart2)) + \
     DT_NODE_EXISTS(DT_NODELABEL(usart3)) + \
     DT_NODE_EXISTS(DT_NODELABEL(usart4)) + \
     DT_NODE_EXISTS(DT_NODELABEL(usart5)) + \
     DT_NODE_EXISTS(DT_NODELABEL(usart6)))
*/

static const struct device *const uart_devs[NUM_OF_PORTS] = {
#if DT_NODE_EXISTS(DT_NODELABEL(usart1))
    DEVICE_DT_GET(UART_1_DEVICE_NODE),
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart2))
    DEVICE_DT_GET(UART_2_DEVICE_NODE),
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart3))
    DEVICE_DT_GET(UART_3_DEVICE_NODE),
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart4))
    DEVICE_DT_GET(UART_4_DEVICE_NODE),
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart5))
    DEVICE_DT_GET(UART_5_DEVICE_NODE),
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart6))
    DEVICE_DT_GET(UART_6_DEVICE_NODE),
#endif
};

/* Helper macro for DT_FOREACH_PROP_ELEM */
#define CHECK_PORT(node_id, prop, idx)                                               \
    if (DEVICE_DT_GET(DT_PHANDLE_BY_IDX(PORT_MAPPING_NODE, ports, idx)) == uart_dev) \
    {                                                                                \
        return idx;                                                                  \
    }

struct uart_rx_all_port
{
    struct ring_buf rb;

    uint8_t rb_buffer[MSG_RX_BUF_SIZE];   // buffer used by ring_buf
    uint8_t rx_dma_buf[MAX_MESSAGE_SIZE]; // buffer passed to uart_rx_enable
};

void UARTInit(void);
void SwapUartPins(const struct device *uart_dev, uint8_t direction);

const struct device *GetUart(enum PortNames_e port);
int GetPort(const struct device *uart_dev);

BOS_Status ReadPortsDir(void);
BOS_Status UpdateBaudrate(uint8_t port, uint32_t baudrate);

extern struct uart_rx_all_port uart_ring_buffer[NUM_OF_PORTS];

#endif