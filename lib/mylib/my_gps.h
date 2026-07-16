#ifndef __MY_GPS_H
#define __MY_GPS_H

#include <Arduino.h>
#include "vector"
#include <sstream>
#include <string>


void gps_task(void* para);
double get_gps_N(void);
double convertToDecimalDegrees(double value);
double get_gps_E(void);

#endif