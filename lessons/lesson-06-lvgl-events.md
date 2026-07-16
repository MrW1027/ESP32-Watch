# 第六课：LVGL 事件处理和回调函数

## 本课目标

理解 LVGL 的事件机制，如何响应用户操作（点击、滑动等），以及回调函数的工作原理。

---

## 1. 什么是事件？

### 简单理解

**事件** = 发生的事情

**类比**：
```
生活中的事件：
- 门铃响了 → 事件：有人按门铃
- 电话响了 → 事件：有人打电话
- 闹钟响了 → 事件：到时间了

程序中的事件：
- 按钮被点击 → 事件：LV_EVENT_CLICKED
- 滑块被拖动 → 事件：LV_EVENT_VALUE_CHANGED
- 屏幕被触摸 → 事件：LV_EVENT_PRESSED
```

### LVGL 的事件系统

```
用户操作 → 触发事件 → 调用回调函数 → 执行代码

例子：
用户点击心率按钮
    ↓
LVGL 检测到点击
    ↓
触发 LV_EVENT_CLICKED 事件
    ↓
调用 screen_main_btn_heart_event_handler()
    ↓
切换到心率界面
```

---

## 2. 常见的 LVGL 事件类型

### 2.1 触摸相关事件

**文件位置**：LVGL 库定义

```cpp
LV_EVENT_PRESSED        // 按下（手指触摸屏幕）
LV_EVENT_PRESSING       // 持续按压（手指一直按着）
LV_EVENT_PRESS_LOST     // 失去按压（手指移出控件）
LV_EVENT_SHORT_CLICKED  // 短按（快速点击）
LV_EVENT_LONG_PRESSED   // 长按（按住不放）
LV_EVENT_LONG_PRESSED_REPEAT  // 长按重复
LV_EVENT_CLICKED        // 点击（按下后松开）
LV_EVENT_RELEASED       // 释放（手指离开屏幕）
```

### 2.2 值变化事件

```cpp
LV_EVENT_VALUE_CHANGED  // 值改变（滑块、开关等）
```

**例子**：
```cpp
// 滑块被拖动时触发
case LV_EVENT_VALUE_CHANGED:
{
    int value = lv_slider_get_value(slider);
    Serial.printf("滑块值: %d\n", value);
    break;
}
```

### 2.3 其他常用事件

```cpp
LV_EVENT_FOCUSED        // 获得焦点
LV_EVENT_DEFOCUSED      // 失去焦点
LV_EVENT_READY          // 准备就绪
LV_EVENT_CANCEL         // 取消
LV_EVENT_DELETE         // 删除
LV_EVENT_ALL            // 所有事件（监听所有）
```

---

## 3. 回调函数详解

### 3.1 什么是回调函数？

**回调函数** = 事件发生时自动调用的函数

**类比**：
```
你订了外卖：
1. 你告诉外卖员："到了给我打电话"
2. 外卖员到了 → 触发"到达"事件
3. 外卖员打电话 → 调用"打电话"这个回调函数
4. 你接电话，下楼取外卖 → 执行回调函数的代码

程序中：
1. 你告诉 LVGL："按钮被点击时调用这个函数"
2. 用户点击按钮 → 触发 CLICKED 事件
3. LVGL 调用你指定的函数 → 回调函数
4. 函数执行，切换界面 → 执行回调函数的代码
```

### 3.2 回调函数的结构

**文件位置**：`lib/generated/events_init.cpp`

```cpp
static void 函数名(lv_event_t *e)
{
    // 第 1 步：获取事件类型
    lv_event_code_t code = lv_event_get_code(e);
    
    // 第 2 步：判断事件类型
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        // 第 3 步：执行相应的代码
        // 你的代码...
        break;
    }
    default:
        break;
    }
}
```

**参数解释**：
- `lv_event_t *e`：事件对象，包含事件的所有信息
- `lv_event_code_t code`：事件类型（点击、长按等）

---

## 4. 实战：分析项目中的事件处理

### 4.1 主界面的心率按钮

**文件位置**：`lib/generated/events_init.cpp` - screen_main_btn_heart_event_handler()

