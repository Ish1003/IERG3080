#ifndef __PS2_H
#define __PS2_H
#include "stm32f10x.h"
#include "global.h"

void translate_PS2_Key(void);
void clear_PS2_Key(void);
char get_PS2_Key(void);

#endif
