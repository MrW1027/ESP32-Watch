#include "my_signal.h"

char time_YMD[15];
char time_HMS[10];
char time_HM[10];

bool wifi_flag = false;
int dianliang = 1;

String t_day[5] = {"2025-03-16", "2025-03-16", "2025-03-16", "2025-03-16", "2025-03-16"};
String t_weather[5] = {"晴", "晴", "晴", "晴", "晴"};
String t_temp[5] = {"15-20C", "15-20C", "15-20C", "15-20C", "15-20C"};

String time_day = "2025-3-16";
String time_hour = "12:20";

float my_temperature=23.7;
float my_humidity=56.2;

void refresh_weather(void)
{
    HTTPClient http;
    String url = (String) "http://apis.juhe.cn/simpleWeather/query" + "?city=成都" + "&key=4ab559ae9fe53e807c8606b5005c1592";
    http.begin(url);
    int http_code = http.GET();
    String reponse = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, reponse);

    my_temperature=doc["result"]["realtime"]["temperature"].as<String>().toFloat();
    my_humidity=doc["result"]["realtime"]["humidity"].as<String>().toFloat();
    for (int i = 0; i < 5; i++)
    {
        String date_str = doc["result"]["future"][i]["date"].as<String>();
        t_day[i] = date_str.substring(5);
        // Serial.println(t_day[i]);
        t_weather[i] = doc["result"]["future"][i]["weather"].as<String>();
        t_temp[i] = doc["result"]["future"][i]["temperature"].as<String>();
    }
}
void refresh_time(void)
{
    HTTPClient http;
    String url = (String) "http://apis.juhe.cn/fapigx/worldtime/query" + "?city=成都" + "&key=5c6e94d3b478e85f9e27a3437acc5817";
    http.begin(url);
    int http_code = http.GET();
    String reponse = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, reponse);

    String t_time;
    t_time = doc["result"]["strtime"].as<String>();
    // timestamp = doc["result"]["timestamp"].as<int>();
    char str[30];
    strcpy(str, t_time.c_str());
    char *date = strchr(str, '-');
    if (date)
    {
        date++;
    }
    else
    {
        date = str;
    }
    time_day = strtok(date, " ");
    time_hour = strtok(NULL, " ");

    Serial.println(time_day);
    Serial.println(time_hour);
}
void refresh_wifi_flag(void)
{
    if (WiFi.status() != WL_CONNECTED)
        wifi_flag = false;
    else
    {
        wifi_flag = true;
    }
}

void time_task()
{
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        strftime(time_YMD, sizeof(time_YMD), "%Y-%m-%d", &timeinfo);
        strftime(time_HMS, sizeof(time_HMS), "%H:%M:%S", &timeinfo);
        strftime(time_HM, sizeof(time_HM), "%H:%M", &timeinfo);
    }
}

void buzzer_init()
{
    ledcSetup(2, 5000, 10);
    ledcAttachPin(45, 2);
    Serial.println("buzzer_init done");
}
void buzzer_on()
{
    ledcWrite(2, 512);
}
void buzzer_off()
{
    ledcWrite(2, 0);
}
void buzzer_on_5s()
{
    int now_time = millis();
    ledcWrite(2, 512);
    while (1)
    {
        if (millis() - now_time > 5000)
        {
            ledcWrite(2, 0);
            return;
        }
    }
}

String *get_t_day()
{
    return t_day;
}
String *get_t_weather()
{
    return t_weather;
}
String *get_t_temp()
{
    return t_temp;
}
bool get_wifi_flag()
{
    return wifi_flag;
}
String get_time_day()
{
    return time_day;
}
String get_time_hour()
{
    return time_hour;
}
char *get_time_YMD()
{
    return time_YMD;
}
char *get_time_HMS()
{
    return time_HMS;
}
char *get_time_HM()
{
    return time_HM;
}
float get_humidity()
{
    return my_humidity;
}
float get_temperature()
{
    return my_temperature;
}
