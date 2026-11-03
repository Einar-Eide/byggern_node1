#pragma once

#define F_CPU 84000000UL
#define TICKS_PER_MS (F_CPU / 1000UL) // 84,000 cycles per millisecond

#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"


/**
 * increases points when called
 * retuns current score 
 */
uint8_t increase_points();

void play_pong();

uint32_t millis(void);