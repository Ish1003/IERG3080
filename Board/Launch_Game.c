#include "stm32f10x.h"
#include "Launch_Game.h"
#include "IERG3810_TFTLCD.h"
#include "global.h"
#include "Timer.h"
#include "Main_Menu.h"
#include "PS2.h"
#include "Display.h"
#include "IERG3810_USART.h"
#include "IERG3810_LED.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_Clock.h"


int i, j;

int timescale;
int cards_left=0;

char Ins_Up[] = "press 8 to go up";
char Ins_Down[] = "press 2 to go down";
char Ins_Left[] = "press 4 to go left";
char Ins_Right[] = "press 6 to go right";
char Ins_Slc[] = "press enter to select";

u8 ctl_flag;
u8 x_searchL;
u8 x_searchR;
u8 y_searchU;
u8 y_searchD;

char prevGameKey = ' ';
char gameKey;


void launch_game(void)
{
	// to be changed
	//IERG3810_TFTLCD_FillRectangle(0x070F, 0, 150, 100, 50);
	launchGame(difficulty);
}


void shuffleCards(char* cards, int len) {
    // Shuffle the array
	for (i = 0; i < len; i++)
	{
		random = (TIM5_Random(len) + random) % len;
		temp = cards[i];
		cards[i] = cards[random];
		cards[random] = temp;
	}
}

void generate16Cards()
{
		shuffleCards(all_cards, 18);
    for(i=0; i<8; i++)
    {
        cards[i] = all_cards[i];
        cards[i+8] = all_cards[i];
				cards_masked[i] = 1;
				cards_masked[i+8] = 1;
    }
		shuffleCards(cards, 16);
}

void generate24Cards()
{
    shuffleCards(all_cards, 18);
    for(i=0; i<12; i++)
    {
        cards[i] = all_cards[i];
        cards[i+12] = all_cards[i];
				cards_masked[i] = 1;
				cards_masked[i+12] = 1;
    }
    shuffleCards(cards, 24);
}

void generate36Cards()
{
    shuffleCards(all_cards, 18);
    for(i=0; i<18; i++)
    {
        cards[i] = all_cards[i];
        cards[i+18] = all_cards[i];
				cards_masked[i] = 1;
				cards_masked[i+18] = 1;
    }
    shuffleCards(cards, 36);
}
 
void generateCards(int difficulty)
{
    switch (difficulty)
    {
        case 1:
            generate16Cards();
            break;
        
        case 2:
            generate24Cards();
            break;
        
        case 3:
            generate36Cards();
            break;
        
        default:
            break;
    }
}
void drawInstructions(void)
{
	IERG3810_TFTLCD_FillRectangle(0x0000, 10, 220, 10, 300);
	for (i = 0; i < 16; i++)
			IERG3810_TFTLCD_ShowChar(55 + charWidth * i, 250, Ins_Up[i], 0xFFFF, 0x0000);
	for (i = 0; i < 18; i++)
			IERG3810_TFTLCD_ShowChar(50 + charWidth * i, 200, Ins_Down[i], 0xFFFF, 0x0000);
	for (i = 0; i < 18; i++)
			IERG3810_TFTLCD_ShowChar(50 + charWidth * i, 150, Ins_Left[i], 0xFFFF, 0x0000);
	for (i = 0; i < 19; i++)
			IERG3810_TFTLCD_ShowChar(48 + charWidth * i, 100, Ins_Right[i], 0xFFFF, 0x0000);
	for (i = 0; i < 21; i++)
			IERG3810_TFTLCD_ShowChar(43 + charWidth * i, 50, Ins_Slc[i], 0xFFFF, 0x0000);
	TIM7_General_Delay(1);
	TIM7_General_Delay(29999);
	IERG3810_TFTLCD_FillRectangle(0x0000, 10, 220, 10, 300);
}
void drawTimer(){
   
	// this is a task, which refreshes every 50ms or sth. use heartbeatTimer
	IERG3810_TFTLCD_FillRectangle(0x07E0, 10, 220 - timepass, 300, 10);
	
	if(difficulty == 1)
		timescale = 2000;
	else if (difficulty == 2)
		timescale = 3000;
	else if (difficulty == 3)
		timescale = 4000;
	if(timepass<220)
	{
		if(!is_TIM6_Game_Timer_Running())
		{
			IERG3810_TFTLCD_FillRectangle(0x07E0, 10, 220 - timepass, 300, 10);
			IERG3810_TFTLCD_FillRectangle(0x0000, 230 - timepass, timepass, 300, 10);
			timepass+=2;
			TIM6_Game_Timer_Start(timescale );

			cards_left=0;
			for(i=0;i<row(difficulty)*col(difficulty);i++)
			{
				if(cards_masked[i] == 1)
					cards_left++;
			}
			if(timepass > 200)
			{
				GPIOB->ODR ^= 1 << 8;
				GPIOB->ODR ^= 1 << 5;
			}
			if(cards_left<=2)
			{		
				GPIOB->BRR = 1 << 8;
				GPIOB->BSRR |= 1 << 5;
				end=1;
				isGameEnterReleased = 0;
				x_temp=0;
				y_temp=0;
				select=-1;
				x1 = -1;
				y1 = -1;
				x2 = -1;
				y2 = -1;
				timepass=0;
				to_Page(3);
			}
		}
	}
	else
	{	
		GPIOB->BRR = 1 << 8;
		GPIOB->BSRR |= 1 << 5;
		end=0;
		isGameEnterReleased = 0;
		x_temp=0;
		y_temp=0;
		select=-1;
		x1 = -1;
		y1 = -1;
		x2 = -1;
		y2 = -1;
		timepass=0;
		to_Page(3);
	}
}



