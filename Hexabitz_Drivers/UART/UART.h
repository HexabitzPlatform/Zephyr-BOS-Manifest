#ifndef UART_H
#define UART_H

// /* C STD Library */
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdarg.h>
// #include <string.h>
// #include <ctype.h>
// #include <math.h>
// #include <limits.h>

#include <BOS.h>

#define UART_PORTS 6
#define UART_RX_SIZE 192
#define MAX_MESSAGE_SIZE 56

#if DT_NODE_EXISTS(DT_NODELABEL(usart1))
#define UART_1_DEVICE_NODE DT_NODELABEL(usart1)
static const struct device *const uart1_dev = DEVICE_DT_GET(UART_1_DEVICE_NODE);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart2))
#define UART_2_DEVICE_NODE DT_NODELABEL(usart2)
static const struct device *const uart2_dev = DEVICE_DT_GET(UART_2_DEVICE_NODE);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart3))
#define UART_3_DEVICE_NODE DT_NODELABEL(usart3)
static const struct device *const uart3_dev = DEVICE_DT_GET(UART_3_DEVICE_NODE);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart4))
#define UART_4_DEVICE_NODE DT_NODELABEL(usart4)
static const struct device *const uart4_dev = DEVICE_DT_GET(UART_4_DEVICE_NODE);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart5))
#define UART_5_DEVICE_NODE DT_NODELABEL(usart5)
static const struct device *const uart5_dev = DEVICE_DT_GET(UART_5_DEVICE_NODE);
#endif

#if DT_NODE_EXISTS(DT_NODELABEL(usart6))
#define UART_6_DEVICE_NODE DT_NODELABEL(usart6)
static const struct device *const uart6_dev = DEVICE_DT_GET(UART_6_DEVICE_NODE);
#endif

enum
{
    UART_PORT_1 = 0,
    UART_PORT_2,
    UART_PORT_3,
    UART_PORT_4,
    UART_PORT_5,
    UART_PORT_6
};

struct uart_rx_all_port
{
    struct ring_buf rb;

    uint8_t rb_buffer[UART_RX_SIZE];      // buffer used by ring_buf
    uint8_t rx_dma_buf[MAX_MESSAGE_SIZE]; // buffer passed to uart_rx_enable
};

void UARTInit(void);

static struct uart_rx_all_port uart_ports[UART_PORTS];
#endif