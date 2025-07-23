/* Includes ****************************************************************/
#include <zephyr/kernel.h>
#include <BOS.h>

/***************************************************************************/
/* BackEndTask function ****************************************************/
/***************************************************************************/
void BackEndTask(void)
{
    uint8_t temp[64] = {0};

    while (1)
    {
        if (k_msgq_get(&uart_event_queue, &msg, K_FOREVER) == 0)
        {
            uint8_t port = msg.port_index;
            uint8_t lenght = msg.packet_lenght;
            struct uart_rx_all_port *uart_rx = &uart_ring_buffer[port];

            uint32_t bytes_read = ring_buf_get(&uart_rx->rb, temp, sizeof(temp));
            for (uint32_t i = 0; i < bytes_read; i++)
            {
                uart_poll_out(uart_rx->port, temp[i]); // Echo all bytes
            }

            // Now you know which UART triggered it
            // struct uart_rx_all_port *uart_rx = &uart_ring_buffer[port];

            // k_poll_signal_raise(&PxMessagingTask_signal, 0);
        }
    }
}

/***************************************************************************/
/* PxMessagingTask function ************************************************/
/***************************************************************************/
void PxMessagingTask(void)
{
    struct k_poll_event PxMessagingTask_event = {
        .type = K_POLL_TYPE_SIGNAL,
        .mode = K_POLL_MODE_NOTIFY_ONLY,
        .signal = &PxMessagingTask_signal,
    };

    /* Initialize the poll signal */
    k_poll_signal_init(&PxMessagingTask_signal);
    while (1)
    {
        k_poll(&PxMessagingTask_event, 1, K_FOREVER);
    }
}
