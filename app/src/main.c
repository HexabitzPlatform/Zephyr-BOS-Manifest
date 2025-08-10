#include <BOS.h>

void UserTask(void)
{
    UARTInit();

    led_init();

    SwapUartPins(GetUart(P1), REVERSED);

    while (1)
    {
        // led_blink(60);

        led_toggle();
        k_msleep(500);
    }
}