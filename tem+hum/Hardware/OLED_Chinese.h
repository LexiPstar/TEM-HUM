#ifndef __OLED_CHINESE_H
#define __OLED_CHINESE_H

#include <stdint.h>

void OLED_ShowChinese(uint8_t x, uint8_t y, const uint8_t *font);
void OLED_ShowChineseString(uint8_t x, uint8_t y, const char *text);

#endif
