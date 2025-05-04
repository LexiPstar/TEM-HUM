#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "stdio.h"
#include "string.h"

void USART1_Init(uint32_t baudrate);

void USART1_SendString(const char* str);
void esp8266_Init(void);
void esp8266_SendWebpage(uint8_t temp, uint8_t humi);

void print_temp_humi(uint8_t temp, uint8_t humi);

#endif
