/* Includes ****************************************************************/
#include <BOS.h>

K_POLL_SIGNAL_DEFINE(p1_signal);
K_POLL_SIGNAL_DEFINE(p2_signal);
K_POLL_SIGNAL_DEFINE(p3_signal);
K_POLL_SIGNAL_DEFINE(p4_signal);
K_POLL_SIGNAL_DEFINE(p5_signal);
K_POLL_SIGNAL_DEFINE(p6_signal);

/***************************************************************************/
/* BackEndTask function ****************************************************/
/***************************************************************************/
void BackEndTask(void *p1, void *p2, void *p3)
{
    struct k_poll_signal BackendTask_signal;
    struct k_poll_event BackendTask_event = {
        .type = K_POLL_TYPE_SIGNAL,
        .mode = K_POLL_MODE_NOTIFY_ONLY,
        .signal = &BackendTask_signal,
    };
    while (1)
    {
        K_poll(BackendTask_event, 0, K_FOREVER);

        // Parse UART data
        // Notify P1..P6 accordingly
        // k_poll_signal_raise(&p1_signal, 0);
    }
}

/***************************************************************************/
/* PxMessagingTask function ************************************************/
/***************************************************************************/
void PxMessagingTask(void *p1, void *p2, void *p3)
{
    struct k_poll_signal PxMessagingTask_signal;
    struct k_poll_event PxMessagingTask_event = {
        .type = K_POLL_TYPE_SIGNAL,
        .mode = K_POLL_MODE_NOTIFY_ONLY,
        .signal = &PxMessagingTask_signal,
    };
    while (1)
    {
        K_poll(PxMessagingTask_event, 0, K_FOREVER);
    }
}
