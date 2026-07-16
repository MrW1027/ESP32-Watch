# 第四课：深入理解传感器驱动

## 本课目标

理解传感器是如何工作的，如何通过 I2C 通信读取数据，以及如何处理传感器数据。

---

## 1. 什么是传感器？

### 简单理解

**传感器** = 能感知物理世界的电子元件

**类比**：
```
人的感官 = 传感器
- 眼睛 = 光传感器（摄像头）
- 耳朵 = 声音传感器（麦克风）
- 鼻子 = 气体传感器
- 皮肤 = 温度传感器、压力传感器
```

### 项目中的传感器

```
MAX30102：心率血氧传感器
- 感知：血液中的氧气含量和心跳
- 原理：光电容积脉搏波描记法（PPG）

MPU6050：六轴运动传感器
- 感知：加速度和角速度
- 原理：MEMS（微机电系统）

GPS 模块：定位传感器
- 感知：地理位置
- 原理：接收卫星信号

CST816T：触摸传感器
- 感知：手指触摸位置
- 原理：电容式触摸
```

---

## 2. MAX30102 心率血氧传感器详解

### 2.1 工作原理

**文件位置**：`lib/mylib/my_max30102.cpp`

#### PPG 技术（光电容积脉搏波描记法）

```
┌─────────────────────────────────┐
│  MAX30102 传感器                 │
│                                 │
│  ┌─────┐         ┌─────┐       │
│  │红光 │         │红外 │       │
│  │LED  │         │LED  │       │
│  └──┬──┘         └──┬──┘       │
│     │               │           │
│     ↓               ↓           │
│  ┌─────────────────────┐       │
│  │   你的手指（皮肤）   │       │
│  └─────────────────────┘       │
│     ↑               ↑           │
│     │               │           │
│  ┌──┴──┐         ┌──┴──┐       │
│  │光电 │         │光电 │       │
│  │二极管│         │二极管│       │
│  └─────┘         └─────┘       │
└─────────────────────────────────┘
```

**工作流程**：
```
1. LED 发光 → 照射到皮肤
2. 光线穿过皮肤 → 被血液吸收
3. 反射的光 → 被光电二极管接收
4. 血液流动 → 光强度变化
5. 分析光强度变化 → 计算心率和血氧
```

**为什么用两种光？**
```
红光（660nm）：
- 被含氧血液吸收少
- 被缺氧血液吸收多

红外光（880nm）：
- 被含氧血液吸收多
- 被缺氧血液吸收少

通过比较两种光的吸收率 → 计算血氧饱和度
```

### 2.2 初始化传感器

**文件位置**：`lib/mylib/my_max30102.cpp` - my_max30102_init()

```cpp
void my_max30102_init()
{
    // 第 1 步：初始化 I2C（Wire1）
    Wire1.begin(MAX30102_SDA, MAX30102_SCL);
    
    // 第 2 步：检测传感器是否存在
    if (!particleSensor.begin(Wire1, I2C_SPEED_FAST)) {
        Serial.println(F("MAX30105 was not found. Please check wiring/power."));
        while (1);  // 卡住，等待重启
    }
    
    // 第 3 步：配置传感器参数
    byte ledBrightness = 80;   // LED 亮度（0-255）
    byte sampleAverage = 4;    // 采样平均（1, 2, 4, 8, 16, 32）
    byte ledMode = 2;          // LED 模式（1=红光, 2=红光+红外, 3=红光+红外+绿光）
    byte sampleRate = 100;     // 采样率（50, 100, 200, 400, 800, 1000, 1600, 3200）
    int pulseWidth = 411;      // 脉冲宽度（69, 118, 215, 411）
    int adcRange = 4096;       // ADC 范围（2048, 4096, 8192, 16384）
    
    // 第 4 步：应用配置
    particleSensor.setup(ledBrightness, sampleAverage, ledMode, 
                         sampleRate, pulseWidth, adcRange);
    
    Serial.println(F("MAX30105 set done"));
}
```

#### 参数详解

