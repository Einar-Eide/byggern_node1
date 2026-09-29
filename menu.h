
#include "OLED.h"
#include "IO_inputs.h"



typedef enum {MENU, SERINE, EINAR, LEA, OPT3, OPT4, OPT5, OPT6, OPT7} MENU_state;


void MENU_init();

void MENU_update();

void MENU_draw();

void MENU_scroll(uint8_t *pos, volatile uint8_t *joy_x, volatile uint8_t *joy_y,  volatile Dir *dir_joy_x, volatile Dir *dir_joy_y);







