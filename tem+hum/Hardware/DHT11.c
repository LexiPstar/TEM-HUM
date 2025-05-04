#include "stm32f10x.h"                  // Device header
#include "DHT11.h"
#include "Delay.h"

#define DHT11_PIN GPIO_Pin_8
#define DHT11_PORT GPIOA

//DHT11属于单线双向通信  需要配置输入和输出 手动切换模式
void DHT11_IO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置为输出模式（推挽输出），让 STM32 主动控制总线电平  用于“起始信号”和“拉高拉低”
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

void DHT11_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置为输入模式（浮空输入），STM32 读取从 DHT11 返回的数据  读取回应和采集数据
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

//起始信号
void DHT11_Rst(void)
{
    DHT11_IO_OUT();
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN); //先拉低电平启动
    Delay_ms(20);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);   //再拉高接收
    Delay_us(30);
}

//判断状态
uint8_t DHT11_Check(void)
{
    uint8_t retry = 0;//超时计数器
    DHT11_IO_IN(); //读取 DHT11 的回应信号
	
	//等待 DHT11 拉低（80us），表示它收到了起始信号并准备回应
    while(GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) && retry < 100)
	{
        retry++; Delay_us(1);
    }
    if (retry >= 100) return 1;//未收到信号

	//等待拉高（80us）
    retry = 0;
    while(!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) && retry < 100) {
        retry++; Delay_us(1);
    }
    return (retry >= 100) ? 1 : 0;//是否正常
}

//读取数据位
uint8_t DHT11_Read_Bit(void)
{
    uint8_t retry = 0;
	//等待拉低 起始信号
    while(GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) && retry++ < 100) Delay_us(1);
    retry = 0;
	//等待拉高 开始响应
    while(!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) Delay_us(1);
    Delay_us(40);
	
    return GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN);
}

//读取字节数据
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, byte = 0;
    for(i = 0; i < 8; i++) {
        byte <<= 1;
        byte |= DHT11_Read_Bit();
    }
    return byte;
}

//发送数据
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5], i;
    DHT11_Rst();//起始信号
    if(DHT11_Check() == 0) {
		//8bit湿度整数数据+8bit湿度小数数据+8bi温度整数数据+8bit温度小数数据+8bit校验和
        for(i = 0; i < 5; i++) buf[i] = DHT11_Read_Byte();
		//校验数据
        if((buf[0]+buf[1]+buf[2]+buf[3]) == buf[4]) {
            *humi = buf[0];
            *temp = buf[2];
        }
    } else return 1;
    return 0;
}

void DHT11_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	DHT11_IO_OUT();
	GPIO_SetBits(DHT11_PORT, DHT11_PIN);//默认置高电平
}