```cpp
static void screen_main_btn_heart_event_handler(lv_event_t *e)
{
    // 获取事件类型
    lv_event_code_t code = lv_event_get_code(e);
    
    // 判断事件类型
    switch (code)
    {
    case LV_EVENT_CLICKED:  // 如果是点击事件
    {
        // 切换到心率界面
        ui_load_scr_animation(
            &guider_ui,                    // UI 对象
            &guider_ui.screen_heart,       // 目标界面（心率界面）
            guider_ui.screen_heart_del,    // 目标界面删除标志
            &guider_ui.screen_main_del,    // 当前界面删除标志
            setup_scr_screen_heart,        // 目标界面初始化函数
            LV_SCR_LOAD_ANIM_FADE_ON,      // 动画类型（淡入）
            500,                           // 动画时间（500ms）
            200,                           // 延迟时间（200ms）
            false,                         // 是否删除当前界面
            false                          // 是否删除目标界面
        );
        break;
    }
    default:
        break;
    }
}
```

**工作流程**：
```
1. 用户点击心率按钮
   ↓
2. LVGL 检测到点击
   ↓
3. 触发 LV_EVENT_CLICKED 事件
   ↓
4. 调用 screen_main_btn_heart_event_handler()
   ↓
5. 获取事件类型：LV_EVENT_CLICKED
   ↓
6. 执行 case LV_EVENT_CLICKED 中的代码
   ↓
7. 调用 ui_load_scr_animation() 切换界面
   ↓
8. 主界面淡出，心率界面淡入
```

### 4.2 设置界面的亮度滑块

**文件位置**：`lib/generated/events_init.cpp` - screen_setting_slider_hight_event_handler()

```cpp
static void screen_setting_slider_hight_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:  // 滑块值改变
    {
        // 获取滑块当前值
        int value = lv_slider_get_value(guider_ui.screen_setting_slider_hight);
        
        // 设置屏幕亮度
        my_tft_light_set(value);
        
        break;
    }
    default:
        break;
    }
}
```

**工作流程**：
```
1. 用户拖动亮度滑块
   ↓
2. 滑块值改变
   ↓
3. 触发 LV_EVENT_VALUE_CHANGED 事件
   ↓
4. 调用 screen_setting_slider_hight_event_handler()
   ↓
5. 获取滑块当前值（0-100）
   ↓
6. 调用 my_tft_light_set(value) 设置亮度
   ↓
7. 屏幕亮度实时改变
```

### 4.3 设置界面的重启按钮

**文件位置**：`lib/generated/events_init.cpp` - screen_setting_btn_restart_event_handler()

```cpp
static void screen_setting_btn_restart_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        // 重启设备
        watch_restart();
        break;
    }
    default:
        break;
    }
}
```

**工作流程**：
```
1. 用户点击重启按钮
   ↓
2. 触发 LV_EVENT_CLICKED 事件
   ↓
3. 调用 screen_setting_btn_restart_event_handler()
   ↓
4. 调用 watch_restart() 函数
   ↓
5. ESP32 重启
```

---

## 5. 注册事件回调

### 5.1 如何注册回调函数？

**文件位置**：`lib/generated/events_init.cpp` - events_init_screen_main()

```cpp
void events_init_screen_main(lv_ui *ui)
{
    // 为心率按钮注册回调
    lv_obj_add_event_cb(
        ui->screen_main_btn_heart,           // 控件对象
        screen_main_btn_heart_event_handler, // 回调函数
        LV_EVENT_ALL,                        // 监听所有事件
        ui                                   // 用户数据
    );
    
    // 为天气按钮注册回调
    lv_obj_add_event_cb(
        ui->screen_main_btn_weather,
        screen_main_btn_weather_event_handler,
        LV_EVENT_ALL,
        ui
    );
    
    // 为 AI 按钮注册回调
    lv_obj_add_event_cb(
        ui->screen_main_btn_ai,
        screen_main_btn_ai_event_handler,
        LV_EVENT_ALL,
        ui
    );
    
    // 为设置图标注册回调
    lv_obj_add_event_cb(
        ui->screen_main_img_setting,
        screen_main_img_setting_event_handler,
        LV_EVENT_ALL,
        ui
    );
}
```

**参数详解**：

1. **控件对象**：`ui->screen_main_btn_heart`
   - 要监听的控件
   - 例如：按钮、滑块、图片等

2. **回调函数**：`screen_main_btn_heart_event_handler`
   - 事件发生时调用的函数
   - 必须符合 `void func(lv_event_t *e)` 格式

3. **事件过滤器**：`LV_EVENT_ALL`
   - `LV_EVENT_ALL`：监听所有事件
   - `LV_EVENT_CLICKED`：只监听点击事件
   - `LV_EVENT_VALUE_CHANGED`：只监听值改变事件

4. **用户数据**：`ui`
   - 传递给回调函数的额外数据
   - 可以在回调函数中通过 `lv_event_get_user_data(e)` 获取

### 5.2 注册流程

