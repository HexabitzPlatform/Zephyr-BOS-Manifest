#include "BOS.h"

uart_rx_all_port_t uart_ring_buffer[NUM_OF_PORTS];
uart_event_port_len_t uart_data_info;
// BOS_Message_t bos_msg;

uint16_t ArrayPortsDir[MAX_NUM_OF_MODULES];
uint8_t myID = 1;

/**
 * @brief Calculates an 8-bit CRC using polynomial 0x4C11DB7 over 4-byte reversed data chunks.
 *
 * @param data Pointer to the input data array
 * @param length Length of the input data in bytes
 * @return 8-bit CRC result
 */
uint8_t calculate_crc8(const uint8_t *data, size_t length)
{
    uint32_t crc = 0xFFFFFFFF;

    // Calculate ceil(length / 4)
    size_t l = (length + 3) / 4;
    if (l <= 2)
        l = 2;

    size_t padded_len = l * 4;

    // Allocate buffer for reversed data (4-byte reversed blocks)
    uint8_t reversed[padded_len];
    memset(reversed, 0, padded_len); // Zero padding

    // Reorder bytes in each 4-byte block: reverse order
    size_t shift = 0;
    for (size_t i = 0; i < l; ++i)
    {
        for (int jj = 3; jj >= 0; --jj)
        {
            size_t src_index = shift + jj;
            size_t dst_index = shift + (3 - jj);

            if (src_index < length)
                reversed[dst_index] = data[src_index];
            else
                reversed[dst_index] = 0;
        }
        shift += 4;
    }

    // Compute CRC using polynomial 0x4C11DB7 (standard Ethernet CRC32)
    for (size_t i = 0; i < padded_len; ++i)
    {
        crc ^= ((uint32_t)reversed[i]) << 24;
        for (int j = 0; j < 8; ++j)
        {
            uint32_t msb = crc >> 31;
            crc <<= 1;
            if (msb)
                crc ^= 0x4C11DB7;
        }
    }

    return (uint8_t)(crc & 0xFF);
}