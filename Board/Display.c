#include "stm32f10x.h"
#include "Display.h"
#include "IERG3810_TFTLCD.h"
#include "global.h"

// change the page to the i th page
void to_Page(int i) {
    state = i;
}

// clear the display to the background color
void clear_Display(void) {
    IERG3810_TFTLCD_FillRectangle(bgColor, screenStartX, screenWidth, screenStartY, screenHeight);
}
