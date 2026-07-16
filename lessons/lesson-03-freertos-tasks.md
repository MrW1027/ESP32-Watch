# 第三课：理解 FreeRTOS 多任务机制

## 本课目标

理解项目中的多任务是如何工作的，为什么需要多任务，以及它们如何协同工作。

---

## 1. 什么是 FreeRTOS？

### 简单理解

**FreeRTOS** = Free Real-Time Operating System（免费实时操作系统）

**类比**：
```
单任务程序 = 一个人做所有事
- 做饭
- 洗衣服
- 打扫卫生
- 一件一件来，效率低

多任务程序 = 雇了几个人，同时做事
- 人 A：专门做饭
- 人 B：专门洗衣服
- 人 C：专门打扫卫生
- 同时进行，效率高
```

### FreeRTOS 的作用

```
FreeRTOS = 任务管理器
- 创建多个任务
- 分配 CPU 时间
- 任务之间通信
- 任务同步
```

---

## 2. 为什么需要多任务？

### 看看我们的项目需求

```
需要同时做的事情：
1. 刷新屏幕显示（每 5ms）
2. 读取心率传感器（每 1 秒）
3. 读取运动传感器（每 100ms）
4. 读取 GPS 数据（持续接收）
5. 响应触摸操作（随时）
```

### 如果只用一个 loop()

```cpp
void loop() {
    // 刷新屏幕
    lv_timer_handler();  // 需要 5ms
    
    // 读取心率
    read_heart_rate();   // 需要 100ms
    
    // 读取运动
    read_mpu6050();      // 需要 10ms
    
    // 读取 GPS
    read_gps();          // 需要 50ms
    
    // 总共需要 165ms
    // 屏幕刷新变成 165ms 一次，会卡顿！
}
```

**问题**：
- 读取心率时，屏幕不刷新（卡顿）
- 读取 GPS 时，触摸没反应
- 所有事情串行执行，效率低

### 使用多任务

```cpp
任务 1（核心 0）：专门刷新屏幕
    while(1) {
        lv_timer_handler();
        delay(5);  // 5ms 一次，流畅
    }

任务 2（核心 1）：专门读取心率
    while(1) {
        read_heart_rate();
        delay(1000);  // 1 秒一次
    }

任务 3（核心 1）：专门读取运动
    while(1) {
        read_mpu6050();
        delay(100);  // 100ms 一次
    }

任务 4（核心 1）：专门读取 GPS
    while(1) {
        read_gps();
        delay(10);  // 持续读取
    }
```

**优势**：
- 各任务独立运行
- 屏幕刷新不受影响
- 传感器数据实时更新
- 充分利用双核 CPU

---

## 3. 项目中的任务分配

### 回顾 main.cpp 中的任务创建

**文件位置**：`src/main.cpp` - setup() 函数

```cpp
void setup() {
    // ... 初始化代码
    
    // 创建任务 1：UI 刷新（核心 0）
    xTaskCreatePinnedToCore(
        lvgl_handler,      // 任务函数
        "lvgl_handler",    // 任务名称
        8192,              // 栈大小（8KB）
        NULL,              // 参数
        2,                 // 优先级
        NULL,              // 任务句柄
        0                  // 绑定到核心 0
    );
    
    // 创建任务 2：GPS 读取（核心 1）
    xTaskCreatePinnedToCore(
        gps_task,          // 任务函数
        "gps_task",        // 任务名称
        4096,              // 栈大小（4KB）
        NULL,              // 参数
        3,                 // 优先级（高）
        NULL,              // 任务句柄
        1                  // 绑定到核心 1
    );
    
    // 创建任务 3：运动传感器（核心 1）
    xTaskCreatePinnedToCore(
        MPU6050_task,      // 任务函数
        "MPU6050_task",    // 任务名称
        8192,              // 栈大小（8KB）
        NULL,              // 参数
        2,                 // 优先级
        NULL,              // 任务句柄
        1                  // 绑定到核心 1
    );
    
    // 创建任务 4：心率血氧（核心 1）
    xTaskCreatePinnedToCore(
        max30102_task,     // 任务函数
        "max30102_task",   // 任务名称
        4096,              // 栈大小（4KB）
        NULL,              // 参数
        4,                 // 优先级（最高）
        NULL,              // 任务句柄
        1                  // 绑定到核心 1
    );
}
```

