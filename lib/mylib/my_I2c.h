#ifndef _MYI2C__H
#define _MYI2C__H
#include <driver/gpio.h>
#include <esp_log.h>
#include <esp_rom_sys.h>

void myI2C_Init(void);
void myI2C_Start(void);
void myI2C_SendByte(uint8_t Byte);
uint8_t myI2C_ReceiveByte(void);
void myI2C_SendAck(uint8_t ack);
uint8_t myI2C_ReceiveAck(void);
void myI2C_Stop(void);
#endif