**ledBrightness（LED 亮度）**：
```
值：0-255
80 = 适中亮度
- 太低：信号弱，检测不准
- 太高：耗电多，可能烫手
```

**sampleAverage（采样平均）**：
```
值：1, 2, 4, 8, 16, 32
4 = 每 4 个样本取平均
作用：减少噪声，提高准确度
```

**ledMode（LED 模式）**：
```
1 = 只用红光（省电，但不能测血氧）
2 = 红光 + 红外（推荐，可测心率和血氧）
3 = 红光 + 红外 + 绿光（更精确，但耗电）
```

**sampleRate（采样率）**：
```
值：50-3200 Hz
100 = 每秒采样 100 次
- 太低：可能漏掉心跳
- 太高：数据量大，处理慢
```

### 2.3 读取传感器数据

**文件位置**：`lib/mylib/my_max30102.cpp` - my_max30102_run()

```cpp
void my_max30102_run()
{
    bufferLength = 100;  // 缓冲区大小（100 个样本）
    
    // ========== 第一阶段：收集初始数据 ==========
    // 读取前 100 个样本（约 4 秒）
    for (byte i = 0; i < bufferLength; i++)
    {
        // 等待新数据
        while (particleSensor.available() == false) {
            particleSensor.check();  // 检查传感器
        }
        
        // 读取红光和红外光数据
        redBuffer[i] = particleSensor.getRed();
        irBuffer[i] = particleSensor.getIR();
        
        // 移动到下一个样本
        particleSensor.nextSample();
        
        // 打印数据（调试用）
        Serial.print(F("red="));
        Serial.print(redBuffer[i], DEC);
        Serial.print(F(", ir="));
        Serial.println(irBuffer[i], DEC);
    }
    
    // ========== 第二阶段：首次计算 ==========
    // 使用前 100 个样本计算心率和血氧
    maxim_heart_rate_and_oxygen_saturation(
        irBuffer, bufferLength, redBuffer,
        &spo2, &validSPO2,
        &heartRate, &validHeartRate
    );
    
    // ========== 第三阶段：持续监测 ==========
    while (1)
    {
        // 步骤 1：移动数据（滑动窗口）
        // 丢弃前 25 个样本，保留后 75 个
        for (byte i = 25; i < 100; i++)
        {
            redBuffer[i - 25] = redBuffer[i];
            irBuffer[i - 25] = irBuffer[i];
        }
        
        // 步骤 2：采集新的 25 个样本
        for (byte i = 75; i < 100; i++)
        {
            while (particleSensor.available() == false) {
                particleSensor.check();
            }
            
            redBuffer[i] = particleSensor.getRed();
            irBuffer[i] = particleSensor.getIR();
            particleSensor.nextSample();
        }
        
        // 步骤 3：重新计算心率和血氧
        maxim_heart_rate_and_oxygen_saturation(
            irBuffer, bufferLength, redBuffer,
            &spo2, &validSPO2,
            &heartRate, &validHeartRate
        );
        
        // 步骤 4：验证数据有效性
        if (validHeartRate == 1 && heartRate > 50 && heartRate < 150) {
            my_heartreat = heartRate;
            Serial.printf("heartRate:%d\n", my_heartreat);
        }
        
        if (validSPO2 == 1 && spo2 > 40 && spo2 <= 100) {
            my_spo2 = spo2;
            Serial.printf("spo2:%d\n", my_spo2);
        }
        
        delay(10);
    }
}
```

#### 滑动窗口技术

```
初始状态（100 个样本）：
[0][1][2]...[74][75][76]...[99]

移动后（保留后 75 个）：
[75][76][77]...[99][  ][  ]...[  ]
 ↓   ↓   ↓     ↓
[0] [1] [2]...[24][  ][  ]...[  ]

采集新的 25 个样本：
[0] [1] [2]...[24][75][76]...[99]
                   ↑   ↑     ↑
                  新数据

这样每次只需要采集 25 个新样本
而不是重新采集 100 个
```

### 2.4 数据验证

