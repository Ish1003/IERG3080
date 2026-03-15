#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"
#include "global.h"

void TIM2_PS2_Delay_Init_1mhz(void);
void TIM3_PS2_Timer_Init_10khz(void);
void TIM4_Menu_Timer_Init_10khz(void);
void TIM7_General_Delay_Init_1mhz(void);
void TIM2_PS2_Delay(int count);
void TIM3_PS2_Timer_Reset(void);
void TIM3_PS2_Timer_Start(int count);
u8 is_TIM3_PS2_Timer_Running(void);
void TIM4_Menu_Timer_Reset(void);
void TIM4_Menu_Timer_Start(int count);
u8 is_TIM4_Menu_Timer_Running(void);
void TIM7_General_Delay(u16 count);

#endif