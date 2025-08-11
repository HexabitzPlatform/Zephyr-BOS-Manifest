
/* Includes ****************************************************************/
#include <BOS.h>
#include <stm32_ll_usart.h>

/* uart configuration structure */
const struct uart_config uart_cfg = {.baudrate = 921600,
                                     .parity = UART_CFG_PARITY_NONE,
                                     .stop_bits = UART_CFG_STOP_BITS_1,
                                     .data_bits = UART_CFG_DATA_BITS_8,
                                     .flow_ctrl = UART_CFG_FLOW_CTRL_NONE};

void uart_callback(const struct device *dev, struct uart_event *evt, void *user_data);

/***************************************************************************/
/* Configure UARTs *********************************************************/
/***************************************************************************/
void UARTInit(void)
{

#if DT_NODE_EXISTS(DT_NODELABEL(usart1))
    ring_buf_init(&uart_ring_buffer[UART_1].rb, sizeof(uart_ring_buffer[UART_1].rb_buffer), uart_ring_buffer[UART_1].rb_buffer);
    uart_configure(uart_devs[UART_1], &uart_cfg);
    uart_callback_set(uart_devs[UART_1], uart_callback, &uart_ring_buffer[UART_1]);
    uart_rx_enable(uart_devs[UART_1], uart_ring_buffer[UART_1].rx_dma_buf, sizeof(uart_ring_buffer[UART_1].rx_dma_buf), 0);
    uart_ring_buffer[UART_1].port_index = UART_1;
    uart_ring_buffer[UART_1].port = uart_devs[UART_1];
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart2))
    ring_buf_init(&uart_ring_buffer[UART_2].rb, sizeof(uart_ring_buffer[UART_2].rb_buffer), uart_ring_buffer[UART_2].rb_buffer);
    uart_configure(uart_devs[UART_2], &uart_cfg);
    uart_callback_set(uart_devs[UART_2], uart_callback, &uart_ring_buffer[UART_2]);
    uart_rx_enable(uart_devs[UART_2], uart_ring_buffer[UART_2].rx_dma_buf, sizeof(uart_ring_buffer[UART_2].rx_dma_buf), 0);
    uart_ring_buffer[UART_2].port_index = UART_2;
    uart_ring_buffer[UART_2].port = uart_devs[UART_2];
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart3))
    ring_buf_init(&uart_ring_buffer[UART_3].rb, sizeof(uart_ring_buffer[UART_3].rb_buffer), uart_ring_buffer[UART_3].rb_buffer);
    uart_configure(uart_devs[UART_3], &uart_cfg);
    uart_callback_set(uart_devs[UART_3], uart_callback, &uart_ring_buffer[UART_3]);
    uart_rx_enable(uart_devs[UART_3], uart_ring_buffer[UART_3].rx_dma_buf, sizeof(uart_ring_buffer[UART_3].rx_dma_buf), 0);
    uart_ring_buffer[UART_3].port_index = UART_3;
    uart_ring_buffer[UART_3].port = uart_devs[UART_3];
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart4))
    ring_buf_init(&uart_ring_buffer[UART_4].rb, sizeof(uart_ring_buffer[UART_4].rb_buffer), uart_ring_buffer[UART_4].rb_buffer);
    uart_configure(uart_devs[UART_4], &uart_cfg);
    uart_callback_set(uart_devs[UART_4], uart_callback, &uart_ring_buffer[UART_4]);
    uart_rx_enable(uart_devs[UART_4], uart_ring_buffer[UART_4].rx_dma_buf, sizeof(uart_ring_buffer[UART_4].rx_dma_buf), 0);
    uart_ring_buffer[UART_4].port_index = UART_4;
    uart_ring_buffer[UART_4].port = uart_devs[UART_4];
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart5))
    ring_buf_init(&uart_ring_buffer[UART_5].rb, sizeof(uart_ring_buffer[UART_5].rb_buffer), uart_ring_buffer[UART_5].rb_buffer);
    uart_configure(uart_devs[UART_5], &uart_cfg);
    uart_callback_set(uart_devs[UART_5], uart_callback, &uart_ring_buffer[UART_5]);
    uart_rx_enable(uart_devs[UART_5], uart_ring_buffer[UART_5].rx_dma_buf, sizeof(uart_ring_buffer[UART_5].rx_dma_buf), 0);
    uart_ring_buffer[UART_5].port_index = UART_5;
    uart_ring_buffer[UART_5].port = uart_devs[UART_5];
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart6))
    ring_buf_init(&uart_ring_buffer[UART_6].rb, sizeof(uart_ring_buffer[UART_6].rb_buffer), uart_ring_buffer[UART_6].rb_buffer);
    uart_configure(uart_devs[UART_6], &uart_cfg);
    uart_callback_set(uart_devs[UART_6], uart_callback, &uart_ring_buffer[UART_6]);
    uart_rx_enable(uart_devs[UART_6], uart_ring_buffer[UART_6].rx_dma_buf, sizeof(uart_ring_buffer[UART_6].rx_dma_buf), 0);
    uart_ring_buffer[UART_6].port_index = UART_6;
    uart_ring_buffer[UART_6].port = uart_devs[UART_6];
#endif
}

/***************************************************************************/
BOS_Status UART_Tx(uint8_t port_index, const uint8_t *data, size_t length)
{
    // if (port_index >= NUM_OF_PORTS)
    //     return BOS_ERR;

    const struct device *uart_dev = uart_devs[port_index];

    if (!device_is_ready(uart_dev))
        return 0;

    int ret = uart_tx(uart_dev, data, length, SYS_FOREVER_MS);
    if (ret != 0)
        return 0;

    return BOS_OK;
}