**为什么需要验证？**
```
传感器可能返回错误数据：
- 手指没放好
- 手指移动了
- 环境光干扰
- 传感器故障
```

**验证条件**：
```cpp
// 心率验证
if (validHeartRate == 1 && heartRate > 50 && heartRate < 150) {
    // validHeartRate == 1：算法认为数据有效
    // heartRate > 50：正常人心率不会低于 50
    // heartRate < 150：静止时心率不会超过 150
    my_heartreat = heartRate;  // 数据有效，保存
}

// 血氧验证
if (validSPO2 == 1 && spo2 > 40 && spo2 <= 100) {
    // validSPO2 == 1：算法认为数据有效
    // spo2 > 40：血氧不会低于 40%（否则已经昏迷）
    // spo2 <= 100：血氧不会超过 100%
    my_spo2 = spo2;  // 数据有效，保存
}
```

### 2.5 任务函数

**文件位置**：`lib/mylib/my_max30102.cpp` - max30102_task()

```cpp
void max30102_task(void* para)
{
    // 初始化传感器
    my_max30102_init();
    
    // 开始持续监测（无限循环）
    my_max30102_run();
}
```

**任务特点**：
- 绑定到核心 1
- 优先级 4（最高）
- 持续运行，不会退出

---

## 3. I2C 通信协议

### 3.1 什么是 I2C？

**I2C** = Inter-Integrated Circuit（集成电路间通信）

**特点**：
```
只需要 2 根线：
- SDA：数据线（Serial Data）
- SCL：时钟线（Serial Clock）

可以连接多个设备：
- 每个设备有唯一地址
- 主机通过地址选择设备
```

### 3.2 I2C 通信过程

```
主机（ESP32）                从机（MAX30102）
    │                            │
    │──── 开始信号 ────────────→│
    │                            │
    │──── 设备地址 + 写 ────────→│
    │                            │
    │←──── 应答 ─────────────────│
    │                            │
    │──── 寄存器地址 ───────────→│
    │                            │
    │←──── 应答 ─────────────────│
    │                            │
    │──── 重新开始信号 ─────────→│
    │                            │
    │──── 设备地址 + 读 ────────→│
    │                            │
    │←──── 应答 ─────────────────│
    │                            │
    │←──── 数据 ──────────────────│
    │                            │
    │──── 应答 ─────────────────→│
    │                            │
    │──── 停止信号 ─────────────→│
```

### 3.3 ESP32 的 I2C 配置

**ESP32-S3 有两个 I2C 总线**：

```cpp
// I2C 总线 0（Wire）
Wire.begin(SDA_PIN, SCL_PIN);

// I2C 总线 1（Wire1）
Wire1.begin(SDA_PIN, SCL_PIN);
```

**项目中的使用**：
```cpp
// MAX30102 使用 Wire1
#define MAX30102_SDA 48
#define MAX30102_SCL 47
Wire1.begin(MAX30102_SDA, MAX30102_SCL);

// 触摸芯片使用 Wire（默认）
#define I2C_SDA 17
#define I2C_SCL 16
Wire.begin(I2C_SDA, I2C_SCL);
```

### 3.4 I2C 速度

```cpp
I2C_SPEED_STANDARD  // 100 kHz（标准模式）
I2C_SPEED_FAST      // 400 kHz（快速模式，推荐）
I2C_SPEED_FAST_PLUS // 1 MHz（快速增强模式）
```

---

## 4. 数据处理算法

### 4.1 心率计算原理

**基本原理**：
```
心跳 → 血液流动 → 光强度变化 → 周期性波形

例子：
时间：  0s   1s   2s   3s   4s
光强：  ▲    ▲    ▲    ▲    ▲
       心跳  心跳  心跳  心跳  心跳

4 秒内 4 次心跳
心率 = 4 / 4 * 60 = 60 次/分钟
```

**实际算法**（SparkFun 库提供）：
```cpp
maxim_heart_rate_and_oxygen_saturation(
    irBuffer,        // 红外光数据
    bufferLength,    // 数据长度
    redBuffer,       // 红光数据
    &spo2,           // 输出：血氧值
    &validSPO2,      // 输出：血氧是否有效
    &heartRate,      // 输出：心率值
    &validHeartRate  // 输出：心率是否有效
);
```

