#include <WiFi.h>
#include <PubSubClient.h>

#include "my_mqtt.h"
#include "my_gps.h"
#include "my_max30102.h"
#include "my_signal.h"

extern SemaphoreHandle_t xMutex;
// 阿里云MQTT配置
const char *mqtt_server = "iot-06z00hk7bhez32x.mqtt.iothub.aliyuncs.com";
const int mqtt_port = 1883;
const char *mqtt_clientId = "k29qon0Ev9d.STM32-01|securemode=2,signmethod=hmacsha256,timestamp=1743078848306|";
const char *mqtt_username = "STM32-01&k29qon0Ev9d";
const char *mqtt_password = "1d5a10dff43e9ee0a87a15329db8a4b551d4b48323cd5fd6be71028be18e15e1";

#define PRODUCT_KEY "k29qon0Ev9d"                        // 产品ID
#define DEVICE_NAME "STM32-01"                           // 设备名
#define DEVICE_SECRET "072637e82d724977f5e394f5fc4e15f7" // 设备key

// 设备下发命令的set主题
#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
// 设备上传数据的post主题
#define ALINK_TOPIC_PROP_POST "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
// 设备post上传数据要用到一个json字符串, 这个是拼接postJson用到的一个字符串
#define ALINK_METHOD_PROP_POST "thing.event.property.post"
// 这是post上传数据使用的模板
#define ALINK_BODY_FORMAT "{\"id\":\"%u\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"

WiFiClient espClient;
PubSubClient client(espClient);
int postMsgId = 0; // 记录已经post了多少条

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_clientId, mqtt_username, mqtt_password))
    {
      Serial.println("connected");
      client.subscribe("/a1xxxxxx/${deviceName}/user/get");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void my_mqtt_init()
{

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  Serial.println("my_mqtt_init_done");
  // reconnect();
}

void my_mqtt_publish()
{

  if (!client.connected())
  {
    return;
    //reconnect();
  }

  client.loop();

  Serial.println("--------------------------------------------------------------------------------------------------------------------------开始发布消息");
  // 先拼接出json字符串
  char param[32];
  char jsonBuf[128];
  postMsgId += 1;

  // 血压低
  sprintf(param, "{\"bloodPressure_L\":%d}", 85); // 我们把要上传的数据写在param里
  sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, param);
  client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  // 血氧
  sprintf(param, "{\"bloodOxygen\":%d}", get_spo2()); // 我们把要上传的数据写在param里
  sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, param);
  client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  // 经度
  sprintf(param, "{\"longitude\":%f}", get_gps_E()); // 我们把要上传的数据写在param里
  sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, param);
  client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  // 维度
  sprintf(param, "{\"latitude\":%f}", get_gps_N()); // 我们把要上传的数据写在param里
  sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, param);
  client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  // 湿度
  sprintf(param, "{\"humidity\":%f}", get_humidity()); // 我们把要上传的数据写在param里
  sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, param);
  client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  // 温度
  sprintf(param, "{\"temperature\":%f}",get_temperature()); // 我们把要上传的数据写在param里
  sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, param);
  client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  // 血压高
  sprintf(param, "{\"bloodPressure_H\":%d}", 132); // 我们把要上传的数据写在param里
  sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, param);
  client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  // 心率
  sprintf(param, "{\"heartRate\":%d}", get_heart_rate()); // 我们把要上传的数据写在param里
  sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, param);
  client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);

  //vTaskDelete(nullptr);
}

// void my_mqtt_task(void *para)
// {
//   Serial.println("--------------------------------------------------------------------------------------------------------------------------开始发布消息");
//   // if (xSemaphoreTake(xMutex, portMAX_DELAY))
//   //   my_mqtt_publish();
//   // // vTaskDelete(nullptr);
//   // xSemaphoreGive(xMutex);
//   xTaskCreatePinnedToCore(my_mqtt_publish, "my_mqtt_publish", 4096, NULL, 3, NULL, 1);
//   delay(3000);
// }