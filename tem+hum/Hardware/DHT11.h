#ifndef __DHT11_H
#define __DHT11_H

void DHT11_IO_OUT(void);
void DHT11_IO_IN(void);

//起始信号
void DHT11_Rst(void);

//判断状态
uint8_t DHT11_Check(void);
//读取数据位
uint8_t DHT11_Read_Bit(void);
//读取字节数据
uint8_t DHT11_Read_Byte(void);
//发送数据
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi);

void DHT11_Init(void);

#endif
