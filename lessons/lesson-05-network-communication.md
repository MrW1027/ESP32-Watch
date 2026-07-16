# 第五课：网络通信和 API 调用

## 本课目标

理解如何通过网络获取数据，包括 HTTP 请求、JSON 解析、API 调用等。

---

## 1. 网络通信基础

### 1.1 什么是 HTTP？

**HTTP** = HyperText Transfer Protocol（超文本传输协议）

**简单理解**：
```
HTTP = 浏览器和服务器之间的"对话规则"

你：给我看天气（HTTP 请求）
服务器：这是天气数据（HTTP 响应）
```

### 1.2 HTTP 请求方法

```
GET：获取数据（最常用）
- 例：获取天气、获取新闻

POST：提交数据
- 例：登录、发送表单

PUT：更新数据
DELETE：删除数据
```

**项目中使用**：只用了 `GET` 方法

---

## 2. 天气 API 调用详解

### 2.1 完整代码分析

**文件位置**：`lib/mylib/my_signal.cpp` - refresh_weather()

```cpp
void refresh_weather(void)
{
    // 第 1 步：创建 HTTP 客户端对象
    HTTPClient http;
    
    // 第 2 步：构建 API 请求 URL
    String url = (String) "http://apis.juhe.cn/simpleWeather/query" 
                 + "?city=成都" 
                 + "&key=4ab559ae9fe53e807c8606b5005c1592";
    
    // 第 3 步：开始 HTTP 连接
    http.begin(url);
    
    // 第 4 步：发送 GET 请求
    int http_code = http.GET();
    
    // 第 5 步：获取服务器响应
    String reponse = http.getString();
    
    // 第 6 步：创建 JSON 解析器
    DynamicJsonDocument doc(1024);
    
    // 第 7 步：解析 JSON 数据
    deserializeJson(doc, reponse);
    
    // 第 8 步：提取实时温度和湿度
    my_temperature = doc["result"]["realtime"]["temperature"].as<String>().toFloat();
    my_humidity = doc["result"]["realtime"]["humidity"].as<String>().toFloat();
    
    // 第 9 步：循环提取未来 5 天的天气
    for (int i = 0; i < 5; i++)
    {
        String date_str = doc["result"]["future"][i]["date"].as<String>();
        t_day[i] = date_str.substring(5);  // "2025-03-16" → "03-16"
        
        t_weather[i] = doc["result"]["future"][i]["weather"].as<String>();
        t_temp[i] = doc["result"]["future"][i]["temperature"].as<String>();
    }
}
```

---

### 2.2 URL 详解

```cpp
String url = "http://apis.juhe.cn/simpleWeather/query?city=成都&key=xxx";
```

**URL 组成部分**：

```
http://apis.juhe.cn/simpleWeather/query?city=成都&key=xxx
│      │                │               │
│      │                │               └─ 查询参数
│      │                └─ API 路径
│      └─ 域名
└─ 协议
```

**详细分解**：

1. **协议**：`http://`
   - HTTP 协议（不加密）
   - HTTPS 协议（加密）：`https://`

2. **域名**：`apis.juhe.cn`
   - 聚合数据的 API 服务器

3. **路径**：`/simpleWeather/query`
   - 简单天气查询接口

4. **查询参数**：`?city=成都&key=xxx`
   - `?`：参数开始
   - `city=成都`：查询城市
   - `&`：参数分隔符
   - `key=xxx`：API 密钥（身份验证）

---

### 2.3 HTTP 响应码

```cpp
int http_code = http.GET();
```

**常见响应码**：

```
200：成功
- 请求成功，返回数据

400：客户端错误
- 请求格式错误

401：未授权
- API 密钥错误或过期

404：未找到
- API 地址错误

500：服务器错误
- 服务器内部错误
```

**应该添加错误处理**：
```cpp
int http_code = http.GET();

if (http_code == 200) {
    // 成功，处理数据
    String response = http.getString();
} else {
    // 失败，打印错误
    Serial.printf("HTTP 错误: %d\n", http_code);
}
```

---

### 2.4 JSON 数据格式

**服务器返回的 JSON 数据**：

