/* Includes ****************************************************************/
#include <zephyr/kernel.h>
#include <BOS.h>

uint8_t PortStatus[NUM_OF_PORTS + 1] = {FREE}; // Initialize all ports to FREE
uint8_t MessageBuffer[NUM_OF_PORTS][MSG_COUNT][MAX_MESSAGE_SIZE] = {0};
uint8_t MessageIndexStart[NUM_OF_PORTS] = {0};
uint8_t MessageIndexEnd[NUM_OF_PORTS] = {0};
uint8_t ProcessMessageBuffer[MSG_COUNT] = {0};
uint8_t ProcessMessageIndexStart = 0;
uint8_t ProcessMessageIndexEnd = 0;
uint8_t temp_index[NUM_OF_PORTS] = {0};
uint8_t temp_length[NUM_OF_PORTS] = {0};
uint8_t cMessage[NUM_OF_PORTS][MAX_MESSAGE_SIZE] = {0};
uint8_t MessageLength[NUM_OF_PORTS] = {0};
uint8_t AcceptedMessages = 0, RejectedMessages = 0, MessageCounter = 0;
uint8_t bcastID = 0, bcastLastID = 0;

/***************************************************************************/
/* BackEndTask function ****************************************************/
/***************************************************************************/
void BackEndTask(void)
{
    uint8_t temp[64] = {0};
    uint8_t uart_handler = 0, length = 0, dst = 0;

    uint8_t port_number = 0, bytes_to_read = 0;

    uint32_t bytes_read = 0;
    uint8_t port_index = 0;

    while (1)
    {
        if (k_msgq_get(&uart_event_queue, &msg, K_FOREVER) == 0)
        {
            uart_handler = msg.port_index;
            bytes_to_read = msg.packet_lenght;
            struct uart_rx_all_port *uart_rx = &uart_ring_buffer[uart_handler];

            // Process bytes one-by-one
            while (bytes_to_read > 0)
            {
                bytes_read = ring_buf_get(&uart_rx->rb, temp, 1);
                if (bytes_read == 0)
                {
                    printk("Warning: No data in ring buffer for UART %d\n", uart_handler);
                    break;
                }
                bytes_to_read--;

                // CLI Handling
                if (temp[0] == 0x0D && PortStatus[uart_handler] == FREE)
                {
                }
                else if (PortStatus[uart_handler] == CLI)
                {
                    // Forward CLI data (implement as needed)
                    printk("CLI data on UART %d: %c\n", uart_handler, temp[0]);
                }
                // BOS Packet Handling
                else if (temp[0] == 'H' && PortStatus[uart_handler] == FREE)
                {
                    PortStatus[uart_handler] = H_Status;
                    MessageBuffer[uart_handler][MessageIndexEnd[uart_handler]][0] = 'H';
                    temp_index[uart_handler] = 1;
                }
                else if (temp[0] == 'Z' && PortStatus[uart_handler] == H_Status)
                {
                    PortStatus[uart_handler] = Z_Status;
                    MessageBuffer[uart_handler][MessageIndexEnd[uart_handler]][1] = 'Z';
                    temp_index[uart_handler] = 2;
                }
                else if (temp[0] != 'Z' && PortStatus[uart_handler] == H_Status)
                {
                    PortStatus[uart_handler] = FREE;
                    temp_index[uart_handler] = 0;
                }
                else if (PortStatus[uart_handler] == Z_Status)
                {
                    PortStatus[uart_handler] = MSG;
                    /* Receive length byte */
                    MessageBuffer[uart_handler][MessageIndexEnd[uart_handler]][2] = temp[0];
                    temp_length[uart_handler] = temp[0] + 1; // Length + CRC
                    temp_index[uart_handler] = 3;
                }
                else if (PortStatus[uart_handler] == MSG)
                {
                    MessageBuffer[uart_handler][MessageIndexEnd[uart_handler]][temp_index[uart_handler]] = temp[0];
                    temp_index[uart_handler]++;
                    temp_length[uart_handler]--;

                    if (temp_length[uart_handler] == 0)
                    {
                        // Complete message received
                        MessageIndexEnd[uart_handler]++;
                        if (MessageIndexEnd[uart_handler] == MSG_COUNT)
                        {
                            MessageIndexEnd[uart_handler] = 0;
                        }
                        // ProcessMessageBuffer[ProcessMessageIndexEnd] = uart_handler;
                        ProcessMessageIndexEnd++;
                        if (ProcessMessageIndexEnd == MSG_COUNT)
                        {
                            ProcessMessageIndexEnd = 0;
                        }
                        PortStatus[uart_handler] = FREE;
                    }
                }
            }

            // Process complete messages
            while (ProcessMessageIndexStart != ProcessMessageIndexEnd)
            {
                port_number = ProcessMessageBuffer[ProcessMessageIndexStart];
                port_index = port_number - 1;

                length = MessageBuffer[uart_handler][MessageIndexStart[uart_handler]][2];
                dst = MessageBuffer[uart_handler][MessageIndexStart[uart_handler]][3];

                /* Forward Message in these cases: wrong ID , dst ~= 0 (explore) ,not MULTICAST , not BROADCAST */
                if ((dst != myID) && (dst != 0) && (dst != BOS_BROADCAST) && (dst != BOS_MULTICAST))
                {
                    MessageLength[uart_handler] = length;
                    memcpy(&cMessage[uart_handler][0], &MessageBuffer[uart_handler][MessageIndexStart[uart_handler]][3], length);

                    /* in case trace feature is enabled: */
                    // OptionByte.Trace = ((cMessage[port_number - 1][2] >> 2) & 0x01);
                    // if (OptionByte.Trace)
                    //     IndicatorMode = IND_SHORT_BLINK;

                    // ForwardReceivedMessage(port_number);
                }
                else
                {
                    /* Prepare CRC Buffer and Calculate CRC */
                    AcceptedMessages++;
                    MessageLength[uart_handler] = length;
                    memcpy(&cMessage[uart_handler][0], &MessageBuffer[uart_handler][MessageIndexStart[uart_handler]][3], length);

                    /* Echo all bytes */
                    for (uint32_t i = 0; i < length + 1; i++)
                    {
                        uart_poll_out(uart_rx->port, (char)cMessage[uart_handler][i]);
                    }

                    /* Notify messaging tasks */
                    // NotifyMessagingTask(port_number);
                }

                MessageIndexStart[uart_handler]++;
                if (MessageIndexStart[uart_handler] == MSG_COUNT)
                    MessageIndexStart[uart_handler] = 0;

                ProcessMessageIndexStart++;
                if (ProcessMessageIndexStart == MSG_COUNT)
                    ProcessMessageIndexStart = 0;
            }

            // uint32_t bytes_read = ring_buf_get(&uart_rx->rb, temp, sizeof(temp));
            // for (uint32_t i = 0; i < bytes_read; i++)
            // {
            //     uart_poll_out(uart_rx->port, temp[i]); // Echo all bytes
            // }

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