/***************************************************************************/
void uart_callback(const struct device *dev, struct uart_event *evt, void *user_data)
{
    uart_rx_all_port_t *uart_rx_procces = (uart_rx_all_port_t *)user_data;

    switch (evt->type)
    {
    case UART_RX_RDY:
        ring_buf_put(&uart_rx_procces->rb,
                     evt->data.rx.buf + evt->data.rx.offset,
                     evt->data.rx.len);

        // uart_data_info.port_index = GetPort(uart_rx_procces->port);
        uart_data_info.port_index = uart_rx_procces->port_index;
        uart_data_info.packet_length = evt->data.rx.len;

        // for (uint32_t i = 0; i < uart_data_info.packet_length; i++)
        //     uart_poll_out(GetUart(P2), evt->data.rx.buf[i]);

        // UART_Tx(1, evt->data.rx.buf, uart_data_info.packet_length);

        // Send the message to the backend task
        k_msgq_put(&uart_event_msgq, &uart_data_info, K_NO_WAIT);

        break;

    case UART_RX_DISABLED:
        uart_rx_enable(uart_rx_procces->port,
                       uart_rx_procces->rx_dma_buf,
                       sizeof(uart_rx_procces->rx_dma_buf), 0);
        break;

    case UART_TX_DONE:
        // TX completed successfully
        // evt->data.tx.len is how many bytes were sent
        // evt->data.tx.buf is the buffer pointer
        break;

    case UART_TX_ABORTED:
        // TX was aborted before completion
        break;

    default:
        break;
    }
}
/***************************************************************************/
const struct device *GetUart(enum PortNames_e port)
{
    if (port >= NUM_OF_PORTS)
    {
        return NULL; /* Invalid port index */
    }

    /* Use constant indices for DT_PHANDLE_BY_IDX */
    switch (port)
    {
    case P1:
        return DEVICE_DT_GET(DT_PHANDLE_BY_IDX(PORT_MAPPING_NODE, ports, 0));
    case P2:
        return DEVICE_DT_GET(DT_PHANDLE_BY_IDX(PORT_MAPPING_NODE, ports, 1));
    case P3:
        return DEVICE_DT_GET(DT_PHANDLE_BY_IDX(PORT_MAPPING_NODE, ports, 2));
    case P4:
        return DEVICE_DT_GET(DT_PHANDLE_BY_IDX(PORT_MAPPING_NODE, ports, 3));
    case P5:
        return DEVICE_DT_GET(DT_PHANDLE_BY_IDX(PORT_MAPPING_NODE, ports, 4));
    case P6:
        return DEVICE_DT_GET(DT_PHANDLE_BY_IDX(PORT_MAPPING_NODE, ports, 5));
    default:
        return NULL; /* Invalid port */
    }
}
/***************************************************************************/
int GetPort(const struct device *uart_dev)
{
    if (!uart_dev)
    {
        return -EINVAL; /* Invalid device */
    }

    /* Iterate through the ports property at compile time */
    DT_FOREACH_PROP_ELEM(PORT_MAPPING_NODE, ports, CHECK_PORT);

    return -ENODEV; /* Device not found */
}
/***************************************************************************/
void SwapUartPins(const struct device *uart_dev, uint8_t direction)
{
    if (!uart_dev || !device_is_ready(uart_dev))
    {
        return;
    }

    int port = GetPort(uart_dev);
    if (port < 0)
    {
        return; /* Invalid port */
    }

    if (direction == REVERSED)
    {
        /* Set bit to one */
        ArrayPortsDir[myID - 1] |= (0x8000 >> (port - 1));

        /* Get STM32 USART instance from Zephyr device */
        USART_TypeDef *huart = (USART_TypeDef *)DEVICE_MMIO_GET(uart_dev);

        /* Enable pin swap using STM32 LL driver */
        LL_USART_Disable(huart);
        LL_USART_SetTXRXSwap(huart, LL_USART_TXRX_SWAPPED);
        LL_USART_Enable(huart);
    }
    else if (direction == NORMAL)
    {
        /* Set bit to zero */
        ArrayPortsDir[myID - 1] &= (~(0x8000 >> (port - 1)));

        /* Get STM32 USART instance from Zephyr device */
        USART_TypeDef *huart = (USART_TypeDef *)DEVICE_MMIO_GET(uart_dev);

        /* Enable pin swap using STM32 LL driver */
        LL_USART_Disable(huart);
        LL_USART_SetTXRXSwap(huart, LL_USART_TXRX_STANDARD);
        LL_USART_Enable(huart);
    }

    /* Reconfigure UART to apply changes */
    uart_configure(uart_dev, &uart_cfg);

    /* Set up DMA reception */
    uart_rx_disable(uart_dev);
    uart_rx_enable(uart_dev, uart_ring_buffer[port].rx_dma_buf, sizeof(uart_ring_buffer[port].rx_dma_buf), 0);
}
/***************************************************************************/
BOS_Status ReadPortsDir(void)
{
    return 0;
}
/***************************************************************************/
BOS_Status UpdateBaudrate(uint8_t port, uint32_t baudrate)
{
    BOS_Status result = BOS_OK;
    const struct device *uart_dev = GetUart(port);
    struct uart_config newConfig;

    uart_config_get(uart_dev, &newConfig);

    newConfig.baudrate = baudrate;

    uart_configure(uart_dev, &newConfig);

    return result;
}

/***************************************************************************/
/***************** (C) COPYRIGHT HEXABITZ ***** END OF FILE ****************/