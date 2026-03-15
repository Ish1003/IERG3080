#ifndef __LAUNCH_GAME_H
#define __LAUNCH_GAME_H

#include "stm32f10x.h"
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

void launch_game(void);

int row(int difficulty);
int col(int difficulty);
int offset_x(int difficulty);
int offset_y(int difficulty);
int interval_x(int difficulty);
int interval_y(int difficulty);

void generateCards(int difficulty);
void countDown(void);
void shuffleCards(char* cards, int len);
void generate16Cards(void);
void generate24Cards(void);
void generate36Cards(void);
void drawInstructions(void);
void drawTimer(void);
void drawCards(int difficulty);
void flipCard(int x, int y);
int checkCards(int x1, int y1, int x2, int y2);
void cardSelection();

u8 ifStuck_up(char *maskedcards, u8 x, u8 y, int row, int col);
u8 ifStuck_down(char *maskedcards, u8 x, u8 y, int row, int col);
u8 ifStuck_left(char *maskedcards, u8 x, u8 y, int row, int col);
u8 ifStuck_right(char *maskedcards, u8 x, u8 y, int row, int col);

void toUp();
void toDown();
void toLeft();
void toRight();

void launchGame(int difficulty);
void gameController(int difficulty);

#endif