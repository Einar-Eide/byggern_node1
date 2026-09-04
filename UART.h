# pragma once

#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR (F_CPU/16)/BAUD - 1


void uart_init(unsigned int ubrr);
    
void uart_send(unsigned char data);
char uart_read();
