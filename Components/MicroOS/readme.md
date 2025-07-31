# MicroOS 轻量级任务调度器

MicroOS 是一个适用于 STM32 等嵌入式平台的轻量级任务调度器，支持定时任务管理，适合资源受限的单片机项目。

## 主要特性

- 静态任务分配，无动态内存分配
- 支持任务挂起与恢复
- 任务优先级由 ID 决定，ID 越小优先级越高
- 适用于 Keil、STM32 HAL 工程

## 基本 API 用法

### 1. 初始化 MicroOS

```c
MicroOS_Status_t status = MicroOS_Init();
if (status != MICROOS_OK) {
    // 处理初始化失败
}
```

### 2. 添加任务

```c
void MyTask(void *userdata) {
    // 任务代码
}

status = MicroOS_AddTask(0, MyTask, NULL, 100); // ID=0，100ms周期
if (status != MICROOS_OK) {
    // 处理添加失败
}
```

### 3. 启动调度器

```c
MicroOS_RunScheduler();
```

### 4. 在定时器中断中调用 TickHandler

通常在 1ms 定时器中断里调用：

```c
void SysTick_Handler(void) {
    MicroOS_TickHandler();
}
```

### 5. 挂起/恢复任务

```c
MicroOS_SuspendTask(0); // 挂起ID为0的任务
MicroOS_ResumeTask(0);  // 恢复ID为0的任务
```

## 任务函数原型

```c
typedef void (*MicroOS_TaskFunction_t)(void *Userdata);
```

## 注意事项

- 任务数量由 `MICROOS_TASK_NUM` 宏定义，默认10个
- 每个任务 ID 必须唯一且小于 `MICROOS_TASK_NUM`
- 所有任务在同一个 MicroOS 实例中运行

---

如需更多详细说明，请参考头文件 `MicroOS.h` 的注释。