```json
{
  "reason": "查询成功",
  "result": {
    "city": "成都",
    "realtime": {
      "temperature": "23.7",
      "humidity": "56.2",
      "info": "晴"
    },
    "future": [
      {
        "date": "2025-03-16",
        "temperature": "15-20℃",
        "weather": "晴"
      },
      {
        "date": "2025-03-17",
        "temperature": "16-22℃",
        "weather": "多云"
      }
      // ... 共 5 天
    ]
  }
}
```

---

### 2.5 JSON 解析详解

#### 步骤 1：创建 JSON 文档

```cpp
DynamicJsonDocument doc(1024);
```

**参数说明**：
- `1024`：分配 1024 字节内存
- 如果 JSON 数据太大，需要增加这个值

**如何确定大小？**
```cpp
// 方法 1：估算
// 简单数据：512 字节
// 中等数据：1024 字节
// 复杂数据：2048 字节

// 方法 2：使用 ArduinoJson 助手
// https://arduinojson.org/v6/assistant/
```

#### 步骤 2：解析 JSON 字符串

```cpp
deserializeJson(doc, reponse);
```

**这行代码做了什么？**
- 把 JSON 字符串转换成结构化数据
- 现在可以像访问对象一样访问数据

#### 步骤 3：提取数据

```cpp
// 访问嵌套数据
doc["result"]["realtime"]["temperature"]
│    │        │            │
│    │        │            └─ 第三层：temperature
│    │        └─ 第二层：realtime
│    └─ 第一层：result
└─ 根对象
```

**类型转换**：
```cpp
// 转换为字符串
.as<String>()

// 转换为整数
.as<int>()

// 转换为浮点数
.as<float>()

// 字符串转浮点数
.as<String>().toFloat()
```

#### 步骤 4：访问数组

```cpp
for (int i = 0; i < 5; i++)
{
    // 访问数组元素
    doc["result"]["future"][i]["date"]
    //                      ↑
    //                   数组索引
}
```

**数组访问示例**：
```
future[0] → 第 1 天天气
future[1] → 第 2 天天气
future[2] → 第 3 天天气
future[3] → 第 4 天天气
future[4] → 第 5 天天气
```

---

## 3. 时间 API 调用

### 3.1 完整代码

**文件位置**：`lib/mylib/my_signal.cpp` - refresh_time()

```cpp
void refresh_time(void)
{
    // HTTP 请求（同天气 API）
    HTTPClient http;
    String url = "http://apis.juhe.cn/fapigx/worldtime/query?city=成都&key=xxx";
    http.begin(url);
    int http_code = http.GET();
    String reponse = http.getString();
    
    // JSON 解析
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, reponse);
    
    // 提取时间字符串
    String t_time = doc["result"]["strtime"].as<String>();
    // 例如："2025-03-16 12:30:45"
    
    // 字符串处理
    char str[30];
    strcpy(str, t_time.c_str());  // String → char[]
    
    // 查找第一个 '-' 的位置
    char *date = strchr(str, '-');
    if (date) {
        date++;  // 跳过 '-'
    } else {
        date = str;
    }
    
    // 分割字符串
    time_day = strtok(date, " ");   // "03-16"
    time_hour = strtok(NULL, " ");  // "12:30:45"
    
    Serial.println(time_day);
    Serial.println(time_hour);
}
```

### 3.2 字符串处理详解

#### strcpy - 字符串复制

```cpp
char str[30];
strcpy(str, t_time.c_str());
```

**作用**：
- 把 String 对象转换为 C 字符串（char 数组）
- 因为后续的 strtok 需要 char 数组

#### strchr - 查找字符

```cpp
char *date = strchr(str, '-');
```

**作用**：
- 在字符串中查找第一个 '-' 字符
- 返回指向该字符的指针

**例子**：
```
str = "2025-03-16 12:30:45"
       ↑
       找到第一个 '-'
date 指向这里
```

#### strtok - 分割字符串

```cpp
time_day = strtok(date, " ");   // 第一次调用
time_hour = strtok(NULL, " ");  // 第二次调用
```