int row(int difficulty)
{
	switch (difficulty)
	{
			case 1:
					return 4;
			
			case 2:
					return 4;
			
			case 3:
					return 6;
			
			default:
					break;
	}
}

int col(int difficulty)
{
	switch (difficulty)
	{
			case 1:
					return 4;
			
			case 2:
					return 6;
			
			case 3:
					return 6;
			
			default:
					break;
	}
}

int offset_x(int difficulty)
{	
	switch (difficulty)
	{
			case 1:
					return 45;
			
			case 2:
					return 40;
			
			case 3:
					return 30;
			
			default:
					break;
	}
}
int offset_y(int difficulty)
{
	switch (difficulty)
	{
			case 1:
					return 80;
			
			case 2:
					return 50;
			
			case 3:
					return 50;
			
			default:
					break;
	}
}
int interval_x(int difficulty)
{
	switch (difficulty)
	{
			case 1:
					return 45;
			
			case 2:
					return 40;
			
			case 3:
					return 30;
			
			default:
					break;
	}
}
int interval_y(int difficulty)
{
	switch (difficulty)
	{
			case 1:
					return 50;
			
			case 2:
					return 35;
			
			case 3:
					return 35;
			
			default:
					break;
	}
}

void drawCards(int difficulty){
    ;
	// to be changed
	// example of 16 cards
	//IERG3810_TFTLCD_FillRectangle(0x0000, 10, 220, 10, 300);
	if(ifend == 0)
	{
		if(gameHeartBeat >=20)
		{
			gameHeartBeat = 0;
			
			for (i = 0 ; i < row(difficulty); i++)
			{
				for (j = 0 ; j < col(difficulty); j++)
				{
					if(cards_masked[i+j*row(difficulty)])
					{	
						if(i==x_temp && j==y_temp)
							IERG3810_TFTLCD_ShowChar(offset_x(difficulty) + i*interval_x(difficulty), offset_y(difficulty) + j*interval_x(difficulty), cards[i+j*row(difficulty)], 0xFFFF, 0xFFFF);
						//select
						else if(i == x1 && j==y1)
						{	
							IERG3810_TFTLCD_ShowChar(offset_x(difficulty) + i*interval_x(difficulty), offset_y(difficulty) + j*interval_x(difficulty), cards[i+j*row(difficulty)], 0xFFFF, 0xF00F);
						}
						else if(i == x2 && j==y2)
						{	
							IERG3810_TFTLCD_ShowChar(offset_x(difficulty) + i*interval_x(difficulty), offset_y(difficulty) + j*interval_x(difficulty), cards[i+j*row(difficulty)], 0x0000, 0xFFE0);
						}
						else
							IERG3810_TFTLCD_ShowChar(offset_x(difficulty) + i*interval_x(difficulty), offset_y(difficulty) + j*interval_x(difficulty), cards[i+j*row(difficulty)], 0x001F, 0x001F);
						//to be replaced with actual functions
					}
					else
					{
						IERG3810_TFTLCD_ShowChar(offset_x(difficulty) + i*interval_x(difficulty), offset_y(difficulty) + j*interval_x(difficulty), cards[i+j*row(difficulty)], 0x0000, 0x0000);
					}
				}
			}
		}
	}
}

