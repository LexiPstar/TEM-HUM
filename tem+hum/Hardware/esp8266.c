#include "esp8266.h"

#define esp8266_port GPIOA
#define esp8266_pin_TX GPIO_Pin_9
#define esp8266_pin_RX GPIO_Pin_10
#define USARTx USART1

void USART1_Init(uint32_t baudrate)
{
	//开启GPIOA、串口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//初始化引脚
    GPIO_InitTypeDef gpio_structure;
    gpio_structure.GPIO_Pin = esp8266_pin_TX;  // TX
    gpio_structure.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_structure);

    gpio_structure.GPIO_Pin = esp8266_pin_RX;  // RX
    gpio_structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio_structure);
	
	//配置串口
    USART_InitTypeDef usart;
    usart.USART_BaudRate = baudrate;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USARTx, &usart);

    USART_Cmd(USARTx, ENABLE);//串口使能
}

void USART1_SendString(const char* str)
{
    while(*str) {
        USART_SendData(USARTx, *str++);//发送字符串
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);//等待清除标志位
    }
}

void esp8266_Init(void)
{
	USART1_SendString("AT\r\n");  // 发送基本AT测试命令
	Delay_ms(1000);               // 等待返回（可以加串口接收打印）

	USART1_SendString("AT+CWMODE=2\r\n");  // 设置为AP模式
	Delay_ms(1000);
	
	USART1_SendString("AT+CIPMUX=1\r\n");  // 启用多连接
    Delay_ms(1000);
	
	USART1_SendString("AT+CWJAP=\"esp\",\"esp123456\"\r\n");  // 配置WiFi
	Delay_ms(1000);
	
    // 启动 TCP 服务器，监听80端口
    USART1_SendString("AT+CIPSERVER=1,80\r\n");
    Delay_ms(1000);
}

// 向 ESP8266 发送网页内容
void esp8266_SendWebpage(uint8_t temp, uint8_t humi)
{
    char webpage[512]; // 网页内容
    char http[256];    // HTTP 指令
    int content_len = snprintf(webpage,
        sizeof(webpage),
        "<html><head><meta charset=\"UTF-8\"><title>温湿度监测</title></head>"
        "<body><h1>环境监测站</h1><p>温度: %d °C</p><p>湿度: %d %%</p></body></html>", 
        temp, humi);

    // 构造 AT+CIPSEND 指令，发送网页长度
    snprintf(http, sizeof(http), "AT+CIPSEND=0,%d\r\n", content_len);
    USART1_SendString(http);
    Delay_ms(500);  // 等待 ESP8266 的 '>' 提示符

    // 分批发送网页内容
    int chunk_size = 200;  // 设置每次发送的块大小
    for (int i = 0; i < content_len; i += chunk_size) {
        // 发送网页的部分内容
        char chunk[chunk_size + 1];  // +1 是为了存放字符串的结束符 '\0'
        int remaining = content_len - i;
        int size_to_send = (remaining > chunk_size) ? chunk_size : remaining;
        strncpy(chunk, &webpage[i], size_to_send);
        chunk[size_to_send] = '\0';  // 确保字符串正确结束

        // 发送当前块
        USART1_SendString(chunk);
        Delay_ms(100);  // 可调整延迟时间，确保不会发生溢出
    }
	//容器满  则清空数组
	if(strlen(webpage) >= 300) webpage[0] = '\0';
	if(strlen(http) >= 512) webpage[0] = '\0';
	Delay_ms(100);
}



// 温度、湿度值作为参数
void print_temp_humi(uint8_t temp, uint8_t humi)
{
    printf("Temperature: %dC, Humidity: %d%%\r\n", temp, humi);
}

