
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
    ring_buf_init(&uart_ports[UART_PORT_1].rb, sizeof(uart_ports[UART_PORT_1].rb_buffer), uart_ports[UART_PORT_1].rb_buffer);
    uart_configure(uart1_dev, &uart_cfg);
    uart_callback_set(uart1_dev, uart_callback, &uart_ports[UART_PORT_1]);
    uart_rx_enable(uart1_dev, uart_ports[UART_PORT_1].rx_dma_buf, sizeof(uart_ports[UART_PORT_1].rx_dma_buf), 0);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart2))
    ring_buf_init(&uart_ports[UART_PORT_2].rb, sizeof(uart_ports[UART_PORT_2].rb_buffer), uart_ports[UART_PORT_2].rb_buffer);
    uart_configure(uart2_dev, &uart_cfg);
    uart_callback_set(uart2_dev, uart_callback, &uart_ports[UART_PORT_2]);
    uart_rx_enable(uart2_dev, uart_ports[UART_PORT_2].rx_dma_buf, sizeof(uart_ports[UART_PORT_2].rx_dma_buf), 0);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart3))
    ring_buf_init(&uart_ports[UART_PORT_3].rb, sizeof(uart_ports[UART_PORT_3].rb_buffer), uart_ports[UART_PORT_3].rb_buffer);
    uart_configure(uart3_dev, &uart_cfg);
    uart_callback_set(uart3_dev, uart_callback, &uart_ports[UART_PORT_3]);
    uart_rx_enable(uart3_dev, uart_ports[UART_PORT_3].rx_dma_buf, sizeof(uart_ports[UART_PORT_3].rx_dma_buf), 0);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart4))
    ring_buf_init(&uart_ports[UART_PORT_4].rb, sizeof(uart_ports[UART_PORT_4].rb_buffer), uart_ports[UART_PORT_4].rb_buffer);
    uart_configure(uart4_dev, &uart_cfg);
    uart_callback_set(uart4_dev, uart_callback, &uart_ports[UART_PORT_4]);
    uart_rx_enable(uart4_dev, uart_ports[UART_PORT_4].rx_dma_buf, sizeof(uart_ports[UART_PORT_4].rx_dma_buf), 0);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart5))
    ring_buf_init(&uart_ports[UART_PORT_5].rb, sizeof(uart_ports[UART_PORT_5].rb_buffer), uart_ports[UART_PORT_5].rb_buffer);
    uart_configure(uart5_dev, &uart_cfg);
    uart_callback_set(uart5_dev, uart_callback, &uart_ports[UART_PORT_5]);
    uart_rx_enable(uart5_dev, uart_ports[UART_PORT_5].rx_dma_buf, sizeof(uart_ports[UART_PORT_5].rx_dma_buf), 0);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart6))
    ring_buf_init(&uart_ports[UART_PORT_6].rb, sizeof(uart_ports[UART_PORT_6].rb_buffer), uart_ports[UART_PORT_6].rb_buffer);
    uart_configure(uart6_dev, &uart_cfg);
    uart_callback_set(uart6_dev, uart_callback, &uart_ports[UART_PORT_6]);
    uart_rx_enable(uart6_dev, uart_ports[UART_PORT_6].rx_dma_buf, sizeof(uart_ports[UART_PORT_6].rx_dma_buf), 0);
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
