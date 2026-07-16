#ifndef __MY_SMTP_H
#define __MY_SMTP_H

#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include "my_gps.h"
void sendEmail(void* para);
void sendEmail_init();
#endif