#include "stm32f10x.h"
#include "PS2.h"
#include "Timer.h"
#include "global.h"

u16 ps2TempData = 0;
char ps2TempKey = ' ';

char currentKey = ' ';

// translate the PS2 data to the key and place it into the stack
void translate_PS2_Key(void) {
	
	ps2TempData = (ps2data >> 1) & 0xFF;
	ps2data = 0;
	ps2count = 0;
	
	switch (ps2TempData) {
		case 0x5A:
			currentKey = 'e';
			return;
		case 0x72 :
			currentKey = 'd';
			return;
		case 0x6B :
			currentKey = 'l';
			return;
		case 0x74 :
			currentKey = 'r';
			return;
		case 0x75 :
			currentKey = 'u';
			return;
		default :
			return;
	}
}

// clear the PS2 Key read in buffer
void clear_PS2_Key(void) {
	currentKey = ' ';
}

// return the last pressed PS2 key
char get_PS2_Key(void) {
	return currentKey;
}

// Interrupt handler for PS/2 Keyboard
void EXTI15_10_IRQHandler(void) {
	// read the input from PC10 which is the data pin
	// of the PS/2 Keyboard
	ps2data |= (GPIOC->IDR & 0x400) >> (10 - ps2count);
	ps2count++;
	
	TIM2_PS2_Delay(10);
	EXTI->PR 	=  (1 << 11);	// clear this exception pending bit
}

