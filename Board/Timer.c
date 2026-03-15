#include "stm32f10x.h"
#include "global.h"

// The TIM2 for delay in PS2 interupt handler
// Initialize the TIM2
void TIM2_PS2_Delay_Init_1mhz(void) {
	// TIM2, IRQ#28
	RCC->APB1ENR 	|= 1 << 0;
	TIM2->PSC		 = 71;
	TIM2->DIER		|= 1 << 0;
	TIM2->CR1		|= 1 << 3;
	NVIC->IP[28]	 = 0x45;
	NVIC->ISER[0]	|= 1 << 28;
}

// interupt handler for TIM2
void TIM2_IRQHandler(void) {
	TIM2->SR &= ~(1 << 0);
}

// Delay by TIM2
void TIM2_PS2_Delay(u16 count) {
	TIM2->CR1	|= 1 << 0;
	TIM2->ARR	 	 = count;

	while (TIM2->CR1 & 1) {}
}

// The TIM3 for timeout counter in PS2 clear procedure
// Initialize the TIM3
void TIM3_PS2_Timer_Init_10khz(void) {
	// TIM3, IRQ#29
	RCC->APB1ENR 	|= 1 << 1;
	TIM3->PSC		 = 7199;
	TIM3->DIER		|= 1 << 0;
	TIM3->CR1		|= 1 << 3;
	NVIC->IP[29]	 = 0x45;
	NVIC->ISER[0]	|= 1 << 29;
}

// interupt handler for TIM3
void TIM3_IRQHandler(void) {
	TIM3->SR &= ~(1 << 0);
}

// Reset TIM3
void TIM3_PS2_Timer_Reset(void) {
	TIM3->CR1	&= 0xFFFFFFFE;
	TIM3->CNT	 = 0;
}

// start TIM3
void TIM3_PS2_Timer_Start(u16 count) {
	TIM3->CR1	|= 1 << 0;
	TIM3->ARR	 = count;

}

// check for TIM3 current status
u8 is_TIM3_PS2_Timer_Running(void) {
	return (TIM3->CR1 & 1);
}

// The TIM4 for button selection delay in menu
// Initialize the TIM4
void TIM4_Menu_Timer_Init_10khz(void) {
	// TIM4, IRQ#30
	RCC->APB1ENR 	|= 1 << 2;
	TIM4->PSC		 = 7199;
	TIM4->DIER		|= 1 << 0;
	TIM4->CR1		|= 1 << 3;
	NVIC->IP[30]	 = 0x45;
	NVIC->ISER[0]	|= 1 << 30;
}

// interupt handler for TIM4
void TIM4_IRQHandler(void) {
	TIM4->SR &= ~(1 << 0);
}

// Reset TIM4
void TIM4_Menu_Timer_Reset(void) {
	TIM4->CR1	&= 0xFFFFFFFE;
	TIM4->CNT	 = 0;
}

// start TIM4
void TIM4_Menu_Timer_Start(u16 count) {
	TIM4->CR1	|= 1 << 0;
	TIM4->ARR	 = count;

}

// check for TIM4 current status
u8 is_TIM4_Menu_Timer_Running(void) {
	return (TIM4->CR1 & 1);
}

// The TIM7 for delay of general purpose
// Initialize the TIM3
void TIM7_General_Delay_Init_1mhz(void) {
	// TIM7, IRQ#55
	RCC->APB1ENR 	|= 1 << 5;
	TIM7->PSC		 = 71;
	TIM7->DIER		|= 1 << 0;
	TIM7->CR1		|= 1 << 3;
	NVIC->IP[55]	 = 0x45;
	NVIC->ISER[1]	|= 1 << 23;
}

// interupt handler for TIM7
void TIM7_IRQHandler(void) {
	TIM7->SR &= ~(1 << 0);
}

// Delay by TIM7
void TIM7_General_Delay(u16 count) {
	TIM7->ARR	 = count;
	TIM7->CR1	|= 1 << 0;

	while (TIM7->CR1 & 1) {}
}