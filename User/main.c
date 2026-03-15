#include "stm32f10x.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_USART.h"
#include "IERG3810_NVIC.h"
#include "IERG3810_LED.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_Clock.h"

#include "Name_and_SID.h"
#include "Main_Menu.h"
#include "PS2.h"
#include "Display.h"
#include "Timer.h"
#include "Select_Reset.h"
#include "Launch_Game.h"

int screenMargin	= 10;
int screenStartX	= 10;
int screenStartY	= 10;
int screenWidth 	= 220;
int screenHeight	= 300;
int charWidth 		= 8;
int charHeight 		= 16;
int chinWidth 		= 16;
int chinHeight 		= 16;

u8 bgColor = 0x0000;

u8 state=0;
int difficulty = 1;
u8 task1HeartBeat;
u8 gameEnable;
u8 gameHeartBeat;
u8 timerEnable;
u8 timerHeartBeat;


u8 isLaunched = 0;

u8 ps2count = 0;
u16 ps2data	= 0;
u32 timeout = 0;

void IERG3810_SYSTICK_Init10ms(void);

//in main function. also some global vars
char curKey = ' ';
char prevKey = ' ';
u8 prevState = 100;
int cnt=0;
int numCards = 36;
int random;
char temp;
char all_cards[18] = "123456789ABCDEFGHI";
char cards[36]; //real cards order
char cards_masked[36]; // value indicates existence, 1=exist

int end=0;
u8 x_temp=0;
u8 y_temp=0;
u8 select=0;
u8 x1 = -1;
u8 y1 = -1;
u8 x2 = -1;
u8 y2 = -1;
int ifend=0;
int timepass=-2;
u8 isGameEnterReleased = 0;

int main(void)
{
	IERG3810_SYSTICK_Init10ms();
	IERG3810_clock_tree_Init();
	IERG3810_USART1_Init(72, 9600);
	IERG3810_Buzzer_Init();
	IERG3810_TFTLCD_Init();
	IERG3810_LED_Init();
	IERG3810_NVIC_SetPriorityGroup(5);

	TIM2_PS2_Delay_Init_1mhz();
	TIM3_PS2_Timer_Init_10khz();
	TIM4_Page_Timer_Init_10khz();
	TIM5_Random_Generate_Init_72mhz();
	TIM6_Game_Timer_Init_10khz();
	TIM7_General_Delay_Init_10khz();
	IERG3810_PS2key_ExtiInit();


	while (1)
	{
		cnt = cnt +1;

		if(get_PS2_Key() != ' ' && !is_TIM3_PS2_Timer_Running()) {
			clear_PS2_Key();

			USART_print(1, "|c|");
		}

		if (ps2count > 10) {
			translate_PS2_Key();

			USART_print_char(1, get_PS2_Key());
			
			curKey = get_PS2_Key();
			
			TIM3_PS2_Timer_Reset();
			if (prevKey != curKey)
				TIM3_PS2_Timer_Start(5300);
			else
				TIM3_PS2_Timer_Start(2000);

			prevKey = curKey;
			task1HeartBeat = 10;
			//EXTI->IMR &= ~(1 << 11);
		
		}


		if(task1HeartBeat>=10)
		{
			//Delay(5000);
			task1HeartBeat=0;

			if (prevState != state) {
				clear_Display();
				prevState = state;
			}
			
			switch (state)
			{
				case 0:
					name_and_SID();
					break;
				case 1:
					main_menu();
					break;
				case 2:
					launch_game();
					break;
				case 3:
					select_restart();
					break;
			}
		}
	}
}

void IERG3810_SYSTICK_Init10ms(void)
{
	//systick
	SysTick->CTRL = 0; //clear
	SysTick->LOAD = 720000/8 - 1; // 72 MHz = 72000000; 1 ms then 1000 Hz, and then /8
	// CLKSOURCE = 0: STCLK (FCLK/8)
	// clock tree refers, pend handler
	SysTick->CTRL |= 0x03; // to be modified
	//set internal clk, use interrupt, start count
}

void EXTI2_IRQHandler(void)
{
	isGameEnterReleased = 0;
	x_temp=0;
	y_temp=0;
	select=0;
	x1 = -1;
	y1 = -1;
	x2 = -1;
	y2 = -1;
	timepass=0;
	to_Page(3);
	
	EXTI->PR = 1 << 2;
}
