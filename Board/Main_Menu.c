#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"
#include "global.h"
#include "Timer.h"
#include "Main_Menu.h"
#include "PS2.h"
#include "Display.h"
#include "IERG3810_USART.h"

int tempDiff = 0;

char prevMenuKey = ' ';
u8 isEnterReleased = 0;

int menuCharColor = 0xFFFF;

int interval = 10;

int menuStartX;
int menuStartY;

char menuKey;


char diffMsg[26] = "Please Select Difficulties";

int diffOriColor 		= 0x677d;
int diffSelectedColor 	= 0x32f8;

int diffOriCharColor 		= 0x0000;
int diffSelectedCharColor 	= 0xFFFF;

int diffColor;
int diffCharColor;
int diffWidth;
int diffHeight;

int menucharStartX;
int menucharStartY;
char diffName[3][6] = {
	"easy",
	"Normal",
	"hard"
};
int diffNameSize[3] = {4, 6, 4};

void main_menu(void)
{
	// to be changed
	menuKey = get_PS2_Key();

	if (!isEnterReleased && menuKey != 'e') {
		isEnterReleased = 1;
	}

	if(menuKey != prevMenuKey || !is_TIM4_Menu_Timer_Running()) {
		if (menuKey == 'u') {
			tempDiff = (tempDiff + 2) % 3;
		} else if (menuKey == 'd') {
			tempDiff = (tempDiff + 1) % 3;
		}

		
		TIM4_Menu_Timer_Reset();

		if (menuKey != prevMenuKey) {
			TIM4_Menu_Timer_Start(5000);
		} else {
			TIM4_Menu_Timer_Start(1500);
		}

		prevMenuKey = menuKey;
	}		

	diff_Select_Message();
	draw_Difficulty();	

	if (isEnterReleased && menuKey == 'e') {
		to_Page(2);
		difficulty = tempDiff + 1;
		task1HeartBeat = 10;
		isEnterReleased = 0;
	}
}

void diff_Select_Message(void)
{
	int i;

	menuStartX = screenStartX + (screenWidth - (26 * charWidth)) / 2;
	menuStartY = screenStartY + screenHeight - charHeight - interval;
	
	for (i = 0; i < 26; i++)
		IERG3810_TFTLCD_ShowChar(menuStartX + charWidth * i, menuStartY, diffMsg[i], menuCharColor, bgColor);
}

void draw_Difficulty(void)
{
	int i, j;

	menuStartX = screenStartX + interval * 2;

	diffWidth  = screenWidth - ((menuStartX - screenMargin) * 2);
	diffHeight = (screenHeight - (interval * 5) - charHeight) / 3;
	
	menuStartY = screenStartY + interval * 3 + diffHeight * 2;

	//change_update to show the selected -- with a darker background color
	for(i = 0; i < 3; i++){
		if (i != tempDiff) {
			diffColor = diffOriColor;
			diffCharColor = diffOriCharColor;
		} else {
			diffColor = diffSelectedColor;
			diffCharColor = diffSelectedCharColor;
		}

		IERG3810_TFTLCD_FillRectangle(diffColor, menuStartX, diffWidth, menuStartY, diffHeight);

		menucharStartX = menuStartX + ((diffWidth - (charWidth * diffNameSize[i])) / 2);
		menucharStartY = menuStartY + ((diffHeight - charHeight) / 2);

		for(j = 0; j < diffNameSize[i]; j++)
			IERG3810_TFTLCD_ShowChar(menucharStartX + j * charWidth, menucharStartY, diffName[i][j], diffCharColor, diffColor);
		
		menuStartY -= (diffHeight + interval);
	}
}

u8 getDifficulty(void)
{
	// to be changed
	return tempDiff;
}