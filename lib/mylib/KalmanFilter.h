#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

class KalmanFilter {
public:
    KalmanFilter();  // 构造函数
    float getAngle(float newAngle, float newRate, float dt);  // 计算卡尔曼滤波后的角度

private:
    float Q_angle;  // 角度过程噪声
    float Q_bias;   // 偏差过程噪声
    float R_measure; // 观测噪声

    float angle;  // 估计的角度
    float bias;   // 估计的偏差
    float rate;   // 角速度
    float P[2][2]; // 误差协方差矩阵
};

#endif
