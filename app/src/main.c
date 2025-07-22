/*
 * Copyright (c) 2024 STMicroelectronics
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <BOS.h>

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(module_led), gpios);

void UserTask(void)
{
    UARTInit();

    gpio_is_ready_dt(&led);

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

    while (1)
    {
        gpio_pin_toggle_dt(&led);

        k_sleep(K_SECONDS(1));
    }
}