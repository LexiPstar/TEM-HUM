#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_flash.h"
#include "stdio.h"
#include "string.h"
#include <stdint.h>

// 定义用于保存的单条历史记录结构体（温度、湿度）
typedef struct {
    uint16_t temperature;  // 温度（整型，0~65535）
    uint16_t humidity;     // 湿度（整型，0~65535）
} FlashRecord;

// 每页 1KB，减去 4 字节标志位，其余空间用于保存历史记录
// 每条记录占用 4 字节，因此最大记录数：(1024 - 4) / 4 = 255 条
#define MAX_RECORDS 255

/**
 * @brief 初始化 Flash 历史记录模块。
 *        如果未初始化，则擦除 Flash 页面并写入标志位。
 *        如果已初始化，则自动查找写入索引。
 */
void FlashHistory_Init(void);

/**
 * @brief 擦除 Flash 中历史记录页并重新初始化。
 *        会清除所有保存的温湿度数据。
 */
void FlashHistory_Clear(void);

/**
 * @brief 写入一条新的温湿度历史记录。
 * @param temp 当前温度值（uint8_t）
 * @param humi 当前湿度值（uint8_t）
 */
void FlashHistory_Write(uint8_t temp, uint8_t humi);

/**
 * @brief 读取指定索引的历史记录。
 * @param index 要读取的记录索引（0 ~ MAX_RECORDS-1）
 * @param rec 读取结果存储的结构体指针（FlashRecord*）
 */
void FlashHistory_Read(uint16_t index, FlashRecord *rec);

#endif
