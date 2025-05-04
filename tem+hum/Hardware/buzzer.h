#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void buzzer_Init(void);
void buzzer_on(void);
void buzzer_off(void);

#endif
