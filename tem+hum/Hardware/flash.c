#include "flash.h"
#include "stm32f10x_flash.h"
#include <string.h>

// Flash 中用于存储历史数据的起始地址（STM32F103 最后一页：1KB）
#define FLASH_HISTORY_START_ADDR  ((uint32_t)0x0801FC00)
#define FLASH_PAGE_SIZE           ((uint16_t)0x400)

// 当前写入的历史记录索引
static uint16_t flash_index = 0;

// 初始化 Flash 历史记录模块
void FlashHistory_Init(void) {
    // 读取起始地址的标志位（是否已初始化）
    uint32_t flag = *(uint32_t *)FLASH_HISTORY_START_ADDR;
    if (flag != 0xA5A5A5A5) {
        // 如果未初始化，清空历史记录页
        FlashHistory_Clear();
    } else {
        // 已初始化，查找当前历史记录索引（第一个未写入的位置）
        for (flash_index = 0; flash_index < MAX_RECORDS; flash_index++) {
            FlashRecord rec;
            FlashHistory_Read(flash_index, &rec);
            if (rec.temperature == 0xFFFF && rec.humidity == 0xFFFF) {
                break;
            }
        }
    }
}

// 清空历史记录页，并设置初始化标志
void FlashHistory_Clear(void) {
    FLASH_Unlock();                             // 解锁 Flash 写保护
    FLASH_ErasePage(FLASH_HISTORY_START_ADDR); // 擦除该页
    FLASH_Lock();                               // 上锁

    FLASH_Unlock();
    FLASH_ProgramWord(FLASH_HISTORY_START_ADDR, 0xA5A5A5A5);  // 写入初始化标志
    FLASH_Lock();

    flash_index = 0;  // 重置索引
}

// 写入一条新的温湿度记录
void FlashHistory_Write(uint8_t temp, uint8_t humi) {
    if (flash_index >= MAX_RECORDS) return;  // 已满不再写入

    // 计算写入地址：起始地址 + 标志位（4 字节） + 已有记录长度
    uint32_t addr = FLASH_HISTORY_START_ADDR + 4 + flash_index * sizeof(FlashRecord);
    FlashRecord rec = { temp, humi };

    FLASH_Unlock();
    // Flash 编程只能按 16 位（HalfWord）写入
    uint16_t *pData = (uint16_t *)&rec;
    for (uint32_t i = 0; i < sizeof(FlashRecord) / 2; i++) {
        FLASH_ProgramHalfWord(addr + i * 2, pData[i]);
    }
    FLASH_Lock();

    flash_index++;  // 索引增加
}

// 读取指定索引的历史记录
void FlashHistory_Read(uint16_t index, FlashRecord *rec) {
    if (index >= MAX_RECORDS) return;  // 越界检查
    uint32_t addr = FLASH_HISTORY_START_ADDR + 4 + index * sizeof(FlashRecord);
    memcpy(rec, (void *)addr, sizeof(FlashRecord));
}
