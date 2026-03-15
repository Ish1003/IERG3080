#ifndef __SELECT_RESET_H
#define __SELECT_RESET_H
#include "stm32f10x.h"
#include "Name_and_SID.h"
#include "PS2.h"
#include "Display.h"
#include "IERG3810_TFTLCD.h"
#include "global.h"
#include "Timer.h"

void select_restart(void);

void displayWin(void);

void displayLose(void);

void displayEnd(int end);

#endif