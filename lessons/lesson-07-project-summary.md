# 第七课：项目总结和架构分析

## 本课目标

全面理解整个项目的架构、数据流向、各模块如何协作，以及如何扩展新功能。

---

## 1. 项目整体架构

### 1.1 分层架构

```
┌─────────────────────────────────────────────┐
│          用户界面层（LVGL UI）               │
│  - 主界面、心率界面、天气界面、设置界面      │
│  - 按钮、标签、图片、滑块等控件              │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────┴───────────────────────────┐
│          应用逻辑层（任务和定时器）          │
│  - lvgl_handler: UI 刷新                    │
│  - max30102_task: 心率血氧检测              │
│  - MPU6050_task: 运动检测                   │
│  - gps_task: GPS 定位                       │
│  - LVGL 定时器: 定时更新 UI                 │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────┴───────────────────────────┐
│          驱动层（硬件驱动）                  │
│  - TFT_eSPI: 显示屏驱动                     │
│  - CST816T: 触摸驱动                        │
│  - MAX30102: 心率传感器驱动                 │
│  - MPU6050: 运动传感器驱动                  │
│  - GPS: 定位模块驱动                        │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────┴───────────────────────────┐
│          操作系统层（FreeRTOS）              │
│  - 任务调度                                  │
│  - 互斥锁管理                                │
│  - 内存管理                                  │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────┴───────────────────────────┐
│          硬件层（ESP32-S3）                  │
│  - 双核 CPU                                  │
│  - WiFi/蓝牙                                 │
│  - GPIO、I2C、SPI、UART                     │
└─────────────────────────────────────────────┘
```

### 1.2 文件结构

```
项目根目录/
├── src/
│   └── main.cpp                    # 主程序入口
├── lib/
│   ├── mylib/                      # 自定义库
│   │   ├── WiFiUser.cpp/h          # WiFi 连接
│   │   ├── my_signal.cpp/h         # 网络 API 调用
│   │   ├── my_max30102.cpp/h       # 心率传感器
│   │   ├── my_mpu6050.cpp/h        # 运动传感器
│   │   ├── my_gps.cpp/h            # GPS 模块
│   │   ├── my_mqtt.cpp/h           # MQTT 通信
│   │   └── my_timer.cpp/h          # 定时器和工具函数
│   └── generated/                  # LVGL GUI Designer 生成
│       ├── gui_guider.c/h          # UI 结构定义
│       ├── events_init.cpp/h       # 事件回调
│       ├── custom.c/h              # 自定义代码
│       └── setup_scr_*.cpp         # 各界面初始化
└── platformio.ini                  # PlatformIO 配置
```

---

## 2. 核心数据流向

### 2.1 传感器数据流

```
传感器硬件
    ↓ (I2C/SPI/UART)
传感器任务读取原始数据
    ↓
算法处理（滤波、计算）
    ↓
保存到全局变量
    ↓
LVGL 定时器读取变量
    ↓
获取互斥锁
    ↓
更新 UI 控件
    ↓
释放互斥锁
    ↓
lvgl_handler 刷新屏幕
    ↓
用户看到数据
```


### 2.2 心率数据流详细示例

**文件位置**：
- 传感器任务：`lib/mylib/my_max30102.cpp` - max30102_task()
- 定时器更新：`lib/mylib/my_timer.cpp` - my_timer2()
- UI 刷新：`src/main.cpp` - lvgl_handler()

```
1. MAX30102 传感器（硬件）
   ↓ I2C 通信
2. max30102_task() 读取红光和红外光数据
   位置：lib/mylib/my_max30102.cpp
   ↓
3. maxim_heart_rate_and_oxygen_saturation() 计算
   ↓
4. 验证数据有效性
   if (validHeartRate == 1 && heartRate > 50 && heartRate < 150)
   ↓
5. 保存到全局变量
   my_heartreat = heartRate;
   ↓
6. LVGL 定时器（每 1 秒）调用 my_timer2()
   位置：lib/mylib/my_timer.cpp
   ↓
7. 读取全局变量
   int hr = get_heart_rate();
   ↓
8. 格式化字符串
   sprintf(heart_timer, "%dbpm", hr);
   ↓
9. 更新 UI（已在 LVGL 定时器中，自动有锁保护）
   lv_label_set_text(guider_ui.screen_heart_label_heart, heart_timer);
   ↓
10. lvgl_handler() 刷新屏幕（每 5ms）
    位置：src/main.cpp
    ↓
11. 用户看到心率：75bpm
```

