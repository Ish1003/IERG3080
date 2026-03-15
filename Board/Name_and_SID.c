#include "stm32f10x.h"
#include "Name_and_SID.h"
#include "PS2.h"
#include "Display.h"
#include "IERG3810_TFTLCD.h"
#include "global.h"

int nameCharColor = 0xFFFF;

int nameStartX;
int nameStartY;

char prjTitle[] = "mini project";
char gameTitle[] = "Card Flipping Game";

char id[2][10] = {"1155200226", "1155191597"};

char inst[2][30] = {
	{"Please Press Enter to"},
	{"Continue"}
};

void name_and_SID(void)
{
	game_Title();
	author_and_SID();
	Instruction();
	
	if (get_PS2_Key() == 'e') {
		to_Page(1);
		task1HeartBeat = 10;
	}
}

void game_Title(void) {
	int i;
	
	nameStartX = screenStartX + (screenWidth - (12 * charWidth)) / 2;
	nameStartY = screenStartY + screenHeight / 3 * 2 - charWidth;
	
	for (i = 0; i < 12; i++)
		IERG3810_TFTLCD_ShowChar(nameStartX + charWidth * i, nameStartY + charHeight, prjTitle[i], nameCharColor, bgColor);

	nameStartX = screenStartX + (screenWidth - (18 * charWidth)) / 2;

	for (i = 0; i < 18; i++)
		IERG3810_TFTLCD_ShowChar(nameStartX + charWidth * i, nameStartY, gameTitle[i], nameCharColor, bgColor);
}

void author_and_SID(void) {
	int i, j; 
	
	nameStartX = screenStartX + (screenWidth - (3 * chinWidth) - (11 * charWidth)) / 2;
	nameStartY = screenStartY + screenHeight / 3 - charWidth;
	
	for(j = 0; j < 2; j++) {
		for (i = 0; i < 3; i++)
			IERG3810_TFTLCD_ShowChinChar(nameStartX + chinWidth * i, nameStartY + chinHeight * j, 3 * j + i, nameCharColor, bgColor);
		IERG3810_TFTLCD_ShowChar(nameStartX + chinWidth * 3, nameStartY + charHeight * j, ' ', nameCharColor, bgColor);
		for (i = 0; i < 10; i++)
			IERG3810_TFTLCD_ShowChar(nameStartX + chinWidth * 3 + charWidth * (i + 1), nameStartY + charHeight * j, id[j][i], nameCharColor, bgColor);
	}
}

void Instruction(void) {
	int i; 
	
	nameStartX = screenStartX + (screenWidth - (21 * charWidth)) / 2;
	nameStartY = screenStartY + charHeight;
	
	for (i = 0; i < 24; i++)
		IERG3810_TFTLCD_ShowChar(nameStartX + charWidth * i, nameStartY, inst[0][i], nameCharColor, bgColor);

	nameStartX = screenStartX + (screenWidth - (8 * charWidth)) / 2;
	nameStartY = screenStartY;

	for (i = 0; i < 8; i++)
		IERG3810_TFTLCD_ShowChar(nameStartX + charWidth * i, nameStartY, inst[1][i], nameCharColor, bgColor);
}
