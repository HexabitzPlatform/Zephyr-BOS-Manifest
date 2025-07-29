/* Includes ****************************************************************/
#include <zephyr/kernel.h>
#include <BOS.h>

uint8_t PortStatus[NUM_OF_PORTS + 1] = {FREE}; // Initialize all ports to FREE
// uint8_t cMessage[NUM_OF_PORTS][MAX_MESSAGE_SIZE] = {0};

/***************************************************************************/
/* BackEndTask function ****************************************************/
/***************************************************************************/
void BackEndTask(void)
{

    uint8_t byte = 0, bytes_to_read = 0;
    uint8_t uart_handler = 0;
    uint32_t bytes_read = 0;
    uart_rx_all_port_t *uart_rx;

    uint8_t byte_index[NUM_OF_PORTS] = {0};
    uint8_t byte_length[NUM_OF_PORTS] = {0};
    uint8_t PortStatus[NUM_OF_PORTS + 1] = {FREE};

    /* Temporary buffer per port to hold in-progress messages before complete */
    uint8_t TempMsg[NUM_OF_PORTS][MAX_MESSAGE_SIZE] = {{0}};

    while (1)
    {
        /* Wait for UART event */
        if (k_msgq_get(&uart_event_msgq, &uart_data_info, K_FOREVER) == 0)
        {
            uart_handler = uart_data_info.port_index;
            bytes_to_read = uart_data_info.packet_lenght;
            uart_rx = &uart_ring_buffer[uart_handler];

            /* Process bytes one-by-one */
            while (bytes_to_read > 0)
            {
                bytes_read = ring_buf_get(&uart_rx->rb, &byte, 1);
                if (bytes_read == 0)
                {
                    printk("Warning: No data in ring buffer for UART %d\n", uart_handler);
                    break;
                }
                bytes_to_read--;

                /* CLI Handling */
                if (byte == 0x0D && PortStatus[uart_handler] == FREE)
                {
                }
                else if (PortStatus[uart_handler] == CLI)
                {
                    /* Forward CLI data */
                    printk("CLI data on UART %d: %c\n", uart_handler, byte);
                }
                /* BOS Packet Handling */
                else if (byte == 'H' && PortStatus[uart_handler] == FREE)
                {
                    PortStatus[uart_handler] = H_Status;
                    TempMsg[uart_handler][0] = 'H';
                    byte_index[uart_handler] = 1;
                }
                else if (byte == 'Z' && PortStatus[uart_handler] == H_Status)
                {
                    PortStatus[uart_handler] = Z_Status;
                    TempMsg[uart_handler][1] = 'Z';
                    byte_index[uart_handler] = 2;
                }
                else if (byte != 'Z' && PortStatus[uart_handler] == H_Status)
                {
                    PortStatus[uart_handler] = FREE;
                    byte_index[uart_handler] = 0;
                }
                else if (PortStatus[uart_handler] == Z_Status)
                {
                    PortStatus[uart_handler] = MSG;
                    /* Receive length byte */
                    TempMsg[uart_handler][2] = byte;
                    byte_length[uart_handler] = byte + 1; // Length + CRC
                    byte_index[uart_handler] = 3;
                }
                else if (PortStatus[uart_handler] == MSG)
                {
                    TempMsg[uart_handler][byte_index[uart_handler]] = byte;
                    byte_index[uart_handler]++;
                    byte_length[uart_handler]--;

                    if (byte_length[uart_handler] == 0)
                    {
                        /* Full message received, allocate buffer */
                        BOS_Message_t *msg_ptr;
                        if (k_mem_slab_alloc(&message_slab, (void **)&msg_ptr, K_NO_WAIT) == 0)
                        {
                            uint8_t full_len = TempMsg[uart_handler][2] + 1;

                            msg_ptr->port = uart_handler;
                            msg_ptr->length = full_len;
                            memcpy(msg_ptr->data, &TempMsg[uart_handler][3], full_len);

                            if (k_msgq_put(&bos_packet_msgq, &msg_ptr, K_NO_WAIT) != 0)
                            {
                                k_mem_slab_free(&message_slab, (void *)&msg_ptr);
                                printk("⚠️ bos_msgq full, message dropped\n");
                            }
                            else
                            {
                                for (uint32_t i = 0; i < full_len; i++)
                                    uart_poll_out(uart_rx->port, msg_ptr->data[i]);
                            }
                        }
                        else
                        {
                            printk("❌ No memory in slab, message dropped\n");
                        }

                        // Reset parser state
                        PortStatus[uart_handler] = FREE;
                        byte_index[uart_handler] = 0;
                        byte_length[uart_handler] = 0;
                    }
                }
            }
        }
    }
}

/***************************************************************************/
/* PxMessagingTask function ************************************************/
/***************************************************************************/
void PxMessagingTask(void)
{
    BOS_Message_t *msg_ptr;
    while (1)
    {
        if (k_msgq_get(&bos_packet_msgq, &msg_ptr, K_FOREVER) == 0)
        {

            // Free the slab after you're done
            k_mem_slab_free(&message_slab, (void *)&msg_ptr);
        }
    }
}
