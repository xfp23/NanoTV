#ifndef MICROOS_H
#define MICROOS_H

/**
 * @file MicroOS.h
 * @author xfp23
 * @brief 轻量级的系统调度器
 * @note 该调度器仅支持单实例运行，所有任务都在同一个MicroOS实例中运行。
 *       1. 每个任务都有一个唯一的ID，ID可以代表任务的优先级。id越小，优先级越高
 *       2. 任务支持的数量为MICROOS_TASK_NUM，默认为10个任务
 *       3. 没有用堆栈动态分配任务，使用了栈静态分配任务，考虑到嵌入式系统的资源限制以及堆分配造成内存碎片问题。
 *       4. 如果想让系统调度器变快，只需要更改MicroOS_TickHandler在系统时钟中断的调用频率即可。
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

#define MICROOS_TASK_NUM (10) // 任务数量

// 检查空指针
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

// 任务函数
typedef void (*MicroOS_TaskFunction_t)(void *Userdata);

typedef enum
{
    MICROOS_OK = 0,          // 成功
    MICROOS_ERROR,           // 错误
    MICROOS_TIMEOUT,         // 超时
    MICROOS_INVALID_PARAM,   // 无效参数
    MICROOS_NOT_INITIALIZED, // 未初始化
} MicroOS_Status_t;          // 状态码

typedef struct
{
    // uint8_t id;
    MicroOS_TaskFunction_t TaskFunction; // 任务函数指针
    void *Userdata;                      // 用户数据
    uint32_t Period;                     // 周期（单位：毫秒）
    uint32_t LastRunTime;                // 上次运行时间
    bool IsRunning;                      // 是否运行
} MicroOS_Task_t;

typedef struct
{
    MicroOS_Task_t Tasks[MICROOS_TASK_NUM]; // 任务堆栈
    uint32_t TickCount;                     // MicroOS时钟滴答计数
    uint32_t MaxTasks;                      // 最大任务数量
    uint8_t CurrentTaskId;               // 当前任务ID
	uint8_t TaskNum;                     // 任务数量
} MicroOS_t;

typedef volatile MicroOS_t* MicroOS_Handle_t;

/**
 * @brief MicroOS实例
 * 
 * 该实例是MicroOS的唯一实例，所有任务都在这个实例中运行。
 */
extern MicroOS_Handle_t MicroOS_handle;

/**
 * @brief 初始化MicroOS实例
 *
 * @return MicroOS_Status_t 操作状态码
 */
extern MicroOS_Status_t MicroOS_Init(void);

/**
 * @brief
 *
 * @param id id也可代表优先级,不可以超过 MICROOS_TASK_NUM,也不可以有两个相同的优先级存在
 * @param TaskFunction 任务函数
 * @param Userdata 用户自定义数据
 * @param Period 任务周期
 * @return MicroOS_Status_t 操作状态码
 */
extern MicroOS_Status_t MicroOS_AddTask(uint8_t id, MicroOS_TaskFunction_t TaskFunction, void *Userdata, uint32_t Period);

/**
 * @brief 启动MicroOS调度器，开始运行任务
 *
 */
extern void MicroOS_RunScheduler(void);

/**
 * @brief 滴答处理函数，通常在系统时钟中断中调用
 * @note 该函数会增加TickCount计数器，MicroOS的任务调度依赖于TickCount。一般在1ms定时器中断调用
 * @return MicroOS_Status_t
 */
extern MicroOS_Status_t MicroOS_TickHandler(void);

/**
 * @brief 挂起指定ID的任务
 *
 * @param id 任务id
 * @return MicroOS_Status_t
 */
extern MicroOS_Status_t MicroOS_SuspendTask(uint8_t id);

/**
 * @brief 恢复指定ID的任务
 *
 * @param id
 * @return MicroOS_Status_t
 */
extern MicroOS_Status_t MicroOS_ResumeTask(uint8_t id);

#ifdef __cplusplus
}
#endif

#endif // !MICROOS_H