void flipCard(int x, int y){
	//IF SELECT = 0, select 1st card, else 2nd card
	if(select)
	{
		x2 = x;
		y2 = y;
		//determine if match
		IERG3810_TFTLCD_ShowChar(offset_x(difficulty) + x2*interval_x(difficulty), offset_y(difficulty) + y2*interval_x(difficulty), cards[x2+y2*row(difficulty)], 0x0000, 0xFFE0);
		TIM7_General_Delay(10000);
		if(checkCards(x1, y1, x2, y2) && (x1 != x2 || y1 != y2))
		{
			//clear and update
			cards_masked[x1+row(difficulty)*y1]=0;
			cards_masked[x2+row(difficulty)*y2]=0;
			//move cursor to another card, priority: x first.
			while(cards_masked[(x_temp)+row(difficulty)*((y_temp) % col(difficulty))]==0)
			{
				y_temp = (y_temp + 1) % col(difficulty); // move 1 on y axis
				//for each x
				for(i=0;i<row(difficulty); i++)
				{
					if(cards_masked[(x_temp)+row(difficulty)*((y_temp) % col(difficulty))]==0)
						x_temp = (x_temp + 1) % row(difficulty);
				}
			}
			
		}
		x1 = -1;
		y1 = -1;
		x2 = -1;
		y2 = -1;
	}
	else
	{
		x1 = x;
		y1 = y;
		IERG3810_TFTLCD_ShowChar(offset_x(difficulty) + x1*interval_x(difficulty), offset_y(difficulty) + y1*interval_x(difficulty), cards[x1+y1*row(difficulty)], 0x0000, 0xF800);
		TIM7_General_Delay(10000);
	}
	//drawCards(difficulty);
	select = 1 - select;
}

int checkCards(int x1, int y1, int x2, int y2)
{
	//to be selected;
	if(cards[x1 + y1 *row(difficulty)] == cards[x2 + y2 * row(difficulty)])
		return 1;
	else
		return 0;
}


u8 ifStuck_up(char *maskedcards, u8 x, u8 y, int row, int col)
{
	//no others on row and col;
	int j;
	int flag = 1;
	if(y==col - 1)
		return 1;
	for(j=y+1;j<col;j++)
	{
		if(maskedcards[x + row * j] == 1)
			flag = 0;
	}
	return flag;
}

u8 ifStuck_down(char *maskedcards, u8 x, u8 y, int row, int col)
{
	//no others on row and col;
	int j;
	int flag = 1;
	if(y == 0)
		return 1;
	for(j=0 ;j < y; j++)
	{
		if(maskedcards[x + row * j] == 1)
			flag = 0;
	}
	return flag;
}

u8 ifStuck_left(char *maskedcards, u8 x, u8 y, int row, int col)
{
	//no others on row and col;
	int i;
	int flag = 1;
	if(x ==0)
		return 1;
	for(i=0;i < x;i++)
	{
		if(maskedcards[i + row * y] == 1)
			flag = 0;
	}
	
	return flag;
}

u8 ifStuck_right(char *maskedcards, u8 x, u8 y, int row, int col)
{
	//no others on row and col;
	int i;
	int flag = 1;
	if(x==row - 1)
		return 1;
	for(i=x+1;i<row;i++)
	{
		if(maskedcards[i + row * y] == 1)
			flag = 0;
	}
	return flag;
}

void toUp()
{
			while(cards_masked[x_temp+row(difficulty)*((y_temp + 1) % col(difficulty))]==0)
			{
				y_temp = (y_temp + 1) % col(difficulty);
				
			}
			y_temp = (y_temp + 1) % col(difficulty);
}
void toDown()
{
		while(cards_masked[x_temp+row(difficulty)*((y_temp + col(difficulty) - 1) % col(difficulty))]==0)
		{
			y_temp = (y_temp + col(difficulty) - 1) % col(difficulty);
			
		}
		y_temp = (y_temp + col(difficulty) - 1) % col(difficulty);
}

void toLeft()
{
			while(cards_masked[((x_temp + row(difficulty) - 1) % row(difficulty))+row(difficulty)*(y_temp)]==0)
			{
				x_temp = (x_temp + row(difficulty) - 1) % row(difficulty);
			}
			x_temp = (x_temp + row(difficulty) - 1) % row(difficulty);
}

void toRight()
{	
			while(cards_masked[((x_temp + 1) % row(difficulty))+row(difficulty)*(y_temp)]==0)
			{
				x_temp = (x_temp + 1) % row(difficulty);
			}
			x_temp = (x_temp + 1) % row(difficulty);
}

