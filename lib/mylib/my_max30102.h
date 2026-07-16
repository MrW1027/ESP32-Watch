#ifndef __MY_MAX30102_H
#define __MY_MAX30102_H

#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"


void max30102_task(void* para);

int32_t get_heart_rate();
int32_t get_spo2();
#endif