**算法步骤**：
1. 找到波峰（心跳）
2. 计算波峰间隔
3. 转换为每分钟次数
4. 平滑处理（移动平均）

### 4.2 血氧计算原理

**基本原理**：
```
含氧血液：红光吸收少，红外光吸收多
缺氧血液：红光吸收多，红外光吸收少

比值 R = (红光吸收) / (红外光吸收)

血氧饱和度 SpO2 = f(R)
（f 是一个经验公式）
```

**正常值**：
```
SpO2 > 95%：正常
SpO2 90-95%：轻度缺氧
SpO2 < 90%：需要就医
```

---

## 5. 实战：理解完整流程

### 5.1 从传感器到屏幕显示

```
1. MAX30102 传感器
   ↓ (I2C 通信)
2. ESP32 读取原始数据
   位置：lib/mylib/my_max30102.cpp
   ↓
3. 算法计算心率和血氧
   函数：maxim_heart_rate_and_oxygen_saturation()
   ↓
4. 验证数据有效性
   if (validHeartRate == 1 && ...)
   ↓
5. 保存到全局变量
   my_heartreat = heartRate;
   my_spo2 = spo2;
   ↓
6. 其他任务读取变量
   int32_t hr = get_heart_rate();
   ↓
7. 获取互斥锁
   xSemaphoreTake(xMutex, ...)
   ↓
8. 更新 LVGL UI
   lv_label_set_text_fmt(label, "%d", hr);
   ↓
9. 释放互斥锁
   xSemaphoreGive(xMutex);
   ↓
10. lvgl_handler 刷新屏幕
    位置：src/main.cpp
    ↓
11. 屏幕显示心率数值
```

### 5.2 时间线

```
时间轴：
0s ────────────────────────────────────────→ 10s

传感器任务（核心 1）：
├─ 初始化传感器
├─ 采集 100 个样本（4s）
├─ 首次计算
├─ 采集 25 个新样本（1s）
├─ 重新计算
├─ 更新 UI
└─ 重复...

UI 任务（核心 0）：
├─ 刷新屏幕（每 5ms）
├─ 刷新屏幕
├─ 刷新屏幕
└─ 持续刷新...
```

---

## 6. 调试技巧

### 6.1 查看原始数据

**在 my_max30102_run() 中取消注释**：

```cpp
Serial.print(F("red="));
Serial.print(redBuffer[i], DEC);
Serial.print(F(", ir="));
Serial.println(irBuffer[i], DEC);
```

**输出示例**：
```
red=85234, ir=92341
red=85123, ir=92456
red=85345, ir=92234
...
```

### 6.2 查看计算结果

```cpp
Serial.print(F("HR="));
Serial.print(heartRate, DEC);
Serial.print(F(", HRvalid="));
Serial.print(validHeartRate, DEC);
Serial.print(F(", SPO2="));
Serial.print(spo2, DEC);
Serial.print(F(", SPO2Valid="));
Serial.println(validSPO2, DEC);
```

**输出示例**：
```
HR=75, HRvalid=1, SPO2=98, SPO2Valid=1
HR=76, HRvalid=1, SPO2=98, SPO2Valid=1
HR=0, HRvalid=0, SPO2=0, SPO2Valid=0  ← 手指移开了
```

### 6.3 检查 I2C 连接

```cpp
void setup() {
    Wire1.begin(MAX30102_SDA, MAX30102_SCL);
    
    // 扫描 I2C 设备
    Serial.println("Scanning I2C bus...");
    for (byte addr = 1; addr < 127; addr++) {
        Wire1.beginTransmission(addr);
        if (Wire1.endTransmission() == 0) {
            Serial.printf("Found device at 0x%02X\n", addr);
        }
    }
}
```

**预期输出**：
```
Scanning I2C bus...
Found device at 0x57  ← MAX30102 的地址
```

