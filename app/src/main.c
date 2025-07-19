/*
 * Copyright (c) 2024 STMicroelectronics
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <BOS.h>

// void print_uart(char *buf, int len)
// {
//     for (int i = 0; i < len; i++)
//     {

//         if ((buf[i] == '\n' || buf[i] == '\r'))
//         {
//             uart_poll_out(uart6_dev, '\n');
//         }
//         else
//         {
//             uart_poll_out(uart6_dev, buf[i]);
//         }
//     }
// }

int main(void)
{
    UARTInit();

    uint8_t rx_data[100];
    size_t len;

    while (1)
    {

        // /* Check if there's data in the ring buffer */
        // len = ring_buf_get(&uart2_dev, rx_data, sizeof(rx_data));

        // if (len > 0)
        // {
        //     /* Process `len` bytes of data */
        //     print_uart(rx_data, len);
        // }
    }
}

void UserTask(void)
{
}