#include <BOS.h>

#define BOS_TASKS_STACK_SIZE 1024

#define BACKEND_TASK_PRIORITY 5
#define PX_MESSAGING_TASK_PRIORITY 4
#define DEFAULT_TASK_PRIORITY 3
#define USER_TASK_PRIORITY 3

void BackEndTask(void *p1, void *p2, void *p3);
void PxMessagingTask(void *p1, void *p2, void *p3);
void DefaultTask(void *p1, void *p2, void *p3);
void UserTask(void *p1, void *p2, void *p3);

/* Define thread stacks */
K_THREAD_STACK_DEFINE(backend_stack, BOS_TASKS_STACK_SIZE);
K_THREAD_STACK_DEFINE(default_stack, BOS_TASKS_STACK_SIZE);
K_THREAD_STACK_DEFINE(user_stack, BOS_TASKS_STACK_SIZE);
K_THREAD_STACK_DEFINE(p1_stack, BOS_TASKS_STACK_SIZE);
K_THREAD_STACK_DEFINE(p2_stack, BOS_TASKS_STACK_SIZE);
K_THREAD_STACK_DEFINE(p3_stack, BOS_TASKS_STACK_SIZE);
K_THREAD_STACK_DEFINE(p4_stack, BOS_TASKS_STACK_SIZE);
K_THREAD_STACK_DEFINE(p5_stack, BOS_TASKS_STACK_SIZE);
K_THREAD_STACK_DEFINE(p6_stack, BOS_TASKS_STACK_SIZE);

/* Define thread control blocks */
struct k_thread BackEndTaskHandle;
struct k_thread DefaultTaskHandle;
struct k_thread UserTaskHandle;
struct k_thread P1MsgTaskHandle;
struct k_thread P2MsgTaskHandle;
struct k_thread P3MsgTaskHandle;
struct k_thread P4MsgTaskHandle;
struct k_thread P5MsgTaskHandle;
struct k_thread P6MsgTaskHandle;

/* Thread priorities */
#define THREAD_OPTIONS 0

void ZephyrKernalInit(void)
{
    k_thread_create(&BackEndTaskHandle, backend_stack, K_THREAD_STACK_SIZEOF(backend_stack),
                    BackEndTask, NULL, NULL, NULL, BACKEND_TASK_PRIORITY, THREAD_OPTIONS, K_NO_WAIT);

    k_thread_create(&DefaultTaskHandle, default_stack, K_THREAD_STACK_SIZEOF(default_stack),
                    DefaultTask, NULL, NULL, NULL, DEFAULT_TASK_PRIORITY, THREAD_OPTIONS, K_NO_WAIT);

    k_thread_create(&UserTaskHandle, user_stack, K_THREAD_STACK_SIZEOF(user_stack),
                    UserTask, NULL, NULL, NULL, USER_TASK_PRIORITY, THREAD_OPTIONS, K_NO_WAIT);

    k_thread_create(&P1MsgTaskHandle, p1_stack, K_THREAD_STACK_SIZEOF(p1_stack),
                    PxMessagingTask, NULL, NULL, NULL, PX_MESSAGING_TASK_PRIORITY, THREAD_OPTIONS, K_NO_WAIT);

    k_thread_create(&P2MsgTaskHandle, p2_stack, K_THREAD_STACK_SIZEOF(p2_stack),
                    PxMessagingTask, NULL, NULL, NULL, PX_MESSAGING_TASK_PRIORITY, THREAD_OPTIONS, K_NO_WAIT);

    k_thread_create(&P3MsgTaskHandle, p3_stack, K_THREAD_STACK_SIZEOF(p3_stack),
                    PxMessagingTask, NULL, NULL, NULL, PX_MESSAGING_TASK_PRIORITY, THREAD_OPTIONS, K_NO_WAIT);

    k_thread_create(&P4MsgTaskHandle, p4_stack, K_THREAD_STACK_SIZEOF(p4_stack),
                    PxMessagingTask, NULL, NULL, NULL, PX_MESSAGING_TASK_PRIORITY, THREAD_OPTIONS, K_NO_WAIT);

    k_thread_create(&P5MsgTaskHandle, p5_stack, K_THREAD_STACK_SIZEOF(p5_stack),
                    PxMessagingTask, NULL, NULL, NULL, PX_MESSAGING_TASK_PRIORITY, THREAD_OPTIONS, K_NO_WAIT);

    k_thread_create(&P6MsgTaskHandle, p6_stack, K_THREAD_STACK_SIZEOF(p6_stack),
                    PxMessagingTask, NULL, NULL, NULL, PX_MESSAGING_TASK_PRIORITY, THREAD_OPTIONS, K_NO_WAIT);
}

void DefaultTask(void *p1, void *p2, void *p3)
{

    while (1)
    {

        k_sleep(K_MSEC(10));
    }
}