```
1. 创建 UI 控件
   lv_obj_t *btn = lv_btn_create(parent);
   ↓
2. 编写回调函数
   void my_btn_event_handler(lv_event_t *e) { ... }
   ↓
3. 注册回调函数
   lv_obj_add_event_cb(btn, my_btn_event_handler, LV_EVENT_ALL, NULL);
   ↓
4. 用户操作控件
   点击按钮
   ↓
5. LVGL 自动调用回调函数
   my_btn_event_handler() 被执行
```

---

## 6. 界面切换动画

### 6.1 ui_load_scr_animation() 函数

**文件位置**：`lib/generated/events_init.cpp`（调用处）

```cpp
ui_load_scr_animation(
    &guider_ui,                    // UI 对象
    &guider_ui.screen_heart,       // 目标界面
    guider_ui.screen_heart_del,    // 目标界面删除标志
    &guider_ui.screen_main_del,    // 当前界面删除标志
    setup_scr_screen_heart,        // 目标界面初始化函数
    LV_SCR_LOAD_ANIM_FADE_ON,      // 动画类型
    500,                           // 动画时间（ms）
    200,                           // 延迟时间（ms）
    false,                         // 是否删除当前界面
    false                          // 是否删除目标界面
);
```

### 6.2 动画类型

```cpp
LV_SCR_LOAD_ANIM_NONE           // 无动画（立即切换）
LV_SCR_LOAD_ANIM_OVER_LEFT      // 从左侧覆盖
LV_SCR_LOAD_ANIM_OVER_RIGHT     // 从右侧覆盖
LV_SCR_LOAD_ANIM_OVER_TOP       // 从顶部覆盖
LV_SCR_LOAD_ANIM_OVER_BOTTOM    // 从底部覆盖
LV_SCR_LOAD_ANIM_MOVE_LEFT      // 向左移动
LV_SCR_LOAD_ANIM_MOVE_RIGHT     // 向右移动
LV_SCR_LOAD_ANIM_MOVE_TOP       // 向上移动
LV_SCR_LOAD_ANIM_MOVE_BOTTOM    // 向下移动
LV_SCR_LOAD_ANIM_FADE_ON        // 淡入淡出
```

**项目中的使用**：

```cpp
// 主界面 → 心率界面：淡入淡出
LV_SCR_LOAD_ANIM_FADE_ON

// 心率界面 → 主界面：向左移动（返回效果）
LV_SCR_LOAD_ANIM_MOVE_LEFT

// 主界面 → AI 界面：从左侧覆盖
LV_SCR_LOAD_ANIM_OVER_LEFT
```

---

## 7. 从事件中获取信息

### 7.1 获取事件类型

```cpp
lv_event_code_t code = lv_event_get_code(e);
```

### 7.2 获取触发事件的控件

```cpp
lv_obj_t *obj = lv_event_get_target(e);
```

**例子**：
```cpp
void my_event_handler(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    
    // 修改按钮文本
    lv_obj_t *label = lv_obj_get_child(btn, 0);
    lv_label_set_text(label, "已点击");
}
```

### 7.3 获取用户数据

```cpp
void *user_data = lv_event_get_user_data(e);
```

**例子**：
```cpp
// 注册时传递数据
int my_data = 123;
lv_obj_add_event_cb(btn, my_handler, LV_EVENT_ALL, &my_data);

// 回调函数中获取数据
void my_handler(lv_event_t *e)
{
    int *data = (int *)lv_event_get_user_data(e);
    Serial.printf("数据: %d\n", *data);  // 输出：数据: 123
}
```

### 7.4 获取触摸坐标（触摸事件）

```cpp
lv_point_t point;
lv_indev_get_point(lv_indev_get_act(), &point);
Serial.printf("触摸坐标: (%d, %d)\n", point.x, point.y);
```

---

## 8. 自定义事件处理

### 8.1 添加自己的事件处理

**文件位置**：`lib/generated/custom.c`

```cpp
void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    
    // 例子：为主界面添加长按事件
    lv_obj_add_event_cb(
        ui->screen_main,
        my_custom_event_handler,
        LV_EVENT_LONG_PRESSED,
        NULL
    );
}

void my_custom_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_LONG_PRESSED) {
        Serial.println("长按主界面");
        // 执行自定义操作
    }
}
```

### 8.2 实战例子：双击返回主界面

