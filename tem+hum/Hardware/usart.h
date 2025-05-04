#ifndef __USART_H
#define __USART_H

#include <stdio.h>

void USART2_Init(uint32_t baudrate);
void USART2_SendByte(uint8_t byte);
void USART2_SendString(const char* str);

#endif
