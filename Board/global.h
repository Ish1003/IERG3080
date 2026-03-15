#ifndef __GLOBAL_H
#define __GLOBAL_H
#include "stm32f10x.h"

// General Display Information
extern int screenMargin;
extern int screenStartX;
extern int screenStartY;
extern int screenWidth;
extern int screenHeight;
extern int charWidth;
extern int charHeight;
extern int chinWidth;
extern int chinHeight;
extern u8 bgColor;

extern u8 task1HeartBeat;

extern u8 flipEnable;
extern u8 flipHeartBeat;

extern u8 timerEnable;
extern u8 timerHeartBeat;


extern u8 gameEnable;
extern u8 gameHeartBeat;

// Current Pages
extern u8 state;
extern int difficulty;

// PS2 Key Pad
extern u8 ps2count;
extern u16 ps2data;

extern int difficulty;
extern u8 isLaunched;

// others
extern int cnt;
extern int numCards;
extern int random;
extern char temp;
extern char all_cards[18];
extern char cards[36]; //real cards order
extern char cards_masked[36]; // value indicates existence, 1=exist

extern int ifend;
extern int end;

extern int timepass;
extern u8 isGameEnterReleased;

extern u8 x_temp;
extern u8 y_temp;
extern u8 select;
extern u8 x1;
extern u8 y1;
extern u8 x2;
extern u8 y2;

#endif
