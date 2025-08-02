/* Includes ****************************************************************/
#include <zephyr/kernel.h>
#include <BOS.h>

/***************************************************************************/
/*****************************  Private Functions **************************/
/***************************************************************************/
BOS_Status SendMessageToModule(uint8_t dst, uint16_t code, uint8_t *pParameters, uint16_t numberOfParams)
{
    BOS_Status result = BOS_OK;

    return result;
}
/***************************************************************************/
BOS_Status SendMessageFromPort(uint8_t port, uint8_t src, uint8_t dst, uint16_t code, uint8_t *pParameters, uint16_t numberOfParams)
{
    BOS_Status result = BOS_OK;

    uint8_t Message[MAX_MESSAGE_SIZE] = {0};
    uint8_t shift = 0;
    uint8_t length = 0;
    uint8_t calculated_crc = 0;

    /* Increase the priority of current running task */

    /***************************************************************************/
    /* Start constructing the message buffer ***********************************/
    /***************************************************************************/

    /* Sending to adjacent neighbors - case 2, case 8 and part of case 6 */
    if (src == 0)
        src = myID;

    /* ToDo: Implement extended options */
    if (OptionByte.ExtendedOptions == true)
        ++shift;

    /* Extended code flag? */
    if (code > 0xFF)
        OptionByte.ExtendedMessageCode = true;

    /* Construct the message */
    /* Add the HZ Delimiter to mark the start of the message */
    Message[0] = 'H';
    Message[1] = 'Z';

    Message[3] = dst;
    Message[4] = src;
    /* Assign the Options byte value */
    Message[5] = *(uint8_t *)&OptionByte;

    /* Code - LSB first */
    Message[6 + shift] = (uint8_t)code;

    if (OptionByte.ExtendedMessageCode)
    {
        ++shift;
        Message[6 + shift] = (uint8_t)(code >> 8);
    }

    /* Parameters */
    if (numberOfParams <= MAX_PARAMS_PER_MESSAGE)
    {
        memcpy((char *)&Message[7 + shift], pParameters, numberOfParams);
        /* Calculate message length */
        length = numberOfParams + shift + 4;
    }
    else
        return result;

    Message[2] = length;

    calculated_crc = calculate_crc8(Message, length + 3);
    Message[length + 3] = calculated_crc;

    /* Send the Constructed Message over UART */
    uart_poll_out(GetUart(port), Message[0]);

    /* Restore the original task priority */

    return result;
}