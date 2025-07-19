
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
    for (int i = 0; i < UART_PORTS; ++i)
    {
        uart_ports[i].dev = uart_devs[i];
        ring_buf_init(&uart_ports[i].rb, sizeof(uart_ports[i].rb_buffer), uart_ports[i].rb_buffer);
        uart_configure(uart_devs[i], &uart_cfg);
        uart_callback_set(uart_devs[i], uart_callback, &uart_ports[i]);
        uart_rx_enable(uart_devs[i], uart_ports[i].rx_dma_buf, sizeof(uart_ports[i].rx_dma_buf), 0);
    }
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
        uart_rx_enable(uart_rx_procces->dev,
                       uart_rx_procces->rx_dma_buf,
                       sizeof(uart_rx_procces->rx_dma_buf), 0);
        break;

    default:
        break;
    }
}