---

## 7. 常见问题

### 问题 1：传感器检测不到

**症状**：
```
MAX30105 was not found. Please check wiring/power.
```

**原因**：
- I2C 引脚接错
- 传感器没有供电
- I2C 地址错误

**解决**：
1. 检查引脚连接
2. 检查电源（3.3V）
3. 运行 I2C 扫描代码

### 问题 2：心率一直是 0

**症状**：
```
HR=0, HRvalid=0
```

**原因**：
- 手指没放好
- LED 亮度太低
- 手指移动了

**解决**：
1. 手指紧贴传感器
2. 增加 LED 亮度：`ledBrightness = 100`
3. 保持手指静止

### 问题 3：数据跳动很大

**症状**：
```
HR=75, HR=120, HR=60, HR=90...
```

**原因**：
- 手指压力不稳定
- 环境光干扰
- 采样平均太小

**解决**：
1. 保持手指压力稳定
2. 遮挡环境光
3. 增加采样平均：`sampleAverage = 8`

### 问题 4：血氧值不准

**症状**：
```
SPO2=85（实际应该 98）
```

**原因**：
- 手指太冷
- 指甲油干扰
- 传感器位置不对

**解决**：
1. 温暖手指
2. 去除指甲油
3. 调整传感器位置

---

## 8. 性能优化

### 8.1 减少功耗

```cpp
// 降低 LED 亮度
byte ledBrightness = 50;  // 从 80 降到 50

// 降低采样率
byte sampleRate = 50;  // 从 100 降到 50
```

### 8.2 提高准确度

```cpp
// 增加采样平均
byte sampleAverage = 8;  // 从 4 增加到 8

// 增加脉冲宽度
int pulseWidth = 411;  // 使用最大值
```

### 8.3 加快响应速度

```cpp
// 减少缓冲区大小
bufferLength = 50;  // 从 100 减到 50

// 增加采样率
byte sampleRate = 200;  // 从 100 增加到 200
```

---

## 9. 扩展知识

### 9.1 其他传感器对比

| 传感器 | 测量参数 | 接口 | 精度 | 价格 |
|--------|---------|------|------|------|
| MAX30102 | 心率、血氧 | I2C | 高 | 中 |
| MAX30100 | 心率、血氧 | I2C | 中 | 低 |
| MAX30105 | 心率、血氧、温度 | I2C | 高 | 高 |
| PPG 传感器 | 心率 | 模拟 | 低 | 低 |

### 9.2 医疗级 vs 消费级

**消费级（本项目）**：
```
优点：
- 便宜
- 易用
- 适合日常监测

缺点：
- 精度一般（±2-3 bpm）
- 不能用于医疗诊断
```

**医疗级**：
```
优点：
- 精度高（±1 bpm）
- 经过认证
- 可用于诊断

缺点：
- 昂贵
- 复杂
- 需要专业知识
```

---

## 10. 总结

### MAX30102 工作流程

```
1. 初始化传感器
   ↓
2. 配置参数（LED 亮度、采样率等）
   ↓
3. 采集 100 个样本（初始化）
   ↓
4. 计算心率和血氧
   ↓
5. 持续监测：
   - 丢弃旧数据
   - 采集新数据
   - 重新计算
   - 验证有效性
   - 更新显示
   ↓
6. 重复步骤 5
```

### 关键技术点

1. **I2C 通信**：2 根线连接多个设备
2. **PPG 技术**：光电法测量心率血氧
3. **滑动窗口**：高效的数据更新方式
4. **数据验证**：确保数据合理性
5. **多任务**：传感器任务独立运行

### 学到的概念

- ✅ 传感器的工作原理
- ✅ I2C 通信协议
- ✅ 数据采集和处理
- ✅ 滑动窗口算法
- ✅ 数据验证技巧

---

## 下一课预告

**第五课：网络通信和 API 调用**

我们会学习：
- HTTP 协议
- JSON 数据解析
- 天气 API 调用
- 百度 AI API 使用
- MQTT 物联网通信

准备好了告诉我！