### 任务分配表

| 任务名称 | 功能 | 核心 | 优先级 | 栈大小 | 刷新频率 |
|---------|------|------|--------|--------|----------|
| lvgl_handler | UI 刷新 | 0 | 2 | 8KB | 5ms |
| gps_task | GPS 定位 | 1 | 3 | 4KB | 持续 |
| MPU6050_task | 运动检测 | 1 | 2 | 8KB | 100ms |
| max30102_task | 心率血氧 | 1 | 4 | 4KB | 1s |

---

## 4. 深入理解任务函数

### 任务函数的结构

所有任务函数都遵循这个模板：

```cpp
void 任务名称(void *pvParameters) {
    // 初始化代码（只运行一次）
    初始化传感器();
    
    // 无限循环（一直运行）
    while (1) {
        // 做任务的工作
        读取数据();
        处理数据();
        更新显示();
        
        // 延时（让出 CPU）
        delay(时间);
    }
    
    // 永远不会执行到这里
    vTaskDelete(NULL);
}
```

### 例子 1：lvgl_handler 任务

**文件位置**：`src/main.cpp` - 全局函数

```cpp
void lvgl_handler(void *pvParameters) {
    while (1) {
        // 获取互斥锁
        if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
            // 让 LVGL 处理 UI 更新
            lv_timer_handler();
            
            // 释放互斥锁
            xSemaphoreGive(xMutex);
        }
        
        // 延时 5ms
        delay(5);
    }
}
```

**工作流程**：
```
1. 尝试获取锁（等待其他任务释放）
2. 获取成功 → 更新 UI
3. 释放锁
4. 延时 5ms（让其他任务运行）
5. 重复
```

**为什么需要锁？**
```
如果没有锁：
任务 A：正在更新心率显示 "75"
任务 B：同时更新时间显示 "12:30"
结果：数据混乱，程序崩溃

有了锁：
任务 A：拿到锁 → 更新心率 → 还锁
任务 B：等待 → 拿到锁 → 更新时间 → 还锁
结果：数据正确，程序稳定
```

### 例子 2：max30102_task 任务（简化版）

**文件位置**：`lib/mylib/my_max30102.cpp` - 全局函数

```cpp
void max30102_task(void* para) {
    // 初始化传感器（只运行一次）
    particleSensor.begin();
    particleSensor.setup();
    
    // 无限循环
    while (1) {
        // 读取 100 个样本
        for (int i = 0; i < 100; i++) {
            while (!particleSensor.available()) {
                particleSensor.check();
            }
            redBuffer[i] = particleSensor.getRed();
            irBuffer[i] = particleSensor.getIR();
            particleSensor.nextSample();
        }
        
        // 计算心率和血氧
        maxim_heart_rate_and_oxygen_saturation(
            irBuffer, 100, redBuffer,
            &spo2, &validSPO2,
            &heartRate, &validHeartRate
        );
        
        // 更新 UI（需要获取锁）
        if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
            lv_label_set_text_fmt(label_heart, "%d", heartRate);
            lv_label_set_text_fmt(label_spo2, "%d", spo2);
            xSemaphoreGive(xMutex);
        }
        
        // 延时 1 秒
        delay(1000);
    }
}
```

**工作流程**：
```
1. 初始化传感器
2. 循环：
   a. 读取 100 个样本（需要几百毫秒）
   b. 计算心率和血氧
   c. 获取锁 → 更新 UI → 释放锁
   d. 延时 1 秒
   e. 重复
```

---

## 5. 任务调度原理

### CPU 如何在任务之间切换？

```
时间线：
0ms    5ms   10ms   15ms   20ms   25ms
│      │     │      │      │      │
├──────┼─────┼──────┼──────┼──────┤
│ 任务A │任务B│ 任务A │任务C │ 任务A │
└──────┴─────┴──────┴──────┴──────┘

每个任务运行一小段时间，然后切换
看起来像是"同时"运行
```

### 任务状态

```
┌─────────┐
│  创建    │
└────┬────┘
     ↓
┌─────────┐
│  就绪    │ ← 等待 CPU
└────┬────┘
     ↓
┌─────────┐
│  运行    │ ← 正在执行
└────┬────┘
     ↓
┌─────────┐
│  阻塞    │ ← 等待事件（delay、获取锁）
└────┬────┘
     ↓
   (循环)
```

