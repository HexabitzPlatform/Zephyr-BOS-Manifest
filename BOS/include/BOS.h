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

void ZephyrKernalInit(void);

#endif