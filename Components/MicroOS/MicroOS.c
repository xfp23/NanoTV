#include "MicroOS.h"
#include "stdlib.h"

static volatile MicroOS_t MicroOS = {0};

 MicroOS_Handle_t  MicroOS_handle = &MicroOS;

MicroOS_Status_t MicroOS_Init()
{
    MICROOS_CHECK_PTR(MicroOS_handle);

    MicroOS_handle->MaxTasks = MICROOS_TASK_NUM;
	MicroOS_handle->TaskNum = 0;
    MicroOS_handle->TickCount = 0;
    MicroOS_handle->CurrentTaskId = 0;

    return MICROOS_OK;
}

MicroOS_Status_t MicroOS_AddTask(uint8_t id, MicroOS_TaskFunction_t TaskFunction, void *Userdata, uint32_t Period)
{
    MICROOS_CHECK_PTR(MicroOS_handle);
    MICROOS_CHECK_ID(id);
    MICROOS_CHECK_PTR(TaskFunction);
	
	if(MicroOS_handle->TaskNum > MICROOS_TASK_NUM) return MICROOS_ERROR;
	MicroOS_handle->TaskNum++;
    MicroOS_handle->Tasks[id].TaskFunction = TaskFunction;
    MicroOS_handle->Tasks[id].Userdata = Userdata;
    MicroOS_handle->Tasks[id].Period = Period;
    MicroOS_handle->Tasks[id].LastRunTime = 0;
    MicroOS_handle->Tasks[id].IsRunning = true;

    return MICROOS_OK;
}

void MicroOS_RunScheduler(void)
{

    while (1)
    {
        if (MicroOS_handle == NULL)
        {
            return;
        }
        // 遍历所有任务
        for (uint8_t i = 0; i < MicroOS_handle->TaskNum; i++)
        {
            if (MicroOS_handle->Tasks[i].IsRunning)
            {
                uint32_t currentTime = MicroOS_handle->TickCount;
                if ((currentTime - MicroOS_handle->Tasks[i].LastRunTime) >= MicroOS_handle->Tasks[i].Period)
                {
                    MicroOS_handle->CurrentTaskId = i; // 当前任务ID
                    MicroOS_handle->Tasks[i].TaskFunction(MicroOS_handle->Tasks[i].Userdata);
                    MicroOS_handle->Tasks[i].LastRunTime = currentTime;
                }
            }
        }
    }
}

MicroOS_Status_t MicroOS_TickHandler(void)
{
    MICROOS_CHECK_PTR(MicroOS_handle);

    MicroOS_handle->TickCount++;
	
	return MICROOS_OK;
}

MicroOS_Status_t MicroOS_SuspendTask(uint8_t id)
{
    MICROOS_CHECK_PTR(MicroOS_handle);
    MICROOS_CHECK_ID(id);

    MicroOS_handle->Tasks[id].IsRunning = false;
    return MICROOS_OK;
}

MicroOS_Status_t MicroOS_ResumeTask(uint8_t id)
{
    MICROOS_CHECK_PTR(MicroOS_handle);
    MICROOS_CHECK_ID(id);

    MicroOS_handle->Tasks[id].IsRunning = true;
    return MICROOS_OK;
}