### 优先级调度

```
优先级：4 > 3 > 2 > 1

如果多个任务都就绪：
- 优先级高的先运行
- 相同优先级轮流运行

例子：
任务 A（优先级 4）：心率检测
任务 B（优先级 3）：GPS
任务 C（优先级 2）：UI 刷新

如果 A、B、C 都就绪：
运行顺序：A → B → C
```

---

## 6. 双核 CPU 的利用

### ESP32-S3 有两个核心

```
┌─────────────────────────────────┐
│  ESP32-S3                       │
│                                 │
│  ┌───────────┐  ┌───────────┐  │
│  │  核心 0   │  │  核心 1   │  │
│  │           │  │           │  │
│  │ UI 刷新   │  │ GPS 读取  │  │
│  │           │  │ 心率检测  │  │
│  │           │  │ 运动检测  │  │
│  └───────────┘  └───────────┘  │
│                                 │
└─────────────────────────────────┘
```

### 为什么这样分配？

**核心 0**：
- 专门处理 UI
- 避免被传感器任务打断
- 保证界面流畅

**核心 1**：
- 处理所有传感器
- 实时性要求高
- 不影响 UI

---

## 7. 互斥锁（Mutex）详解

### 什么是互斥锁？

```
互斥锁 = 厕所的门锁
- 有人在用时，门是锁着的
- 其他人必须等待
- 用完后，开锁，下一个人进入
```

### 代码中的使用

**文件位置**：`src/main.cpp`

```cpp
// 创建互斥锁（在 setup() 函数中）
SemaphoreHandle_t xMutex;
xMutex = xSemaphoreCreateMutex();

// 使用互斥锁
if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
    // 临界区（只有一个任务能进入）
    操作共享资源();
    
    // 释放锁
    xSemaphoreGive(xMutex);
}
```

### 工作流程

```
任务 A：
1. xSemaphoreTake(xMutex, portMAX_DELAY)  // 尝试获取锁
2. 获取成功 → 进入临界区
3. 操作 LVGL
4. xSemaphoreGive(xMutex)  // 释放锁

任务 B（同时）：
1. xSemaphoreTake(xMutex, portMAX_DELAY)  // 尝试获取锁
2. 锁被任务 A 占用 → 阻塞等待
3. 任务 A 释放锁 → 任务 B 获取成功
4. 操作 LVGL
5. xSemaphoreGive(xMutex)  // 释放锁
```

### 为什么需要互斥锁？

**保护的资源**：LVGL UI

**原因**：
```
LVGL 不是线程安全的
多个任务同时操作 LVGL 会导致：
- 数据混乱
- 内存错误
- 程序崩溃
```

**解决方案**：
```
用互斥锁保护
同一时间只有一个任务能操作 LVGL
```

---

## 8. 任务之间的通信

### 方法 1：全局变量

**文件位置**：`lib/mylib/my_max30102.cpp`

```cpp
// 全局变量
int32_t heartRate = 0;
int32_t spo2 = 0;

// 任务 A：写入数据
void max30102_task(void* para) {
    while (1) {
        // 读取传感器
        heartRate = 读取心率();
        spo2 = 读取血氧();
        delay(1000);
    }
}

// 任务 B：读取数据
void lvgl_handler(void *pvParameters) {
    while (1) {
        // 显示数据
        lv_label_set_text_fmt(label, "心率: %d", heartRate);
        delay(5);
    }
}
```

**优点**：简单
**缺点**：需要注意数据同步

### 方法 2：队列（Queue）

```cpp
// 创建队列
QueueHandle_t dataQueue;
dataQueue = xQueueCreate(10, sizeof(int));

// 任务 A：发送数据
void sensor_task(void* para) {
    while (1) {
        int data = 读取传感器();
        xQueueSend(dataQueue, &data, 0);
        delay(100);
    }
}

// 任务 B：接收数据
void display_task(void* para) {
    while (1) {
        int data;
        if (xQueueReceive(dataQueue, &data, 0) == pdTRUE) {
            显示数据(data);
        }
        delay(5);
    }
}
```

**优点**：线程安全，支持缓冲
**缺点**：稍微复杂

