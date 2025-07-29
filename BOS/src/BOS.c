#include "BOS.h"

uart_rx_all_port_t uart_ring_buffer[NUM_OF_PORTS];
uart_event_port_len_t uart_data_info;
// BOS_Message_t bos_msg;

uint16_t ArrayPortsDir[MAX_NUM_OF_MODULES];
uint8_t myID = 2;