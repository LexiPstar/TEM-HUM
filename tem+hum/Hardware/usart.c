#include "stm32f10x.h"                  // Device header
#include "usart.h"

#define USARTx USART2
#define USART_PORT GPIOA
#define USART_TX GPIO_Pin_2
#define USART_RX GPIO_Pin_3


void USART2_Init(uint32_t baudrate)
{
	// 打开GPIOA、USART2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 配置PA9为复用推挽输出（TX）
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = USART_TX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(USART_PORT, &GPIO_InitStructure);

    // 配置PA10为浮空输入（RX）
    GPIO_InitStructure.GPIO_Pin = USART_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART_PORT, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	// 初始化串口参数
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USARTx, &USART_InitStructure);

    USART_Cmd(USARTx, ENABLE); // 使能串口
}

void USART2_SendByte(uint8_t byte)
{
    USART_SendData(USARTx, byte);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

void USART2_SendString(const char* str)
{
    while (*str) {
        USART2_SendByte(*str++);
    }
}

// printf重定向支持（Keil标准写法）
int fputc(int ch, FILE *f)
{
    USART2_SendByte(ch);
    return ch;
}
