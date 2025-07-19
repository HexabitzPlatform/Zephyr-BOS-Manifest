
#include <BOS.h>

/* uart configuration structure */
const struct uart_config uart_cfg = {.baudrate = 921600,
                                     .parity = UART_CFG_PARITY_NONE,
                                     .stop_bits = UART_CFG_STOP_BITS_1,
                                     .data_bits = UART_CFG_DATA_BITS_8,
                                     .flow_ctrl = UART_CFG_FLOW_CTRL_NONE};

void uart_callback(const struct device *dev, struct uart_event *evt, void *user_data);

void UARTInit(void)
{

#if DT_NODE_EXISTS(DT_NODELABEL(usart1))
    ring_buf_init(&uart_ports[UART_1].rb, sizeof(uart_ports[UART_1].rb_buffer), uart_ports[UART_1].rb_buffer);
    uart_configure(uart_devs[UART_1], &uart_cfg);
    uart_callback_set(uart_devs[UART_1], uart_callback, &uart_ports[UART_1]);
    uart_rx_enable(uart_devs[UART_1], uart_ports[UART_1].rx_dma_buf, sizeof(uart_ports[UART_1].rx_dma_buf), 0);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart2))
    ring_buf_init(&uart_ports[UART_2].rb, sizeof(uart_ports[UART_2].rb_buffer), uart_ports[UART_2].rb_buffer);
    uart_configure(uart_devs[UART_2], &uart_cfg);
    uart_callback_set(uart_devs[UART_2], uart_callback, &uart_ports[UART_2]);
    uart_rx_enable(uart_devs[UART_2], uart_ports[UART_2].rx_dma_buf, sizeof(uart_ports[UART_2].rx_dma_buf), 0);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart3))
    ring_buf_init(&uart_ports[UART_3].rb, sizeof(uart_ports[UART_3].rb_buffer), uart_ports[UART_3].rb_buffer);
    uart_configure(uart_devs[UART_3], &uart_cfg);
    uart_callback_set(uart_devs[UART_3], uart_callback, &uart_ports[UART_3]);
    uart_rx_enable(uart_devs[UART_3], uart_ports[UART_3].rx_dma_buf, sizeof(uart_ports[UART_3].rx_dma_buf), 0);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart4))
    ring_buf_init(&uart_ports[UART_4].rb, sizeof(uart_ports[UART_4].rb_buffer), uart_ports[UART_4].rb_buffer);
    uart_configure(uart_devs[UART_4], &uart_cfg);
    uart_callback_set(uart_devs[UART_4], uart_callback, &uart_ports[UART_4]);
    uart_rx_enable(uart_devs[UART_4], uart_ports[UART_4].rx_dma_buf, sizeof(uart_ports[UART_4].rx_dma_buf), 0);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart5))
    ring_buf_init(&uart_ports[UART_5].rb, sizeof(uart_ports[UART_5].rb_buffer), uart_ports[UART_5].rb_buffer);
    uart_configure(uart_devs[UART_5], &uart_cfg);
    uart_callback_set(uart_devs[UART_5], uart_callback, &uart_ports[UART_5]);
    uart_rx_enable(uart_devs[UART_5], uart_ports[UART_5].rx_dma_buf, sizeof(uart_ports[UART_5].rx_dma_buf), 0);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart6))
    ring_buf_init(&uart_ports[UART_6].rb, sizeof(uart_ports[UART_6].rb_buffer), uart_ports[UART_6].rb_buffer);
    uart_configure(uart_devs[UART_6], &uart_cfg);
    uart_callback_set(uart_devs[UART_6], uart_callback, &uart_ports[UART_6]);
    uart_rx_enable(uart_devs[UART_6], uart_ports[UART_6].rx_dma_buf, sizeof(uart_ports[UART_6].rx_dma_buf), 0);
#endif
}

void uart_callback(const struct device *dev, struct uart_event *evt, void *user_data)
{
    struct uart_rx_all_port *uart_rx_procces = (struct uart_rx_all_port *)user_data;

    switch (evt->type)
    {
    case UART_RX_RDY:
        ring_buf_put(&uart_rx_procces->rb,
                     evt->data.rx.buf + evt->data.rx.offset,
                     evt->data.rx.len);
        break;

    case UART_RX_DISABLED:
        uart_rx_enable(dev,
                       uart_rx_procces->rx_dma_buf,
                       sizeof(uart_rx_procces->rx_dma_buf), 0);
        break;

    default:
        break;
    }
}

/***************************************************************************/
/* Configure UARTs *********************************************************/
/***************************************************************************/
const struct device *GetUart(enum PortNames_e port)
{
    switch (port)
    {
#ifdef _P1
    case P1:
        return UART_P1;
#endif
#ifdef _P2
    case P2:
        return UART_P2;
#endif
#ifdef _P3
    case P3:
        return UART_P3;
#endif
#ifdef _P4
    case P4:
        return UART_P4;
#endif
#ifdef _P5
    case P5:
        return UART_P5;
#endif
#ifdef _P6
    case P6:
        return UART_P6;
#endif
    }

    return 0;
}
/***************************************************************************/
uint8_t GetPort(const struct device *uart_dev)
{
    /* Map UART device to port */
    if (uart_dev == &uart_devs[UART_4])
    { /* usart4 */
        return P1;
    }
    else if (uart_dev == &uart_devs[UART_2])
    { /* usart2 */
        return P2;
    }
    else if (uart_dev == &uart_devs[UART_3])
    { /* usart3 */
        return P3;
    }
    else if (uart_dev == &uart_devs[UART_1])
    { /* usart1 */
        return P4;
    }
    else if (uart_dev == &uart_devs[UART_5])
    { /* usart5 */
        return P5;
    }
    else if (uart_dev == &uart_devs[UART_6])
    { /* usart6 */
        return P6;
    }

    return 0;
}
/***************************************************************************/
BOS_Status UpdateBaudrate(uint8_t port, uint32_t baudrate)
{
}
/***************************************************************************/
void SwapUartPins(UART_HandleTypeDef *huart, uint8_t direction)
{
}
/***************************************************************************/
BOS_Status ReadPortsDir(void)
{
}
/***************************************************************************/
BOS_Status UpdateMyPortsDir(void)
{
}

/***************************************************************************/
/***************** (C) COPYRIGHT HEXABITZ ***** END OF FILE ****************/