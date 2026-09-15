# pragma once

#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include <stdlib.h>

void get_position(volatile uint8_t *joy_x, volatile uint8_t *joy_y, volatile uint8_t *sli_x, volatile uint8_t *sli_y);
void get_direction(volatile uint8_t *joy_x, volatile uint8_t *joy_y, volatile uint8_t *sli_x, volatile uint8_t * sli_y);


void calibrate_joy(uint8_t* joy_x_min, uint8_t* joy_x_max, uint8_t* joy_y_min, uint8_t* joy_y_max);