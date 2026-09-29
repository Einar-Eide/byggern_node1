# pragma once

#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include <stdlib.h>

typedef enum {JOY_LEFT, JOY_RIGHT, JOY_UP, JOY_DOWN, JOY_NEUTRAL} Dir;

void get_position(volatile uint8_t *joy_x, volatile uint8_t *joy_y, volatile uint8_t *sli_x, volatile uint8_t *sli_y);
void get_direction(volatile uint8_t *joy_x, volatile uint8_t *joy_y, volatile uint8_t *sli_x, volatile uint8_t * sli_y);
void calibrate_joy(uint8_t* joy_x_min, uint8_t* joy_x_max, uint8_t* joy_y_min, uint8_t* joy_y_max);
void get_analog_position(volatile uint8_t joy_x_min, volatile uint8_t joy_x_max, volatile uint8_t joy_y_min, volatile uint8_t joy_y_max);
void get_joy_direction(volatile uint8_t *joy_x, volatile uint8_t *joy_y,  volatile Dir *dir_joy_x, volatile Dir *dir_joy_y);

uint8_t button_down();
uint8_t button_pressed();