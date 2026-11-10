#pragma once

#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "uart.h"

void encoder_init();
void encoder_read(uint32_t* data);
void encoder_reset();