### 2.3 用户交互数据流

```
用户点击按钮
    ↓
触摸芯片检测（CST816T）
    ↓
my_touchpad_read() 读取坐标
    ↓
LVGL 判断点击了哪个控件
    ↓
触发 LV_EVENT_CLICKED 事件
    ↓
调用注册的回调函数
    ↓
执行界面切换或其他操作
    ↓
lvgl_handler 刷新屏幕
    ↓
用户看到界面变化
```

### 2.4 网络数据流

```
WiFi 连接
    ↓
发送 HTTP GET 请求
    ↓
服务器返回 JSON 数据
    ↓
ArduinoJson 解析
    ↓
提取数据（温度、天气等）
    ↓
保存到全局变量
    ↓
LVGL 定时器或回调函数读取
    ↓
更新 UI 显示
```

---

## 3. 任务和定时器协作

### 3.1 FreeRTOS 任务（后台持续运行）

**文件位置**：`src/main.cpp` - setup()

| 任务名称 | 核心 | 优先级 | 栈大小 | 功能 | 频率 |
|---------|------|--------|--------|------|------|
| lvgl_handler | 0 | 2 | 8KB | UI 刷新 | 5ms |
| max30102_task | 1 | 4 | 4KB | 心率血氧 | 1s |
| MPU6050_task | 1 | 2 | 8KB | 运动检测 | 100ms |
| gps_task | 1 | 3 | 4KB | GPS 定位 | 持续 |

### 3.2 LVGL 定时器（UI 层定时任务）

**文件位置**：`lib/mylib/my_timer.cpp`

| 定时器名称 | 功能 | 频率 | 调用函数 |
|-----------|------|------|----------|
| timer_time | 更新时间显示 | 1s | my_timer1() |
| timer_heart_spo2 | 更新心率血氧显示 | 1s | my_timer2() |
| timer_mqtt | MQTT 数据上传 | 3s | my_timer3() |

### 3.3 任务 vs 定时器

**FreeRTOS 任务**：
```cpp
// 特点：独立线程，持续运行
void max30102_task(void* para) {
    初始化();
    while(1) {
        读取传感器();
        处理数据();
        delay(1000);  // 让出 CPU
    }
}

// 优点：
// - 可以执行耗时操作
// - 独立的栈空间
// - 可以阻塞等待

// 缺点：
// - 占用内存多
// - 需要手动管理互斥锁
```

**LVGL 定时器**：
```cpp
// 特点：定时回调，在 lvgl_handler 任务中执行
void my_timer1(lv_timer_t *timer) {
    更新UI();
    // 不能有 delay()
}

// 优点：
// - 自动有互斥锁保护（在 lvgl_handler 中）
// - 不占用额外任务栈
// - 适合 UI 更新

// 缺点：
// - 不能执行耗时操作
// - 不能阻塞
```

### 3.4 为什么这样设计？

```
传感器任务（FreeRTOS）：
- 需要持续读取数据
- 可能需要等待传感器就绪
- 数据处理可能耗时
→ 使用独立任务

UI 更新（LVGL 定时器）：
- 只是读取变量，更新显示
- 操作很快（几毫秒）
- 需要频繁执行
→ 使用 LVGL 定时器
```

---

## 4. 互斥锁的使用

### 4.1 为什么需要互斥锁？

**问题场景**：
```
时刻 1：max30102_task 正在更新心率标签
        lv_label_set_text(label, "75");
        
时刻 2：lvgl_handler 同时在刷新屏幕
        lv_timer_handler();
        
结果：数据混乱，程序崩溃
```

**解决方案**：
```
时刻 1：max30102_task 获取锁
        xSemaphoreTake(xMutex, portMAX_DELAY);
        lv_label_set_text(label, "75");
        xSemaphoreGive(xMutex);
        
时刻 2：lvgl_handler 等待锁
        等待 max30102_task 释放锁
        然后获取锁，刷新屏幕
        
结果：数据正确，程序稳定
```

### 4.2 项目中的锁使用规则

**文件位置**：`src/main.cpp`