**作用**：
- 按分隔符（空格）分割字符串

**工作流程**：
```
原始："-03-16 12:30:45"
      ↓ strtok(date, " ")
第一次："-03-16\0 12:30:45"
        └─ 返回 "-03-16"

      ↓ strtok(NULL, " ")
第二次："-03-16\0 12:30:45\0"
                 └─ 返回 "12:30:45"
```

---

## 4. WiFi 连接

### 4.1 WiFi 连接代码

**文件位置**：`lib/mylib/WiFiUser.cpp`

```cpp
void connectToWiFi(int timeout_s)
{
    // 开始连接
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to WiFi");
    
    // 等待连接，最多 timeout_s 秒
    int count = 0;
    while (WiFi.status() != WL_CONNECTED && count < timeout_s * 10) {
        delay(100);
        Serial.print(".");
        count++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWiFi Connection Failed!");
    }
}
```

### 4.2 WiFi 状态

```cpp
WiFi.status()
```

**返回值**：
```
WL_CONNECTED：已连接
WL_NO_SHIELD：没有 WiFi 模块
WL_IDLE_STATUS：空闲
WL_NO_SSID_AVAIL：找不到 SSID
WL_SCAN_COMPLETED：扫描完成
WL_CONNECT_FAILED：连接失败
WL_CONNECTION_LOST：连接丢失
WL_DISCONNECTED：未连接
```

### 4.3 获取 WiFi 信息

```cpp
// IP 地址
WiFi.localIP()

// MAC 地址
WiFi.macAddress()

// 信号强度
WiFi.RSSI()

// SSID
WiFi.SSID()
```

---

## 5. 完整的网络请求流程

### 5.1 流程图

```
1. 连接 WiFi
   WiFi.begin(ssid, password)
   ↓
2. 等待连接成功
   while (WiFi.status() != WL_CONNECTED)
   ↓
3. 创建 HTTP 客户端
   HTTPClient http;
   ↓
4. 设置 URL
   http.begin(url);
   ↓
5. 发送 GET 请求
   int code = http.GET();
   ↓
6. 检查响应码
   if (code == 200)
   ↓
7. 获取响应数据
   String response = http.getString();
   ↓
8. 解析 JSON
   deserializeJson(doc, response);
   ↓
9. 提取数据
   doc["result"]["data"]
   ↓
10. 使用数据
    更新 UI、保存变量等
```

### 5.2 完整示例（带错误处理）

```cpp
void getWeatherWithErrorHandling()
{
    // 检查 WiFi 连接
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi 未连接");
        return;
    }
    
    // 创建 HTTP 客户端
    HTTPClient http;
    String url = "http://apis.juhe.cn/simpleWeather/query?city=成都&key=xxx";
    
    // 开始连接
    http.begin(url);
    
    // 设置超时（可选）
    http.setTimeout(5000);  // 5 秒超时
    
    // 发送请求
    int httpCode = http.GET();
    
    // 检查响应码
    if (httpCode == 200) {
        // 成功
        String response = http.getString();
        
        // 解析 JSON
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response);
        
        if (error) {
            Serial.print("JSON 解析失败: ");
            Serial.println(error.c_str());
            return;
        }
        
        // 提取数据
        if (doc.containsKey("result")) {
            float temp = doc["result"]["realtime"]["temperature"].as<float>();
            Serial.printf("温度: %.1f℃\n", temp);
        } else {
            Serial.println("数据格式错误");
        }
        
    } else if (httpCode > 0) {
        // HTTP 错误
        Serial.printf("HTTP 错误: %d\n", httpCode);
    } else {
        // 连接错误
        Serial.printf("连接失败: %s\n", http.errorToString(httpCode).c_str());
    }
    
    // 关闭连接
    http.end();
}
```

---

## 6. 常见问题

### 问题 1：WiFi 连接失败

**症状**：
```
Connecting to WiFi.........
WiFi Connection Failed!
```

**原因**：
- SSID 或密码错误
- WiFi 信号太弱
- 路由器设置问题

