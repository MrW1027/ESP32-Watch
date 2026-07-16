#include "my_mpu6050.h"
#include "KalmanFilter.h"
#include "my_signal.h"
#include "my_smtp.h"

#define MPU6050_Address 0xD0

#define FALL_ACCEL_THRESHOLD_LOW 0.6  // 自由落体加速度阈值 (g)
#define FALL_ACCEL_THRESHOLD_HIGH 1.5 // 撞击加速度阈值 (g)
#define FALL_GYRO_THRESHOLD 140       // 陀螺仪角速度阈值 (°/s)
#define FALL_ANGLE_THRESHOLD 60       // 姿态角变化阈值 (°)
/**
 * 函    数：MPU6050写寄存器
 * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
 * 参    数：Data 要写入寄存器的数据，范围：0x00~0xFF
 * 返 回 值：无
 */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{

  myI2C_Start();
  myI2C_SendByte(MPU6050_Address);
  myI2C_ReceiveAck();
  myI2C_SendByte(RegAddress);
  myI2C_ReceiveAck();
  myI2C_SendByte(Data);
  myI2C_ReceiveAck();
  myI2C_Stop();
}
/**
 * 函    数：MPU6050读寄存器
 * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
 * 返 回 值：读取寄存器的数据，范围：0x00~0xFF
 */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
  uint8_t Data;

  myI2C_Start();                   // I2C起始
  myI2C_SendByte(MPU6050_Address); // 发送从机地址，读写位为0，表示即将写入
  myI2C_ReceiveAck();              // 接收应答
  myI2C_SendByte(RegAddress);      // 发送寄存器地址
  myI2C_ReceiveAck();              // 接收应答

  myI2C_Start();                          // I2C重复起始
  myI2C_SendByte(MPU6050_Address | 0x01); // 发送从机地址，读写位为1，表示即将读取
  myI2C_ReceiveAck();                     // 接收应答
  Data = myI2C_ReceiveByte();             // 接收指定寄存器的数据
  myI2C_SendAck(1);                       // 发送应答，给从机非应答，终止从机的数据输出
  myI2C_Stop();                           // I2C终止

  return Data;
}
/**
 * 函    数：MPU6050初始化
 * 参    数：无
 * 返 回 值：无
 */
void MPU6050_Init(void)
{
  myI2C_Init();
  MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01); // 唤醒
  MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
  MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);   // 10分频
  MPU6050_WriteReg(MPU6050_CONFIG, 0x06);       // 数字低通滤波器
  MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);  // 陀螺仪寄存器
  MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18); // 加速度寄存器 最大量程
}

/**
 * 函    数：MPU6050获取ID号
 * 参    数：无
 * 返 回 值：MPU6050的ID号
 */
uint8_t MPU6050_GetID(void)
{
  return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
 * 函    数：MPU6050获取数据
 * 参    数：AccX AccY AccZ 加速度计X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
 * 参    数：GyroX GyroY GyroZ 陀螺仪X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
 * 返 回 值：无
 * 具体选择转的角速度是多少 是通过比例公式计算出来的  读取的数据/32768  =  x /满量程  求x
 */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
  uint8_t DataH, DataL;

  DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H); // 读取加速度x轴寄存器的高八位
  DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L); // 读取加速度x轴寄存器的低八位
  *AccX = (DataH << 8) | DataL;                  // 读取

  DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H); // 读取加速度y轴寄存器的高八位
  DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L); // 读取加速度y轴寄存器的低八位
  *AccY = (DataH << 8) | DataL;                  // 返回出去

  DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H); // 读取加速度z轴寄存器的高八位
  DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L); // 读取加速度z轴寄存器的低八位
  *AccZ = (DataH << 8) | DataL;                  // 返回出去

  DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H); // 读取陀螺仪X轴的高8位数据
  DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L); // 读取陀螺仪X轴的低8位数据
  *GyroX = (DataH << 8) | DataL;                // 数据拼接，通过输出参数返回

  DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H); // 读取陀螺仪Y轴的高8位数据
  DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L); // 读取陀螺仪Y轴的低8位数据
  *GyroY = (DataH << 8) | DataL;                // 数据拼接，通过输出参数返回

  DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H); // 读取陀螺仪Z轴的高8位数据
  DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L); // 读取陀螺仪Z轴的低8位数据
  *GyroZ = (DataH << 8) | DataL;                // 数据拼接，通过输出参数返回
}