---

## 9. 任务的生命周期

### 创建任务

**文件位置**：`src/main.cpp` - setup() 函数

```cpp
void setup() {
    xTaskCreatePinnedToCore(
        task_function,  // 任务函数
        "task_name",    // 任务名称
        4096,           // 栈大小
        NULL,           // 参数
        2,              // 优先级
        NULL,           // 任务句柄
        0               // CPU 核心
    );
}
```

### 任务运行

```cpp
void task_function(void *pvParameters) {
    // 初始化
    初始化();
    
    // 无限循环
    while (1) {
        做任务();
        delay(100);
    }
}
```

### 删除任务（很少用）

```cpp
void task_function(void *pvParameters) {
    做一次性的工作();
    
    // 删除自己
    vTaskDelete(NULL);
}
```

---

## 10. 实战：理解项目的任务协作

### 场景：用户点击心率按钮

```
1. 用户触摸屏幕
   ↓
2. 触摸芯片检测到触摸
   ↓
3. lvgl_handler 任务：
   - 读取触摸坐标
   - LVGL 判断点击了心率按钮
   - 切换到心率界面
   ↓
4. max30102_task 任务：
   - 持续读取心率数据
   - 计算心率值
   - 获取锁 → 更新 UI → 释放锁
   ↓
5. lvgl_handler 任务：
   - 刷新屏幕
   - 显示心率数值
```

### 任务协作图

```
┌─────────────────┐
│ lvgl_handler    │ ← 核心 0
│ (UI 刷新)       │
└────────┬────────┘
         │
         │ 互斥锁
         │
    ┌────┴────┐
    │  LVGL   │ ← 共享资源
    └────┬────┘
         │
         │ 互斥锁
         │
┌────────┴────────┐
│ max30102_task   │ ← 核心 1
│ (心率检测)      │
└─────────────────┘
```

---

## 11. 调试技巧

### 查看任务状态

**文件位置**：`src/main.cpp` - loop() 函数

```cpp
void loop() {
    // 打印任务信息
    Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
    
    // 查看栈使用情况
    UBaseType_t stackLeft = uxTaskGetStackHighWaterMark(NULL);
    Serial.printf("Stack left: %d bytes\n", stackLeft * 4);
    
    delay(5000);
}
```

### 查看运行在哪个核心

**可以在任何任务函数中使用**

```cpp
void task_function(void *pvParameters) {
    Serial.printf("Task running on core: %d\n", xPortGetCoreID());
    
    while (1) {
        // ...
    }
}
```

---

## 12. 常见问题

### 问题 1：任务栈溢出

**症状**：程序崩溃，重启

**原因**：任务栈太小

**解决**：
```cpp
// 增大栈大小
xTaskCreatePinnedToCore(
    task_function,
    "task_name",
    8192,  // 从 4096 增加到 8192
    NULL, 2, NULL, 0
);
```

### 问题 2：任务优先级设置不当

**症状**：某些任务得不到执行

**原因**：高优先级任务占用太多 CPU

**解决**：
- 降低高优先级任务的优先级
- 在高优先级任务中添加 delay

### 问题 3：死锁

**症状**：程序卡住不动

**原因**：任务获取锁后忘记释放

**解决**：
```cpp
// 确保每个 Take 都有对应的 Give
if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
    操作();
    xSemaphoreGive(xMutex);  // 必须释放！
}
```

---

## 13. 总结

### FreeRTOS 多任务的核心概念

1. **任务**：独立的执行单元
2. **调度**：CPU 在任务之间切换
3. **优先级**：决定任务的执行顺序
4. **互斥锁**：保护共享资源
5. **双核**：充分利用硬件资源

### 项目中的任务分工

```
核心 0：UI 刷新（流畅显示）
核心 1：传感器读取（实时数据）
互斥锁：保护 LVGL（避免冲突）
```

### 关键点

- 每个任务都是无限循环
- 任务之间通过全局变量或队列通信
- 操作共享资源必须加锁
- 合理设置优先级和栈大小

---

## 下一课预告

**第四课：深入理解传感器驱动**

我们会学习：
- MAX30102 心率传感器的工作原理
- I2C 通信协议
- 如何读取和处理传感器数据
- 卡尔曼滤波算法

准备好了告诉我！
