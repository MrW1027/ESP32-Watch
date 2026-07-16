# 第二课：深入理解 setup() 函数

## 本课目标

理解 setup() 函数中每一行代码的作用，就像理解一个工厂的开业准备流程。

---

## 1. setup() 函数的整体结构

让我们先看看 setup() 做了哪些事：

```cpp
void setup() {
    // 第一部分：基础设施
    Serial.begin(115200);              // 启动串口
    xMutex = xSemaphoreCreateMutex();  // 创建互斥锁
    
    // 第二部分：硬件初始化
    tft.init();                        // 初始化显示屏
    cst816t.begin();                   // 初始化触摸
    
    // 第三部分：网络连接
    connectToWiFi(connectTimeOut_s);   // 连接 WiFi
    configTime(...);                   // 同步时间
    refresh_weather();                 // 获取天气
    
    // 第四部分：其他模块
    my_tft_init();                     // TFT 相关
    buzzer_init();                     // 蜂鸣器
    my_mqtt_init();                    // MQTT
    
    // 第五部分：图形界面
    lv_init();                         // 初始化 LVGL
    // ... 配置显示和触摸
    
    // 第六部分：定时器
    tim1 = timerBegin(...);            // 硬件定时器
    
    // 第七部分：创建界面
    setup_ui(&guider_ui);              // 创建 UI
    
    // 第八部分：创建任务
    xTaskCreatePinnedToCore(...);      // 创建多个任务
}
```

---

## 2. 第一部分：基础设施（地基）

### 2.1 启动串口通信

**文件位置**：`src/main.cpp` - setup() 函数

```cpp
Serial.begin(115200);
```

**这行代码在做什么？**
- 启动 ESP32 的串口（UART）
- 设置通信速度为 115200 bps（每秒传输 115200 位）

**为什么需要串口？**
- 用来调试：打印信息到电脑
- 就像给设备装了一个"嘴巴"，可以说话

**类比**：
```
你买了一个新手机
第一步：开机（Serial.begin）
现在手机可以显示信息了
```

**实际效果**：
```cpp
Serial.println("Hello");  // 电脑上会显示：Hello
```

### 2.2 创建互斥锁

**文件位置**：`src/main.cpp` - setup() 函数

```cpp
xMutex = xSemaphoreCreateMutex();
```

**这行代码在做什么？**
- 创建一个"锁"
- 这个锁用来保护共享资源（LVGL UI）

**为什么需要锁？**

想象一个场景：
```
任务 A 正在更新屏幕显示心率：75
任务 B 同时想更新屏幕显示时间：12:30

如果没有锁：
- 两个任务同时操作屏幕
- 数据可能混乱：7:530 或 125:75
- 程序可能崩溃

有了锁：
- 任务 A 先拿到锁，更新心率
- 任务 B 等待
- 任务 A 完成，释放锁
- 任务 B 拿到锁，更新时间
- 屏幕显示正常
```

**类比**：
```
公共厕所的门锁
- 有人在用时，门是锁着的
- 其他人必须等待
- 用完后，开锁，下一个人进入
```

---

## 3. 第二部分：硬件初始化

### 3.1 初始化显示屏

```cpp
tft.init();
Serial.println("Setup tft done");
```

**tft.init() 在做什么？**
1. 配置 SPI 通信（ESP32 和屏幕之间的通信方式）
2. 发送初始化命令到屏幕芯片
3. 设置屏幕分辨率、颜色模式
4. 清空屏幕

**SPI 是什么？**
- 一种通信协议（就像两个人说话的规则）
- 需要 4 根线：
  - MOSI：主机发送数据
  - MISO：主机接收数据
  - SCK：时钟信号
  - CS：片选信号

**类比**：
```
你买了一台新电视
tft.init() = 插电源 + 按开机键 + 调整设置
现在电视可以显示画面了
```

### 3.2 初始化触摸芯片

```cpp
cst816t.begin();
Serial.println("Setup cst816t done");
```

**cst816t.begin() 在做什么？**
1. 配置 I2C 通信（另一种通信协议）
2. 复位触摸芯片（重启它）
3. 读取芯片 ID（确认连接正常）
4. 配置触摸参数

**I2C 是什么？**
- 一种通信协议，只需要 2 根线：
  - SDA：数据线
  - SCL：时钟线
- 可以连接多个设备（每个设备有唯一地址）

**类比**：
```
给手机装触摸屏
cst816t.begin() = 连接触摸屏 + 校准
现在可以用手指操作了
```

---

## 4. 第三部分：网络连接

### 4.1 连接 WiFi

```cpp
connectToWiFi(connectTimeOut_s);
```

**这个函数在做什么？**
1. 读取 WiFi 名称（SSID）和密码
2. 尝试连接到 WiFi
3. 如果 10 秒内连不上，放弃

**WiFi 配置在哪里？**
- 在 `lib/mylib/WiFiUser.cpp` 文件中
- 需要修改成你自己的 WiFi 信息

**类比**：
```
手机连接 WiFi
1. 打开 WiFi 设置
2. 选择网络名称
3. 输入密码
4. 连接成功
```

### 4.2 同步网络时间

```cpp
configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
```

**参数解释**：
- `8 * 3600`：时区偏移（北京时间 = UTC + 8 小时）
- `0`：夏令时偏移（中国不用夏令时）
- `"pool.ntp.org"`：时间服务器地址

**这行代码在做什么？**
1. 连接到互联网时间服务器
2. 获取准确的当前时间
3. 设置 ESP32 的内部时钟