```cpp
// 规则 1：只有操作 LVGL UI 时才需要锁
if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
    lv_label_set_text(...);  // 操作 UI
    xSemaphoreGive(xMutex);
}

// 规则 2：LVGL 定时器自动有锁保护
void my_timer1(lv_timer_t *timer) {
    // 这里不需要手动加锁
    // 因为在 lvgl_handler 中已经加锁了
    lv_label_set_text(...);
}

// 规则 3：lvgl_handler 任务持有锁
void lvgl_handler(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
            lv_timer_handler();  // 所有 LVGL 定时器在这里执行
            xSemaphoreGive(xMutex);
        }
        delay(5);
    }
}
```

### 4.3 谁在使用锁？

**获取锁的地方**：
1. `lvgl_handler` 任务（每 5ms）
2. `max30102_task` 任务（更新心率 UI 时）
3. 其他需要更新 UI 的任务

**不需要锁的地方**：
1. LVGL 定时器内部（已在 lvgl_handler 的锁保护中）
2. 只读取传感器数据（不操作 UI）
3. 只修改全局变量（不操作 UI）

---

## 5. 全局变量的作用

### 5.1 为什么使用全局变量？

**任务之间通信的方式**：

```
方式 1：全局变量（项目使用）
优点：简单、快速
缺点：需要注意线程安全

方式 2：队列（Queue）
优点：线程安全、支持缓冲
缺点：稍微复杂

方式 3：事件组（Event Group）
优点：适合事件通知
缺点：不适合传递数据
```

### 5.2 项目中的全局变量

**文件位置**：各个 .cpp 文件

```cpp
// 心率血氧（lib/mylib/my_max30102.cpp）
int32_t my_heartreat = 0;
int32_t my_spo2 = 0;

// 天气数据（lib/mylib/my_signal.cpp）
float my_temperature = 0;
float my_humidity = 0;
String t_day[5];
String t_weather[5];
String t_temp[5];

// GPS 数据（lib/mylib/my_gps.cpp）
double gps_N = 0;  // 纬度
double gps_E = 0;  // 经度

// UI 对象（src/main.cpp）
lv_ui guider_ui;
```

### 5.3 全局变量的访问模式

```
写入（传感器任务）：
max30102_task() {
    my_heartreat = 75;  // 写入
}

读取（UI 更新）：
my_timer2() {
    int hr = get_heart_rate();  // 读取
    lv_label_set_text(..., hr);
}

访问函数（封装）：
int32_t get_heart_rate() {
    return my_heartreat;
}
```

---

## 6. 项目启动流程

### 6.1 setup() 函数执行顺序

**文件位置**：`src/main.cpp` - setup()

```
1. Serial.begin(115200)
   启动串口，可以打印调试信息
   ↓
2. xMutex = xSemaphoreCreateMutex()
   创建互斥锁
   ↓
3. tft.init() + cst816t.begin()
   初始化显示屏和触摸
   ↓
4. connectToWiFi(10)
   连接 WiFi（10 秒超时）
   ↓
5. configTime() + refresh_weather()
   同步时间，获取天气
   ↓
6. my_tft_init() + buzzer_init() + my_mqtt_init()
   初始化其他模块
   ↓
7. lv_init()
   初始化 LVGL
   ↓
8. 配置显示和触摸驱动
   lv_disp_drv_register() + lv_indev_drv_register()
   ↓
9. timerBegin()
   启动硬件定时器（为 LVGL 提供时钟）
   ↓
10. setup_ui() + events_init() + custom_init()
    创建 UI 界面，注册事件
    ↓
11. xTaskCreatePinnedToCore() × 4
    创建 4 个任务
    ↓
12. setup() 完成，进入 loop()
```

### 6.2 运行时的任务调度

```
时间线（毫秒）：
0    5    10   15   20   25   30   ...   1000
│    │    │    │    │    │    │          │
├────┼────┼────┼────┼────┼────┼──────────┤
│lvgl│lvgl│lvgl│lvgl│lvgl│lvgl│lvgl  ... │lvgl  (核心 0)
└────┴────┴────┴────┴────┴────┴──────────┘

├────────────────────────────────────────┤
│  max30102_task (读取心率)               │  (核心 1)
└────────────────────────────────────────┘

├──────┬──────┬──────┬──────┬──────┬─────┤
│MPU   │MPU   │MPU   │MPU   │MPU   │MPU  │  (核心 1)
└──────┴──────┴──────┴──────┴──────┴─────┘

├────────────────────────────────────────┤
│  gps_task (持续读取)                    │  (核心 1)
└────────────────────────────────────────┘
```