void MPU6050_task(void *para)
{
  uint8_t ID;                     // 存放 MPU6050 设备 ID
  int16_t AX, AY, AZ, GX, GY, GZ; // 存放加速度 & 陀螺仪数据
  KalmanFilter kalmanX, kalmanY;

  MPU6050_Init();
  ID = MPU6050_GetID();
  Serial.println("mup6050 set done");

  float yaw = 0.0;       // 偏航角初值
  const float dt = 0.01; // 采样时间（秒）

  int last_time=millis();
  while (1)
  {
    MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);

    // 归一化加速度
    float a_x = AX / 2048.0;
    float a_y = AY / 2048.0;
    float a_z = AZ / 2048.0;

    // 计算总加速度 (单位：g)
    float A_total = sqrt(a_x * a_x + a_y * a_y + a_z * a_z);

    // 计算角速度
    float G_total = sqrt(GX * GX + GY * GY + GZ * GZ) / 131.0;

    // 计算加速度角度
    float accelRoll = atan2(a_y, a_z) * 180 / M_PI;
    float accelPitch = atan2(-a_x, sqrt(a_y * a_y + a_z * a_z)) * 180 / M_PI;

    // 计算角速度（陀螺仪数据转换为 °/s）
    float gyroRollRate = GX / 16.4;
    float gyroPitchRate = GY / 16.4;
    float gyroYawRate = GZ / 16.4;

    // 卡尔曼滤波计算 Roll 和 Pitch
    float roll = kalmanX.getAngle(accelRoll, gyroRollRate, dt);
    float pitch = kalmanY.getAngle(accelPitch, gyroPitchRate, dt);

    // 互补滤波计算 Yaw（简单方法）
    // 这个不准
    float alpha = 0.98; // 互补滤波参数
    yaw = alpha * (yaw + gyroYawRate * dt) + (1 - alpha) * yaw;

    // **🚨 摔倒检测 🚨**
    if ((A_total < FALL_ACCEL_THRESHOLD_LOW || A_total > FALL_ACCEL_THRESHOLD_HIGH) && // 低加速度 或 高加速度
        G_total > FALL_GYRO_THRESHOLD &&                                               // 高角速度
        (abs(roll) > FALL_ANGLE_THRESHOLD || abs(pitch) > FALL_ANGLE_THRESHOLD))       // 终止角度异常
    {
      //xTaskCreatePinnedToCore(buzzer_on_5s, "buzzer_on_5s", 1024, NULL, 4, NULL,1);
     
      Serial.println("⚠️⚠️⚠️ 检测到摔倒！⚠️⚠️⚠️ ⚠️⚠️⚠️ 检测到摔倒！⚠️⚠️⚠️ ⚠️⚠️⚠️ 检测到摔倒！⚠️⚠️⚠️⚠️⚠️⚠️ 检测到摔倒！⚠️⚠️⚠️ ");
      xTaskCreatePinnedToCore(sendEmail, "sendEmail", 8192, NULL, 4, NULL, 1);
      //sendEmail(nullptr);
      buzzer_on_5s();
     
    }
    int now_time=millis();
    if(now_time-last_time>1000)
    {// 输出欧拉角
    Serial.printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f ,A_total: %.2f, G_total: %.2f\n", roll, pitch, yaw, A_total, G_total);
    last_time=now_time;
    }
    delay(10);
  }
}
