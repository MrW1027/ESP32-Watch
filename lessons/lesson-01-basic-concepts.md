# 第一课：基础概念 - 理解程序的骨架

## 1. Arduino 程序的基本结构

每个 Arduino 程序都有两个必须的函数：

```cpp
void setup() {
    // 这里的代码只运行一次
    // 在设备开机时执行
}

void loop() {
    // 这里的代码会一直重复运行
    // 像一个无限循环
}
```

### 用生活例子理解：

想象你早上起床：

```cpp
void setup() {
    // 起床后的准备工作（只做一次）
    穿衣服();
    刷牙();
    吃早餐();
}

void loop() {
    // 一天中重复做的事
    工作();
    休息();
    // 然后又工作、休息...一直循环
}
```

---

## 2. 看看我们项目的 main.cpp

打开 `src/main.cpp`，你会看到：

```cpp
void setup() {
    Serial.begin(115200);
    // ... 很多初始化代码
}

void loop() {
    delay(10);
    // 几乎是空的！
}
```

### 为什么 loop() 几乎是空的？

因为这个项目使用了 **FreeRTOS**（一个操作系统），它会创建多个"任务"来工作。

**类比**：
- 传统方式：你一个人做所有事（loop 里写所有代码）
- FreeRTOS 方式：你雇了几个员工，每个人负责一件事（创建多个任务）

---

## 3. 什么是头文件（#include）？

在代码开头，你会看到很多 `#include`：

```cpp
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
```

### 理解头文件：

**类比**：头文件就像工具箱

```cpp
#include <Arduino.h>      // 基础工具箱（螺丝刀、扳手）
#include <TFT_eSPI.h>     // 显示屏工具箱
#include <lvgl.h>         // 图形界面工具箱
#include "WiFiUser.h"     // 自己做的 WiFi 工具箱
```

- 用 `< >` 包围的：系统提供的库
- 用 `" "` 包围的：自己写的文件

---

## 4. 什么是变量？

变量就是用来存储数据的"盒子"。

### 看代码中的变量：

```cpp
int connectTimeOut_s = 10;
```

**解读**：
- `int`：整数类型（integer）
- `connectTimeOut_s`：变量名（连接超时秒数）
- `= 10`：赋值为 10
- `;`：语句结束符号

**类比**：
```
int 年龄 = 25;           // 一个装整数的盒子
float 身高 = 1.75;       // 一个装小数的盒子
String 名字 = "张三";    // 一个装文字的盒子
```

### 更多变量例子：

```cpp
static const uint32_t screenWidth = 240;   // 屏幕宽度
static const uint32_t screenHeight = 280;  // 屏幕高度
```

**关键字解释**：
- `static`：这个变量只在这个文件中使用
- `const`：常量，不能修改
- `uint32_t`：无符号 32 位整数（只能是正数）

---

## 5. 什么是对象？

对象是把数据和功能打包在一起的"东西"。

### 看代码中的对象：

```cpp
TFT_eSPI tft = TFT_eSPI();
```

**解读**：
- `TFT_eSPI`：类型（就像"汽车"这个类别）
- `tft`：对象名（就像"我的汽车"）
- `= TFT_eSPI()`：创建这个对象

**类比**：
```
汽车 我的车 = 汽车();
我的车.启动();
我的车.加速();
我的车.刹车();
```

在代码中：
```cpp
TFT_eSPI tft = TFT_eSPI();
tft.init();           // 初始化显示屏
tft.fillScreen(0);    // 填充屏幕颜色
```

---

## 6. 什么是函数？

函数是一段可以重复使用的代码。

### 函数的结构：

```cpp
返回类型 函数名(参数) {
    // 函数体
    return 返回值;
}
```

### 例子：

```cpp
// 无返回值的函数
void sayHello() {
    Serial.println("Hello!");
}

// 有返回值的函数
int add(int a, int b) {
    return a + b;
}

// 使用函数
sayHello();           // 输出：Hello!
int result = add(3, 5);  // result = 8
```

### 项目中的函数：

```cpp
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    // 这个函数负责把图像刷新到屏幕
}
```

---

## 7. 什么是 #define？

`#define` 用来定义常量或宏。

```cpp
#define I2C_SDA 17
#define I2C_SCL 16
```

