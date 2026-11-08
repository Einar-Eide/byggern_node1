#pragma once

#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "uart.h"

void adc_init();
void adc_analog_read(uint16_t *data);
uint16_t adc_filter_process(uint16_t new_value);
uint8_t is_ir_diode_blocked();