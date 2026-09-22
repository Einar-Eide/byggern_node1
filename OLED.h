#pragma once


#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include "fonts.h"

#include "SPI.h"

#define OLED_DC PORTB2

void OLED_init();
void OLED_reset();
void OLED_home();
void OLED_send_command(char cmd);
void OLED_send_data(char data);
void OLED_clear_dispay();
void OLED_go_to_line(uint8_t line);
void OLED_set_pixel(char x, char y, char value);
void OLED_go_to_column(uint8_t column);
/*
void OLED_clear_line(uint8_t line);
void OLED_pos(uint8_t row, uint8_t column);*/
void OLED_print_char(char c);
void OLED_printf(char *str);
void OLED_funny_graphics();