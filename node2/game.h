#pragma once

#define F_CPU 84000000UL
#define TICKS_PER_MS (F_CPU / 1000UL) // 84,000 cycles per millisecond

#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"

void play_pong();
void during_game();
bool is_game_over();
void reset_game();
uint32_t millis(void);