void gameController(int difficulty)
{
	gameKey = get_PS2_Key();

	if (!isGameEnterReleased && gameKey != 'e') {
		isGameEnterReleased = 1;
	}

	if(gameKey != prevGameKey || !is_TIM4_Page_Timer_Running()) {
		if (gameKey == 'u') {
			if (ifStuck_up(cards_masked, x_temp, y_temp, row(difficulty), col(difficulty)))
			{
				//simply no value up
				x_searchL = x_temp;
				y_searchU = y_temp;
				x_searchR = x_temp;
				y_searchD = y_temp;
				ctl_flag = 0;
				for(i= 0; i< row(difficulty)*2; i++)
				{
					if(x_searchR != row(difficulty) - 1)
						x_searchR = (x_searchR + 1) % row(difficulty);
					if(!ifStuck_up(cards_masked, x_searchR, y_searchU, row(difficulty), col(difficulty)))
					{	
						ctl_flag=1;
						x_temp = x_searchR;
						//x_temp = i;
						toUp();
						break;
					}
					x_searchL = (x_searchL + row(difficulty) - 1) % row(difficulty);
					if(!ifStuck_up(cards_masked, x_searchL, y_searchD, row(difficulty), col(difficulty)))
					{	
						ctl_flag=1;
						x_temp = x_searchL;
						//x_temp = i;
						toUp();
						break;
					}
				}

			}
			else
				toUp();			
		}
		else if (gameKey == 'd') {
			if (ifStuck_down(cards_masked, x_temp, y_temp, row(difficulty), col(difficulty)))
			{
				x_searchL = x_temp;
				y_searchU = y_temp;
				x_searchR = x_temp;
				y_searchD = y_temp;
				ctl_flag = 0;
				for(i= 0; i< row(difficulty)*2 ; i++)
				{
					if(x_searchR != row(difficulty) - 1)
						x_searchR = (x_searchR + 1) % row(difficulty);
					if(!ifStuck_down(cards_masked, x_searchR, y_searchU, row(difficulty), col(difficulty)))
					{	
						ctl_flag=1;
						x_temp = x_searchR;
						//x_temp = i;
						toDown();
						break;
					}
					x_searchL = (x_searchL + row(difficulty) - 1) % row(difficulty);
					if(!ifStuck_down(cards_masked, x_searchL, y_searchD, row(difficulty), col(difficulty)))
					{	
						ctl_flag=1;
						x_temp = x_searchL;
						//x_temp = i;
						toDown();
						break;
					}
				}

			}
				else
					toDown();
		} 
		else if (gameKey == 'l') {
			if (ifStuck_left(cards_masked, x_temp, y_temp, row(difficulty), col(difficulty)))
			{
				x_searchL = x_temp;
				y_searchU = y_temp;
				x_searchR = x_temp;
				y_searchD = y_temp;
				ctl_flag = 0;
				for(i= 0; i< col(difficulty)*2; i++)
				{
					//if(y_searchU != col(difficulty) - 1)
						y_searchU = (y_searchU + 1) % col(difficulty);
					if(!ifStuck_left(cards_masked, x_searchL, y_searchU, row(difficulty), col(difficulty)))
					{	
						ctl_flag=1;
						y_temp = y_searchU;
						//y_temp = i;
						toLeft();
						break;
					}
					y_searchD = (y_searchD + col(difficulty) - 1) % col(difficulty);
					if(!ifStuck_left(cards_masked, x_searchL, y_searchD, row(difficulty), col(difficulty)))
					{	
						ctl_flag=1;
						y_temp = y_searchD;
						//y_temp = i;
						toLeft();
						break;
					}
				}
			}
			else
				toLeft();
		}
		else if (gameKey == 'r') {
			if (ifStuck_right(cards_masked, x_temp, y_temp, row(difficulty), col(difficulty)))
			{
				x_searchL = x_temp;
				y_searchU = y_temp;
				x_searchR = x_temp;
				y_searchD = y_temp;
				ctl_flag = 0;
				for(i= 0; i< col(difficulty)*2; i++)
				{
					if(y_searchU != col(difficulty) - 1)
						y_searchU = (y_searchU + 1) % col(difficulty);
					if(!ifStuck_right(cards_masked, x_searchL, y_searchU, row(difficulty), col(difficulty)))
					{	
						ctl_flag=1;
						y_temp = y_searchU;
						//y_temp = i;
						toRight();
						break;
					}
					y_searchD = (y_searchD + col(difficulty) - 1) % col(difficulty);
					if(!ifStuck_right(cards_masked, x_searchL, y_searchD, row(difficulty), col(difficulty)))
					{	
						ctl_flag=1;
						y_temp = y_searchD;
						//y_temp = i;
						toRight();
						break;
					}
				}
			}
			else
				toRight();
		}
			
		
		TIM4_Page_Timer_Reset();

		if (gameKey != prevGameKey) {
			TIM4_Page_Timer_Start(5000);
		} else {
			TIM4_Page_Timer_Start(1500);
		}

		prevGameKey = gameKey;
	}		

	if (isGameEnterReleased && gameKey == 'e') {
		flipCard(x_temp, y_temp);
		task1HeartBeat = 10;
	}

}

void launchGame(int difficulty)
{
    if(isLaunched == 0)
		{
			drawInstructions();
			generateCards(difficulty);
			isLaunched = 1;
		}
		timerEnable = 1;
		gameEnable = 1;
		gameController(difficulty);
    drawTimer();
    drawCards(difficulty);
}
