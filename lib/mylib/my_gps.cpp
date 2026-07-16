#include "my_gps.h"

using namespace std;

double N=0.0;
double E=0.0;

void gps_analysis(String receivedData)
{
    vector<string> result;
    string str=receivedData.c_str();
    stringstream ss(str);
    string token;

    while (getline(ss, token, ','))
    {
        result.push_back(token);
    }
    if (result.empty() || result[0] != "$GNGLL" || result.size() < 5) return;

    // 解析经纬度
    if (!result[1].empty() && !result[3].empty()) {
        N = atof(result[1].c_str());
        E = atof(result[3].c_str());
        Serial.printf("成功得到经纬度 N: %f E: %f\n", N, E);
    }
}

void gps_task(void *para)
{
    Serial2.begin(9600, SERIAL_8N1, 40, 39);
    Serial.println("gps set done");

    while (1)
    {
        // Serial.println(millis());
        if (Serial2.available())
        {
            String receivedData = Serial2.readStringUntil('\n');
            gps_analysis(receivedData);
            
            Serial.printf("receivedData:%s\n",receivedData.c_str());
        }
        delay(100);
    }
}
double convertToDecimalDegrees(double value) {
    int degrees = static_cast<int>(value / 100);  // 获取度
    double minutes = value - degrees * 100;       // 获取分
    return degrees + (minutes / 60.0);            // 转换为十进制度数
}
double get_gps_N(void)
{
    return convertToDecimalDegrees(N);
}
double get_gps_E(void)
{
    return convertToDecimalDegrees(E);
}