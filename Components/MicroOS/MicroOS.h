#ifndef MICROOS_H
#define MICROOS_H

/**
 * @file MicroOS.h
 * @author xfp23
 * @brief Lightweight system scheduler
 * @note This scheduler only supports single-instance operation, all tasks run in the same MicroOS instance.
 *       1. Each task has a unique ID, which can represent task priority. The smaller the ID, the higher the priority.
 *       2. The number of supported tasks is defined by MICROOS_TASK_NUM, default is 10.
 *       3. No dynamic stack allocation for tasks, static allocation is used considering embedded resource constraints and heap fragmentation issues.
 *       4. To speed up the scheduler, simply increase the frequency of MicroOS_TickHandler calls in the system clock interrupt.
 * @version 0.1
 * @date 2025-07-31
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif

// MicroOS version
#define MICROOS_VERSION_MAJOR "0.0.1"

#define MICROOS_TASK_NUM (10) // Number of tasks

// Null pointer check
#define MICROOS_CHECK_PTR(ptr)    \
do                            \
{                             \
    if ((ptr) == NULL)        \
    {                         \
        return MICROOS_ERROR; \
    }                         \
} while (0)

#define MIROOS_CHECK_ERR(err)       \
do                              \
{                               \
    MicroOS_Status_t ret = err; \
    if (ret != MICROOS_OK)      \
    {                           \
        return ret;             \
    }                           \
} while (0)

#define MICROOS_CHECK_ID(id)              \
do                                    \
{                                     \
    if (id >= MICROOS_TASK_NUM)       \
    {                                 \
        return MICROOS_INVALID_PARAM; \
    }                                 \
} while (0)

// Task function prototype
typedef void (*MicroOS_TaskFunction_t)(void *Userdata);

typedef enum
{
    MICROOS_OK = 0,          // Success
    MICROOS_ERROR,           // Error
    MICROOS_TIMEOUT,         // Timeout
    MICROOS_INVALID_PARAM,   // Invalid parameter
    MICROOS_NOT_INITIALIZED, // Not initialized
} MicroOS_Status_t;          // Status code

typedef struct
{
    // uint8_t id;
    MicroOS_TaskFunction_t TaskFunction; // Task function pointer
    void *Userdata;                      // User data
    uint32_t Period;                     // Period (ms)
    uint32_t LastRunTime;                // Last run time
    bool IsRunning;                      // Is running
} MicroOS_Task_t;

typedef struct
{
    MicroOS_Task_t Tasks[MICROOS_TASK_NUM]; // Task stack
    uint32_t TickCount;                     // MicroOS tick counter
    uint32_t MaxTasks;                      // Maximum number of tasks
    uint8_t CurrentTaskId;                  // Current task ID
    uint8_t TaskNum;                        // Number of tasks
} MicroOS_t;

typedef volatile MicroOS_t *MicroOS_Handle_t;

/**
 * @brief MicroOS instance
 *
 * This is the only instance of MicroOS, all tasks run in this instance.
 */
extern MicroOS_Handle_t MicroOS_handle;

/**
 * @brief Initialize MicroOS instance
 *
 * @return MicroOS_Status_t Operation status code
 */
extern MicroOS_Status_t MicroOS_Init(void);

/**
 * @brief Add a task
 *
 * @param id Task ID, also represents priority. Must not exceed MICROOS_TASK_NUM, and IDs must be unique.
 * @param TaskFunction Task function
 * @param Userdata User custom data
 * @param Period Task period
 * @return MicroOS_Status_t Operation status code
 */
extern MicroOS_Status_t MicroOS_AddTask(uint8_t id, MicroOS_TaskFunction_t TaskFunction, void *Userdata, uint32_t Period);

/**
 * @brief Start the MicroOS scheduler and begin running tasks
 *
 */
extern void MicroOS_RunScheduler(void);

/**
 * @brief Tick handler, usually called in the system clock interrupt
 * @note This function increases the TickCount counter, which is used for task scheduling. Typically called in a 1ms timer interrupt.
 * @return MicroOS_Status_t
 */
extern MicroOS_Status_t MicroOS_TickHandler(void);

/**
 * @brief Suspend the task with the specified ID
 *
 * @param id Task ID
 * @return MicroOS_Status_t
 */
extern MicroOS_Status_t MicroOS_SuspendTask(uint8_t id);

/**
 * @brief Resume the task with the specified ID
 *
 * @param id
 * @return MicroOS_Status_t
 */
extern MicroOS_Status_t MicroOS_ResumeTask(uint8_t id);

#ifdef __cplusplus
}
#endif

#endif // !MICROOS_H
