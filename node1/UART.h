#pragma once

#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 

#define BAUD 9600
#define MYUBRR (F_CPU/16)/BAUD - 1


void uart_init(unsigned int ubrr);
    
int uart_send(char data, FILE *stream);
int uart_read(FILE *stream);