**为什么需要同步时间？**
- ESP32 没有电池，断电后时间会丢失
- 需要从网络获取准确时间
- 用于显示时钟、记录日志等

**类比**：
```
你的手表时间不准
打电话问报时台："现在几点？"
调整手表时间
```

### 4.3 获取天气数据

```cpp
refresh_weather();
```

**这个函数在做什么？**
1. 发送 HTTP 请求到天气 API
2. 接收 JSON 格式的天气数据
3. 解析数据（日期、天气、温度）
4. 存储到全局变量

**为什么在 setup() 中调用？**
- 提前准备好数据
- 避免在任务中首次调用时超时

---

## 5. 第四部分：其他模块初始化

```cpp
my_tft_init();      // TFT 相关初始化
buzzer_init();      // 蜂鸣器初始化
my_mqtt_init();     // MQTT 初始化
reconnect();        // MQTT 连接
```

这些都是自定义的初始化函数，分别初始化不同的功能模块。

---

## 6. 第五部分：图形界面（LVGL）

### 6.1 初始化 LVGL

**文件位置**：`src/main.cpp` - setup() 函数

```cpp
lv_init();
Serial.println("Setup lv_init done");
```

**LVGL 是什么？**
- Light and Versatile Graphics Library
- 轻量级图形库
- 用来创建按钮、标签、图片等 UI 元素

### 6.2 创建显示缓冲区

```cpp
static lv_color_t buf[screenWidth * screenHeight / 2];
lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 2);
```

**这在做什么？**
- 创建一个内存区域（缓冲区）
- 大小是半个屏幕
- LVGL 先在这里画图，然后一次性刷新到屏幕

**为什么需要缓冲区？**

想象画画：
```
方法 1（没有缓冲区）：
- 直接在墙上画
- 画一笔，墙上出现一笔
- 会看到绘制过程，闪烁

方法 2（有缓冲区）：
- 先在纸上画完整幅画
- 然后贴到墙上
- 一次性显示，不闪烁
```

**内存计算**：
```
屏幕：240 x 280 = 67200 像素
每像素 2 字节（RGB565 颜色）
全屏：67200 x 2 = 134400 字节 ≈ 131 KB
半屏：67200 字节 ≈ 65.6 KB（节省内存）
```

### 6.3 注册显示驱动

```cpp
static lv_disp_drv_t disp_drv;
lv_disp_drv_init(&disp_drv);
disp_drv.hor_res = screenWidth;
disp_drv.ver_res = screenHeight;
disp_drv.flush_cb = my_disp_flush;
disp_drv.draw_buf = &draw_buf;
lv_disp_drv_register(&disp_drv);
```

**这在做什么？**
- 告诉 LVGL 屏幕的尺寸
- 告诉 LVGL 如何刷新屏幕（调用 my_disp_flush 函数）
- 绑定缓冲区

**my_disp_flush 函数的作用**：
```cpp
void my_disp_flush(...) {
    // LVGL 说："我画好了，帮我显示到屏幕"
    // 这个函数把缓冲区的数据发送到 TFT 屏幕
}
```

### 6.4 注册触摸驱动

```cpp
static lv_indev_drv_t indev_drv;
lv_indev_drv_init(&indev_drv);
indev_drv.type = LV_INDEV_TYPE_POINTER;
indev_drv.read_cb = my_touchpad_read;
lv_indev_drv_register(&indev_drv);
```

**这在做什么？**
- 告诉 LVGL 输入设备类型是"指针"（触摸）
- 告诉 LVGL 如何读取触摸（调用 my_touchpad_read 函数）

**my_touchpad_read 函数的作用**：
```cpp
void my_touchpad_read(...) {
    // LVGL 问："有人触摸屏幕吗？在哪里？"
    // 这个函数读取触摸坐标，告诉 LVGL
}
```

---

## 7. 小结：setup() 的执行顺序

```
1. 启动串口（可以打印调试信息了）
   ↓
2. 创建互斥锁（为多任务做准备）
   ↓
3. 初始化显示屏（可以显示画面了）
   ↓
4. 初始化触摸（可以触摸操作了）
   ↓
5. 连接 WiFi（可以上网了）
   ↓
6. 同步时间（知道现在几点了）
   ↓
7. 获取天气（知道天气情况了）
   ↓
8. 初始化 LVGL（可以画 UI 了）
   ↓
9. 配置定时器（为 LVGL 提供时钟）
   ↓
10. 创建 UI 界面（显示主界面）
   ↓
11. 创建任务（启动后台工作）
   ↓
12. setup() 完成，进入 loop()
```

---

## 8. 思考题

1. **为什么要先初始化串口？**
   <details>
   <summary>答案</summary>
   因为后面的初始化步骤需要打印调试信息，如果串口没启动，就看不到信息了。
   </details>

2. **为什么要先连接 WiFi，再获取天气？**
   <details>
   <summary>答案</summary>
   因为获取天气需要通过网络访问 API，必须先连上 WiFi。
   </details>

3. **如果 setup() 中某一步失败了会怎样？**
   <details>
   <summary>答案</summary>
   后续依赖这一步的功能可能无法正常工作。比如 WiFi 连接失败，就无法获取天气和同步时间。
   </details>

---

## 下一课预告

**第三课：理解定时器和 UI 刷新机制**

我们会学习：
- 硬件定时器是如何工作的
- LVGL 如何刷新屏幕
- my_disp_flush 和 my_touchpad_read 的详细工作原理

准备好了告诉我！
