#ifndef __TASKS_H
#define __TASKS_H

#define TASK_FAULT_STATE_PRIO               0
#define TASK_CRITICAL_STATE_PRIO            1
#define TASK_PETWDOG_PRIO                   2
#define TASK_VOLT_TEMP_MONITOR_PRIO         3
#define TASK_AMPERES_MONITOR_PRIO           4
#define TASK_CANBUS_PRIO                    5
#define TASK_LOG_INFO_PRIO                  6
#define TASK_CLI_PRIO                       7
#define TASK_BLE_PRIO                       8
#define TASK_IDLE_PRIO                      9


#define DEFAULT_STACK_SIZE                  256

#define TASK_FAULT_STATE_STACK_SIZE         DEFAULT_STACK_SIZE
#define TASK_CRITICAL_STATE_STACK_SIZE      DEFAULT_STACK_SIZE
#define TASK_PETWDOG_STACK_SIZE             DEFAULT_STACK_SIZE
#define TASK_VOLT_TEMP_MONITOR_STACK_SIZE   512
#define TASK_AMPERES_MONITOR_STACK_SIZE     DEFAULT_STACK_SIZE
#define TASK_CANBUS_STACK_SIZE              DEFAULT_STACK_SIZE
#define TASK_LOG_INFO_STACK_SIZE            DEFAULT_STACK_SIZE
#define TASK_CLI_STACK_SIZE                 DEFAULT_STACK_SIZE
#define TASK_BLE_STACK_SIZE                 DEFAULT_STACK_SIZE
#define TASK_IDLE_STACK_SIZE                DEFAULT_STACK_SIZE

#endif
