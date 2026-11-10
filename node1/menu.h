#pragma once 

#include "OLED.h"
#include "IO_inputs.h"
#include <util/delay.h>

typedef enum {MENU, SERINE, EINAR, LEA, GAME_JOY_SPEED, GAME_JOY_POS, OPT5,} MENU_state;

void MENU_update();
void MENU_draw();
void MENU_scroll();



struct {

    uint8_t game_mode;          // MENU = 0, GAME (EASY) = 1, GAME (HARD) = 2
    uint8_t lives_left;

} typedef Game_state;


extern Game_state* g_p_game_state;

void game_joy_speed_init();
void game_joy_speed();