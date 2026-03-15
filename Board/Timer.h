#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"
#include "global.h"

void TIM2_PS2_Delay_Init_1mhz(void);
void TIM2_PS2_Delay(int count);
void TIM3_PS2_Timer_Init_10khz(void);
void TIM3_PS2_Timer_Reset(void);
void TIM3_PS2_Timer_Start(int count);
u8 is_TIM3_PS2_Timer_Running(void);
void TIM4_Page_Timer_Init_10khz(void);
void TIM4_Page_Timer_Reset(void);
void TIM4_Page_Timer_Start(int count);
u8 is_TIM4_Page_Timer_Running(void);
void TIM5_Random_Generate_Init_72mhz(void);
int TIM5_Random(u16 max);
void TIM6_Game_Timer_Init_10khz(void);
void TIM6_Game_Timer_Reset(void);
void TIM6_Game_Timer_Start(int count);
u8 is_TIM6_Game_Timer_Running(void);
void TIM7_General_Delay_Init_10mkz(void);
void TIM7_General_Delay(u16 count);

#endif