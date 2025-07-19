#ifndef BOS_H
#define BOS_H

/* Includes ****************************************************************/
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>

#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/ring_buffer.h>

/* C STD Library */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

#include <UART.h>
#include <ADC.h>
#include <Timer.h>

#ifdef CONFIG_BOARD_H01R0
#include <H01R0.h>
#endif

// #define BOS_NODE DT_NODELABEL(bos)

// uint32_t ports = DT_PROP(BOS_NODE, number_ports);
// uint32_t part_number = DT_PROP(BOS_NODE, modulePN);

// #define BOS_NODE DT_NODELABEL(bos)
// #define BOS_NUMBER_PORTS DT_PROP(BOS_NODE, number_ports)
// #define BOS_PART_NUMBER DT_PROP(BOS_NODE, modulePN)

void ZephyrKernalInit(void);

#endif