#pragma once

#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include <stdlib.h>


#define SPI_MOSI DDB5
#define SPI_MISO DDB6
#define SPI_SCK DDB7
#define SPI_IOBOARD_SS DDB4
#define SPI_OLED_SS DDB3
#define SPI_CAN_SS DDB1


typedef enum {IOBOARD, OLED, CAN} Slave;


void SPI_master_init();


void SPI_select_slave(Slave slave);

void SPI_write_byte(char cData);

void SPI_read_byte(char* cData); //returner verdi??

void SPI_write(char* buffer, size_t length); //write multiple bytes, array as input?

void SPI_read(char* buffer, size_t length); //read multiple bytes, array as input?

void SPI_release_slaves();

