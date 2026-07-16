#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <Wire.h>
#include "CST816T.h"

#include "WiFiUser.h"
#include "my_smtp.h"
#include "my_signal.h"
#include "my_max30102.h"
#include "my_mpu6050.h"
#include "my_timer.h"
#include "my_gps.h"
#include "my_mqtt.h"

#include <gui_guider.h>
#include <events_init.h>
#include "custom.h"

#define I2C_SDA 17
#define I2C_SCL 16
#define RST_N_PIN 18
#define INT_N_PIN 8

int connectTimeOut_s = 10;

SemaphoreHandle_t xMutex;

/*Change to your screen resolution*/
static const uint32_t screenWidth = 240;
static const uint32_t screenHeight = 280;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 2];
// static lv_color_t buf[screenWidth * 10];

TFT_eSPI tft = TFT_eSPI();
CST816T cst816t(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN);
hw_timer_t *tim1 = NULL;
hw_timer_t *tim2 = NULL;
lv_ui guider_ui;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

// /*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  TouchInfos tp;
  tp = cst816t.GetTouchInfo();
  // bool touched = (tp.touching == 1&&tp.isValid == 1);
  bool touched = tp.touching;

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = tp.x;
    data->point.y = tp.y;

    // Serial.printf("touchX = %d, touchY = %d\n", tp.x, tp.y);
  }
}

void tim1Interrupt()
{
  lv_tick_inc(1);
}

void lvgl_handler(void *pvParameters)
{
  while (1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY))
      lv_timer_handler(); /* let the GUI do its work */
    xSemaphoreGive(xMutex);
    delay(5);
  }
}

void my_signal_inti(void *pvParameters)
{
  if (xSemaphoreTake(xMutex, portMAX_DELAY))
    refresh_time();
  vTaskDelete(nullptr);
  xSemaphoreGive(xMutex);
}

void setup()
{
  Serial.begin(115200);

  xMutex = xSemaphoreCreateMutex();

  tft.init();
  Serial.println("Setup tft done");

  cst816t.begin();
  Serial.println("Setup cst816t done");

  connectToWiFi(connectTimeOut_s);
  //在连接上网络后，与服务器的连接立马进行，不要再任务中在进行，否则容易超时触发看门狗
  configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  refresh_weather();
  my_tft_init();
  buzzer_init();
  my_mqtt_init();
  reconnect();

  lv_init();
  Serial.println("Setup lv_init done");

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 2);
  // lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenWidth * 100);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  // source tick for lvgl
  tim1 = timerBegin(0, 80, true);
  timerAttachInterrupt(tim1, tim1Interrupt, true);
  timerAlarmWrite(tim1, 1000, true);
  timerAlarmEnable(tim1);

  setup_ui(&guider_ui);
  events_init(&guider_ui);
  custom_init(&guider_ui);

  xTaskCreatePinnedToCore(lvgl_handler, "lvgl_handler", 8192, NULL, 2, NULL, 0);

  //任务里面的延时不能取消，必须留一定的时间进行线程切换
  xTaskCreatePinnedToCore(gps_task, "gps_task", 4096, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(MPU6050_task, "MPU6050_task", 8192, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(max30102_task, "max30102_task", 4096, NULL, 4, NULL, 1); 
}

void loop()
{
  delay(10);
  // delay(1000);
  // my_mqtt_publish();
}
