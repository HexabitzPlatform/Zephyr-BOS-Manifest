/* Includes ****************************************************************/
#include <zephyr/kernel.h>
#include <BOS.h>

uint8_t PortStatus[NUM_OF_PORTS + 1] = {FREE}; // Initialize all ports to FREE
// uint8_t cMessage[NUM_OF_PORTS][MAX_MESSAGE_SIZE] = {0};

/***************************************************************************/
/* Private function prototypes *********************************************/
/***************************************************************************/
BOS_Status User_MessagingParser(uint16_t code, uint8_t port, uint8_t src, uint8_t dst, uint8_t *data, uint8_t shift);
static BOS_Status HandlePingCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandlePingResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleHiCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleHiResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleEthernetDefaultValuesCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleExploreAdjacentCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleExploreAdjacentResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandlePortDirectionCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleModuleIDCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleTopologyCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleReadPortDirectionResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift); /*CODE_READ_PORT_DIR*/
static BOS_Status HandleBaudRateCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleExploreEEPROMCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleDefArrayCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift); /*def array*/
static BOS_Status HandleCLICommandCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleCLIResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleUpdateCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleUpdateViaPortCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleDMAChannelCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleDMASingleCastStreamCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleReadRemoteCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleReadRemoteResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleWriteRemoteCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleWriteRemoteResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandlePortForwardCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleReadADCVauleCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleReadTempAndVrefCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleAckAcceptedCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleRejectedCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleReadResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleStopModeUartxCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleEnStandbyModeWakeupPinxCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleRawDataCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift);
static BOS_Status HandleDefaultCode(uint8_t src, uint8_t port, uint16_t code, uint8_t *data, uint8_t shift);

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

                    /* Full message received, allocate buffer */
                    if (byte_length[uart_handler] == 0)
                    {
                        uint8_t length = TempMsg[uart_handler][2];
                        uint8_t dst = TempMsg[uart_handler][3];

                        if ((dst != myID) && (dst != 0) && (dst != BOS_BROADCAST) && (dst != BOS_MULTICAST))
                        {
                            /* Forward Received Message to Destination module */
                        }
                        else
                        {
                            /* if dst = myID then do the following: */
                            /* Prepare CRC Buffer and Calculate CRC */
                            uint8_t calculated_crc = calculate_crc8(&TempMsg[uart_handler][0], length + 3); // lenght + 'H' + 'Z' bytes

                            /* if crc is correct then notify PxMessagingTask */
                            if (calculated_crc == TempMsg[uart_handler][length + 3]) // crc byte in the received msg is the last byte
                            {
                                BOS_Message_t *msg_ptr;
                                if (k_mem_slab_alloc(&message_slab, (void **)&msg_ptr, K_NO_WAIT) == 0)
                                {
                                    // uint8_t msg_length = TempMsg[uart_handler][2];

                                    msg_ptr->port = uart_handler;
                                    msg_ptr->length = length;
                                    memcpy(msg_ptr->data, &TempMsg[uart_handler][3], length);

                                    if (k_msgq_put(&bos_packet_msgq, &msg_ptr, K_NO_WAIT) != 0)
                                    {
                                        // k_mem_slab_free(&message_slab, (void *)&msg_ptr);
                                        k_mem_slab_free(&message_slab, (void *)&msg_ptr);
                                        printk(" BOS_Message_t full, message dropped\n");
                                    }
                                    else
                                    {
                                        // for (uint32_t i = 0; i < length; i++)
                                        //     uart_poll_out(uart_rx->port, msg_ptr->data[i]);
                                        UART_Tx(uart_handler, msg_ptr->data, length);
                                    }
                                }
                                else
                                {
                                    printk(" No memory in slab, message dropped\n");
                                }
                            }
                            else
                            {
                                printk("⚠️ CRC mismatch on UART %d\n", uart_handler);
                            }
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
    BOS_Status result = BOS_OK;
    // bool extendOptions = false;
    uint8_t port = 0, src = 0, dst = 0, shift = 0;
    uint16_t code = 0;
    uint8_t *messageParames;

    BOS_Message_t *msg_ptr;
    while (1)
    {
        if (k_msgq_get(&bos_packet_msgq, &msg_ptr, K_FOREVER) == 0)
        {
            port = msg_ptr->port;
            dst = msg_ptr->data[0];
            src = msg_ptr->data[1];
            messageParames = msg_ptr->data;

            shift = 0;

            /* Assign the value of option byte to OptionByte structure */
            *(uint8_t *)&OptionByte = msg_ptr->data[2];

            /* Read message options */
            /* TODO handle extended options case */
            if (OptionByte.ExtendedOptions)
                shift++;

            /* Read message code - LSB first */
            if (OptionByte.ExtendedMessageCode)
            {
                code = (msg_ptr->data[4 + shift] << 8) | msg_ptr->data[3 + shift];
                shift++;
            }
            else
                code = msg_ptr->data[3 + shift];

            /*ACK Massage */
            if (OptionByte.Acknowledgment)
            {
                OptionByte.Acknowledgment = false;
                // SendMessageToModule(src, MSG_ACKNOWLEDGMENT_ACCEPTED, 0);
            }

            /* Set shift index to the start of message payload (parameters) */
            shift += 4;

            switch (code)
            {
            case CODE_UNKNOWN_MESSAGE:
                break;

            case CODE_PING:
                result = HandlePingCode(src, port, messageParames, shift);
                break;

            case CODE_PING_RESPONSE:
                result = HandlePingResponseCode(src, port, messageParames, shift);
                break;

            case CODE_IND_ON:
                // IND_ON();
                break;

            case CODE_IND_OFF:
                // IND_OFF();
                break;

            case CODE_IND_TOGGLE:
                // IND_toggle();
                break;

            case CODE_HI:
                result = HandleHiCode(src, port, messageParames, shift);
                break;

            case CODE_HI_RESPONSE:
                result = HandleHiResponseCode(src, port, messageParames, shift);
                break;

            case CODE_H1DR5_DEFAULT_VALUES:
                result = HandleEthernetDefaultValuesCode(src, port, messageParames, shift);
                break;

#ifndef __N
            case CODE_EXPLORE_ADJ:
                result = HandleExploreAdjacentCode(src, port, messageParames, shift);
                break;

            case CODE_EXPLORE_ADJ_RESPONSE:
                result = HandleExploreAdjacentResponseCode(src, port, messageParames, shift);
                break;
#endif

            case CODE_PORT_DIRECTION:
                result = HandlePortDirectionCode(src, port, messageParames, shift);
                break;

            case CODE_MODULE_ID:
                result = HandleModuleIDCode(src, port, messageParames, shift);
                break;

            case CODE_TOPOLOGY:
                result = HandleTopologyCode(src, port, messageParames, shift);
                break;

            case CODE_READ_PORT_DIR:
                // result = ReadPortsDirMSG(src);
                break;

            case CODE_READ_PORT_DIR_RESPONSE: /** */
                result = HandleReadPortDirectionResponseCode(src, port, messageParames, shift);
                break;

            case CODE_BAUDRATE:
                result = HandleBaudRateCode(src, port, messageParames, shift);
                break;

            case CODE_EXP_EEPROM:
                result = HandleExploreEEPROMCode(src, port, messageParames, shift);
                break;

            case CODE_DEF_ARRAY:
                result = HandleDefArrayCode(src, port, messageParames, shift);
                break;

            case CODE_CLI_COMMAND:
                result = HandleCLICommandCode(src, port, messageParames, shift);
                break;

            case CODE_CLI_RESPONSE:
                result = HandleCLIResponseCode(src, port, messageParames, shift);
                break;

            case CODE_UPDATE:
                result = HandleUpdateCode(src, port, messageParames, shift);
                break;

            case CODE_UPDATE_VIA_PORT:
                result = HandleUpdateViaPortCode(src, port, messageParames, shift);
                break;

            case CODE_DMA_CHANNEL:
                result = HandleDMAChannelCode(src, port, messageParames, shift);
                break;

            case CODE_DMA_SCAST_STREAM:
                result = HandleDMASingleCastStreamCode(src, port, messageParames, shift);
                break;

            case CODE_READ_REMOTE:
                result = HandleReadRemoteCode(src, port, messageParames, shift);
                break;

            case CODE_READ_REMOTE_RESPONSE:
                result = HandleReadRemoteResponseCode(src, port, messageParames, shift);
                break;

            case CODE_WRITE_REMOTE:
                result = HandleWriteRemoteCode(src, port, messageParames, shift);
                break;

            case CODE_WRITE_REMOTE_RESPONSE:
                result = HandleWriteRemoteResponseCode(src, port, messageParames, shift);
                break;

            case CODE_PORT_FORWARD:
                result = HandlePortForwardCode(src, port, messageParames, shift);
                break;

            case CODE_READ_ADC_VALUE:
                result = HandleReadADCVauleCode(src, port, messageParames, shift);
                break;

            case CODE_READ_TEMPERATURE:
            case CODE_READ_VREF:
                result = HandleReadTempAndVrefCode(src, port, messageParames, shift);
                break;

            case MSG_ACKNOWLEDGMENT_ACCEPTED:
                result = HandleAckAcceptedCode(src, port, messageParames, shift);
                break;

            case MSG_REJECTED:
                result = HandleRejectedCode(src, port, messageParames, shift);
                break;

            case CODE_READ_RESPONSE:
                result = HandleReadResponseCode(src, port, messageParames, shift);
                break;

                /* Power Mode: Stop mode enable */
            case CODE_ENABLE_STOP_MODE_UARTX:
                result = HandleStopModeUartxCode(src, port, messageParames, shift);
                break;

                /* Power Mode: Standby mode enable */
            case CODE_ENABLE_STANDBY_MODE_WAKE_UP_PINX:
                result = HandleEnStandbyModeWakeupPinxCode(src, port, messageParames, shift);
                break;

            case CODE_RAW_DATA:
                result = HandleRawDataCode(src, port, messageParames, shift);
                break;

            default:
                result = HandleDefaultCode(src, port, code, messageParames, shift);
                break;
            }

            if (result == BOS_ERR_UnknownMessage)
            {
                // SendMessageToModule(src, CODE_UNKNOWN_MESSAGE, 0);
            }

            // Free the slab after you're done
            // k_mem_slab_free(&message_slab, (void *)&msg_ptr);
            k_mem_slab_free(&message_slab, (void *)&msg_ptr);
        }
    }
}

/***************************************************************************/
/* User message parser Definitions *****************************************/
/***************************************************************************/
/* This function is declared as __weak to be overwritten by other
 * implementations in user file.
 */
__weak BOS_Status User_MessagingParser(uint16_t code, uint8_t port, uint8_t src, uint8_t dst, uint8_t *data, uint8_t shift)
{
    BOS_Status result = BOS_ERR_UnknownMessage;

    return result;
}

/***************************************************************************/
/* Private function Definitions ********************************************/
/***************************************************************************/
// BOS_Status User_MessagingParser(uint16_t code, uint8_t port, uint8_t src, uint8_t dst, uint8_t *data, uint8_t shift)
// {
//     BOS_Status Status = BOS_OK;

//     return Status;
// }
/***************************************************************************/
static BOS_Status HandlePingCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandlePingResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleHiCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleHiResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleEthernetDefaultValuesCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleExploreAdjacentCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleExploreAdjacentResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandlePortDirectionCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleModuleIDCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleTopologyCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleReadPortDirectionResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleBaudRateCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleExploreEEPROMCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleDefArrayCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleCLICommandCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleCLIResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleUpdateCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleUpdateViaPortCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleDMAChannelCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleDMASingleCastStreamCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleReadRemoteCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleReadRemoteResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleWriteRemoteCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleWriteRemoteResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandlePortForwardCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleReadADCVauleCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleReadTempAndVrefCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleAckAcceptedCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleRejectedCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleReadResponseCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleStopModeUartxCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleEnStandbyModeWakeupPinxCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleRawDataCode(uint8_t src, uint8_t port, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
static BOS_Status HandleDefaultCode(uint8_t src, uint8_t port, uint16_t code, uint8_t *data, uint8_t shift)
{
    BOS_Status Status = BOS_OK;

    return Status;
}
/***************************************************************************/
/***************** (C) COPYRIGHT HEXABITZ ***** END OF FILE ****************/