**作用**：
- 在编译时，所有的 `I2C_SDA` 都会被替换成 `17`
- 方便修改：如果要改引脚，只需改一处

**类比**：
```cpp
#define 上班时间 9
#define 下班时间 18

if (现在时间 >= 上班时间 && 现在时间 < 下班时间) {
    工作();
}
```

---

## 8. 理解代码的层次结构

我们的项目代码分为几层：

```
┌─────────────────────────────────┐
│   用户界面（LVGL）               │  ← 你看到的屏幕
├─────────────────────────────────┤
│   应用层（任务函数）             │  ← 业务逻辑
├─────────────────────────────────┤
│   驱动层（传感器、显示驱动）     │  ← 硬件控制
├─────────────────────────────────┤
│   操作系统（FreeRTOS）           │  ← 任务调度
├─────────────────────────────────┤
│   硬件层（ESP32-S3）             │  ← 芯片
└─────────────────────────────────┘
```

---

## 9. 串口输出 - 你的调试好帮手

```cpp
Serial.begin(115200);           // 启动串口，波特率 115200
Serial.println("Hello World");  // 打印一行文字
Serial.printf("数字: %d\n", 123); // 格式化打印
```

**如何查看输出？**
1. 用 USB 线连接 ESP32 到电脑
2. 打开串口监视器（PlatformIO: Monitor）
3. 就能看到设备打印的信息

**类比**：
- 串口就像设备的"嘴巴"
- 它可以告诉你程序运行到哪里了
- 可以输出变量的值，帮助你理解程序

---

## 10. 实战练习：读懂 setup() 的前几行

让我们一起读懂 setup() 函数的开头：

**文件位置**：`src/main.cpp`

```cpp
void setup() {
    // 第 1 行：启动串口通信
    Serial.begin(115200);
    
    // 第 2 行：创建一个互斥锁（后面会详细讲）
    xMutex = xSemaphoreCreateMutex();
    
    // 第 3 行：初始化 TFT 显示屏
    tft.init();
    
    // 第 4 行：打印调试信息
    Serial.println("Setup tft done");
    
    // 第 5 行：初始化触摸芯片
    cst816t.begin();
    
    // 第 6 行：打印调试信息
    Serial.println("Setup cst816t done");
}
```

**执行流程**：
```
1. 开机
2. 启动串口（现在可以打印信息了）
3. 创建互斥锁（为多任务做准备）
4. 初始化显示屏
5. 打印 "Setup tft done"（告诉你显示屏好了）
6. 初始化触摸芯片
7. 打印 "Setup cst816t done"（告诉你触摸好了）
8. ... 继续其他初始化
```

---

## 11. 小测验（检验理解）

看看你能否理解这段代码：

```cpp
#define LED_PIN 2

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("LED 初始化完成");
}

void loop() {
    digitalWrite(LED_PIN, HIGH);  // 点亮 LED
    delay(1000);                  // 等待 1 秒
    digitalWrite(LED_PIN, LOW);   // 熄灭 LED
    delay(1000);                  // 等待 1 秒
}
```

**问题**：
1. LED 连接在哪个引脚？
2. LED 会怎样闪烁？
3. 闪烁的频率是多少？

**答案**：
1. GPIO 2
2. 亮 1 秒，灭 1 秒，循环
3. 每 2 秒一个周期（0.5 Hz）

---

## 12. 总结

今天我们学习了：

✅ Arduino 程序的基本结构（setup 和 loop）
✅ 头文件的作用（#include）
✅ 变量的概念（存储数据的盒子）
✅ 对象的概念（数据+功能的组合）
✅ 函数的概念（可重复使用的代码）
✅ #define 的作用（定义常量）
✅ 串口输出（调试工具）
✅ 代码的层次结构

---

## 下一课预告

**第二课：深入理解 setup() 函数**

我们会详细分析 setup() 中的每一行代码：
- 为什么要初始化这些硬件？
- 初始化的顺序为什么这样安排？
- 每个初始化步骤在做什么？

---

## 作业（可选）

1. 打开 `src/main.cpp`，找到 setup() 函数
2. 数一数 setup() 中调用了多少个初始化函数
3. 找到所有的 `Serial.println()` 语句，理解它们的作用

准备好了就告诉我，我们开始第二课！