**解决**：
```cpp
// 1. 检查 SSID 和密码
const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

// 2. 扫描可用 WiFi
int n = WiFi.scanNetworks();
for (int i = 0; i < n; i++) {
    Serial.println(WiFi.SSID(i));
}

// 3. 增加超时时间
connectToWiFi(30);  // 30 秒
```

### 问题 2：HTTP 请求超时

**症状**：
```
HTTP 错误: -1
```

**原因**：
- 网络不稳定
- API 服务器响应慢
- URL 错误

**解决**：
```cpp
// 增加超时时间
http.setTimeout(10000);  // 10 秒

// 添加重试机制
for (int retry = 0; retry < 3; retry++) {
    int code = http.GET();
    if (code == 200) break;
    delay(1000);
}
```

### 问题 3：JSON 解析失败

**症状**：
```
JSON 解析失败: InvalidInput
```

**原因**：
- JSON 数据格式错误
- 内存不足
- 响应不是 JSON 格式

**解决**：
```cpp
// 1. 打印原始响应
Serial.println(response);

// 2. 增加内存
DynamicJsonDocument doc(2048);  // 从 1024 增加到 2048

// 3. 检查解析错误
DeserializationError error = deserializeJson(doc, response);
if (error) {
    Serial.println(error.c_str());
}
```

### 问题 4：API 密钥失效

**症状**：
```
HTTP 错误: 401
```

**原因**：
- API 密钥错误
- API 密钥过期
- API 调用次数超限

**解决**：
- 检查 API 密钥是否正确
- 申请新的 API 密钥
- 查看 API 文档的限制说明

---

## 7. 性能优化

### 7.1 减少内存使用

```cpp
// 使用 StaticJsonDocument（栈内存）
StaticJsonDocument<1024> doc;

// 而不是 DynamicJsonDocument（堆内存）
// DynamicJsonDocument doc(1024);
```

### 7.2 复用 HTTP 客户端

```cpp
// 全局变量
HTTPClient http;

void getWeather() {
    http.begin(url);
    // ...
    http.end();
}

void getTime() {
    http.begin(url);
    // ...
    http.end();
}
```

### 7.3 使用 HTTPS（加密）

```cpp
// 包含证书
#include <WiFiClientSecure.h>

WiFiClientSecure client;
client.setInsecure();  // 跳过证书验证（不推荐）

// 或者设置证书
client.setCACert(root_ca);

HTTPClient https;
https.begin(client, "https://api.example.com");
```

---

## 8. 实战：添加新的 API

### 示例：获取 IP 地址信息

```cpp
void getIPInfo()
{
    HTTPClient http;
    String url = "http://ip-api.com/json/";
    
    http.begin(url);
    int code = http.GET();
    
    if (code == 200) {
        String response = http.getString();
        
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, response);
        
        String country = doc["country"].as<String>();
        String city = doc["city"].as<String>();
        String ip = doc["query"].as<String>();
        
        Serial.printf("国家: %s\n", country.c_str());
        Serial.printf("城市: %s\n", city.c_str());
        Serial.printf("IP: %s\n", ip.c_str());
    }
    
    http.end();
}
```

---

## 9. 总结

### 网络请求的核心步骤

```
1. 连接 WiFi
   ↓
2. 创建 HTTP 客户端
   ↓
3. 设置 URL
   ↓
4. 发送请求（GET/POST）
   ↓
5. 检查响应码
   ↓
6. 获取响应数据
   ↓
7. 解析 JSON
   ↓
8. 提取数据
   ↓
9. 使用数据
```

### 关键技术点

1. **HTTPClient**：发送 HTTP 请求
2. **ArduinoJson**：解析 JSON 数据
3. **WiFi**：网络连接管理
4. **字符串处理**：strcpy、strchr、strtok

### 学到的概念

- ✅ HTTP 协议基础
- ✅ URL 结构
- ✅ JSON 数据格式
- ✅ API 调用流程
- ✅ 错误处理
- ✅ WiFi 连接管理

---

## 下一课预告

**第六课：项目总结和扩展**

我们会学习：
- 项目整体架构回顾
- 如何添加新功能
- 常见问题排查
- 项目优化建议
- 进阶学习方向

准备好了告诉我！
