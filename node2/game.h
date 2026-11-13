#pragma once

#define F_CPU 84000000UL
#define TICKS_PER_MS (F_CPU / 1000UL) // 84,000 cycles per millisecond
#define GAME_LIVES 5

#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"

typedef enum {MENU, SPEED, POSITION} Game_Mode;

bool is_game_over();
uint8_t game_reset();
void lose_life(uint8_t *lives);