```cpp
// 全局变量：记录上次点击时间
static uint32_t last_click_time = 0;

void double_click_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        uint32_t now = millis();
        
        // 如果两次点击间隔小于 500ms，认为是双击
        if (now - last_click_time < 500) {
            Serial.println("检测到双击");
            // 返回主界面
            ui_load_scr_animation(...);
        }
        
        last_click_time = now;
    }
}
```

---

## 9. 事件传播和阻止

### 9.1 事件传播

```
事件传播顺序：
子控件 → 父控件 → 祖父控件 → ...

例子：
按钮（子） → 容器（父） → 屏幕（祖父）
```

### 9.2 阻止事件传播

```cpp
void my_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        // 处理事件
        Serial.println("按钮被点击");
        
        // 阻止事件继续传播到父控件
        lv_event_stop_bubbling(e);
    }
}
```

---

## 10. 常见问题

### 问题 1：回调函数没有被调用

**原因**：
- 没有注册回调函数
- 事件类型不匹配
- 控件被禁用或隐藏

**解决**：
```cpp
// 1. 确认已注册
lv_obj_add_event_cb(btn, my_handler, LV_EVENT_ALL, NULL);

// 2. 检查控件状态
lv_obj_clear_state(btn, LV_STATE_DISABLED);  // 启用控件
lv_obj_clear_flag(btn, LV_OBJ_FLAG_HIDDEN);  // 显示控件

// 3. 添加调试信息
void my_handler(lv_event_t *e)
{
    Serial.println("回调函数被调用");
    // ...
}
```

### 问题 2：界面切换后程序崩溃

**原因**：
- 访问了已删除的控件
- 内存不足

**解决**：
```cpp
// 1. 不要删除界面（设置为 false）
ui_load_scr_animation(..., false, false);

// 2. 在切换前停止定时器
lv_timer_del(my_timer);

// 3. 增加任务栈大小
xTaskCreatePinnedToCore(..., 16384, ...);  // 增加到 16KB
```

### 问题 3：滑块值不更新

**原因**：
- 没有监听 LV_EVENT_VALUE_CHANGED
- 获取值的方式错误

**解决**：
```cpp
// 正确的方式
void slider_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *slider = lv_event_get_target(e);
        int value = lv_slider_get_value(slider);
        Serial.printf("滑块值: %d\n", value);
    }
}
```

---

## 11. 性能优化

### 11.1 只监听需要的事件

```cpp
// 不好：监听所有事件
lv_obj_add_event_cb(btn, my_handler, LV_EVENT_ALL, NULL);

// 好：只监听点击事件
lv_obj_add_event_cb(btn, my_handler, LV_EVENT_CLICKED, NULL);
```

### 11.2 避免在回调中执行耗时操作

```cpp
// 不好：在回调中延时
void my_handler(lv_event_t *e)
{
    delay(1000);  // 会阻塞 UI
}

// 好：使用定时器或任务
void my_handler(lv_event_t *e)
{
    // 设置标志，在任务中处理
    need_process = true;
}
```

### 11.3 复用回调函数

```cpp
// 多个按钮使用同一个回调函数
void common_btn_handler(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    
    if (btn == ui->btn1) {
        // 处理按钮 1
    } else if (btn == ui->btn2) {
        // 处理按钮 2
    }
}

lv_obj_add_event_cb(ui->btn1, common_btn_handler, LV_EVENT_CLICKED, NULL);
lv_obj_add_event_cb(ui->btn2, common_btn_handler, LV_EVENT_CLICKED, NULL);
```

---

## 12. 总结

### 事件处理流程

```
1. 用户操作（点击、滑动等）
   ↓
2. LVGL 检测到操作
   ↓
3. 生成事件对象（lv_event_t）
   ↓
4. 查找注册的回调函数
   ↓
5. 调用回调函数
   ↓
6. 回调函数执行代码
   ↓
7. 更新 UI 或执行其他操作
```

### 关键技术点

1. **事件类型**：CLICKED、VALUE_CHANGED、PRESSED 等
2. **回调函数**：事件发生时自动调用的函数
3. **注册回调**：lv_obj_add_event_cb()
4. **获取信息**：lv_event_get_code()、lv_event_get_target()
5. **界面切换**：ui_load_scr_animation()

### 学到的概念

- ✅ 事件的概念和类型
- ✅ 回调函数的结构和用法
- ✅ 如何注册事件回调
- ✅ 如何从事件中获取信息
- ✅ 界面切换动画
- ✅ 自定义事件处理

---

## 下一课预告

**第七课：项目总结和扩展**

我们会学习：
- 项目整体架构回顾
- 数据流向分析
- 如何添加新功能
- 常见问题排查
- 进阶学习方向

准备好了告诉我！
