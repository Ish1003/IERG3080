#include "stm32f10x.h"
#include "Name_and_SID.h"
#include "PS2.h"
#include "Display.h"
#include "IERG3810_TFTLCD.h"
#include "global.h"
#include "Select_Reset.h"
#include "Timer.h"

//var
char prevResetKey = ' ';
char ResetKey;
u8 isResetEnterReleased = 0;

char Winning[] = "You Win!";
char Losing[] = "You Lose!";
char Restarting[] = "Press enter to restart";

void displayWin(){
		int i;
		IERG3810_TFTLCD_FillRectangle(0x0FF0, 10, 220, 10, 300);
		for (i = 0; i < 8; i++)
			IERG3810_TFTLCD_ShowChar(80 + charWidth * i, 250, Winning[i], 0xFFFF, 0x0FF0);
		for (i = 0; i < 22; i++)
			IERG3810_TFTLCD_ShowChar(25 + charWidth * i, 150, Restarting[i], 0xFFFF, 0x0FF0);
		TIM7_General_Delay(10000);

}

void displayLose(){
		int i;
		IERG3810_TFTLCD_FillRectangle(0xF00F, 10, 220, 10, 300);
		for (i = 0; i < 9; i++)
			IERG3810_TFTLCD_ShowChar(85 + charWidth * i, 250, Losing[i], 0xFFFF, 0xF00F);
		for (i = 0; i < 22; i++)
			IERG3810_TFTLCD_ShowChar(25 + charWidth * i, 150, Restarting[i], 0xFFFF, 0xF00F);
		TIM7_General_Delay(10000);
}

void displayEnd(int end){
    switch (end)
    {
			case 1:
					displayWin();
					break;
			
			case 0:
					displayLose();
					break;
    }
} //1 win 0 lose

void select_restart(void)
{
	// to be changed
	displayEnd(end);
	difficulty = 0;
	isLaunched = 0;
	ResetKey = get_PS2_Key();


	if (!isResetEnterReleased && ResetKey != 'e') {
		isResetEnterReleased = 1;
	}
		
		TIM4_Menu_Timer_Reset();

	if(ResetKey != prevResetKey || !is_TIM4_Menu_Timer_Running()){
		if (ResetKey != prevResetKey) {
			TIM4_Menu_Timer_Start(5000);
		} else {
			TIM4_Menu_Timer_Start(1500);
		}

		prevResetKey = ResetKey;
	}
	if (isResetEnterReleased && ResetKey == 'e') {
		to_Page(1);
		task1HeartBeat = 10;
		isResetEnterReleased = 0;
	}
}
