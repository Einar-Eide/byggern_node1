#pragma once


#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
//#include <string.h>

#include "SPI.h"
#include "fonts.h"

#define OLED_DC PORTB2

#define SCREEN_CENTER_COLUMN 64

typedef enum {OLED_CENTER, OLED_LEFT, OLED_RIGHT} textpos;

void OLED_init();
void OLED_reset();
void OLED_home();
void OLED_send_command(char cmd);
void OLED_send_data(char data);
void OLED_clear_dispay();
void OLED_go_to_page(uint8_t line);
void OLED_go_to_column(uint8_t column);
void OLED_go_to(uint8_t column, uint8_t row);

/*
void OLED_clear_line(uint8_t line);*/

size_t strlen(char *str);

void OLED_print_char(char c);
void OLED_printf(char *str);

void OLED_printf_2(char *str, textpos pos);

void OLED_printf_1(char *str, textpos pos, uint8_t line);

void OLED_print_emoji(uint8_t emoji);

void OLED_funny_graphics();