#include "BOS.h"

struct uart_rx_all_port uart_ring_buffer[NUM_OF_PORTS];
struct k_poll_signal PxMessagingTask_signal;
struct uart_event_msg msg;

uint16_t ArrayPortsDir[MAX_NUM_OF_MODULES];
uint8_t myID = 2;