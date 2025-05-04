#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "usart.h"
#include "OLED.h"
#include "DHT11.h"
#include "esp8266.h"
#include "buzzer.h"

int main(void)
{
    // 初始化系统时钟、延时函数
    SystemInit();    

    // 外设初始化
    USART2_Init(115200);        // 串口调试
    OLED_Init();                // OLED 显示
    DHT11_Init();               // DHT11 初始化
    USART1_Init(115200);        // ESP8266 初始化
//  Control_Init();             // 温控逻辑初始化（可选）
	esp8266_Init();
	buzzer_Init();
	
    uint8_t temperature = 0, humidity = 0;

    while (1)
    {
        if (DHT11_Read_Data(&temperature, &humidity) == 0)
        {
			esp8266_SendWebpage(temperature, humidity);
			
			OLED_ShowString(1, 1, "Tem:  C");
			OLED_ShowString(2, 1, "Hum:  %");
            OLED_ShowNum(1, 5, temperature, 2); // 显示温度
            OLED_ShowNum(2, 5, humidity, 2);    // 显示湿度

            print_temp_humi(temperature, humidity);
			
			if(temperature > 35){buzzer_on();}	else buzzer_off();
			if(humidity > 50){buzzer_on();} else buzzer_off();
			
        }
        else
        {
			OLED_ShowString(3, 1, "ERROR");
            printf("读取 DHT11 失败\r\n");
        }

        Delay_ms(3000); // 每5秒采样一次
    }
}
