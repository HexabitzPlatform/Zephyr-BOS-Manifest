#include <BOS.h>

#define BOS_TASKS_STACK_SIZE 1024

#define BACKEND_TASK_PRIORITY 5
#define PX_MESSAGING_TASK_PRIORITY 4
#define DEFAULT_TASK_PRIORITY 3
#define USER_TASK_PRIORITY 3

void BackEndTask(void);
void PxMessagingTask(void);
void DefaultTask(void);
void UserTask(void);

/* Create the Backend task */
K_THREAD_DEFINE(BackEndTaskHandle, BOS_TASKS_STACK_SIZE, BackEndTask, NULL, NULL, NULL,
                BACKEND_TASK_PRIORITY, 0, 0);

/* Create the defaultTask */
K_THREAD_DEFINE(DefaultTaskHandle, BOS_TASKS_STACK_SIZE, DefaultTask, NULL, NULL, NULL,
                DEFAULT_TASK_PRIORITY, 0, 0);

/* Create the User task */
K_THREAD_DEFINE(UserTaskHandle, BOS_TASKS_STACK_SIZE, UserTask, NULL, NULL, NULL,
                USER_TASK_PRIORITY, 0, 0);

/* Create message parsing tasks for module ports */

K_THREAD_DEFINE(P1MsgTaskHandle, BOS_TASKS_STACK_SIZE, PxMessagingTask, NULL, NULL, NULL,
                PX_MESSAGING_TASK_PRIORITY, 0, 0);

K_THREAD_DEFINE(P2MsgTaskHandle, BOS_TASKS_STACK_SIZE, PxMessagingTask, NULL, NULL, NULL,
                PX_MESSAGING_TASK_PRIORITY, 0, 0);

K_THREAD_DEFINE(P3MsgTaskHandle, BOS_TASKS_STACK_SIZE, PxMessagingTask, NULL, NULL, NULL,
                PX_MESSAGING_TASK_PRIORITY, 0, 0);

K_THREAD_DEFINE(P4MsgTaskHandle, BOS_TASKS_STACK_SIZE, PxMessagingTask, NULL, NULL, NULL,
                PX_MESSAGING_TASK_PRIORITY, 0, 0);

K_THREAD_DEFINE(P5MsgTaskHandle, BOS_TASKS_STACK_SIZE, PxMessagingTask, NULL, NULL, NULL,
                PX_MESSAGING_TASK_PRIORITY, 0, 0);

K_THREAD_DEFINE(P6MsgTaskHandle, BOS_TASKS_STACK_SIZE, PxMessagingTask, NULL, NULL, NULL,
                PX_MESSAGING_TASK_PRIORITY, 0, 0);

void DefaultTask(void)
{
}
