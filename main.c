#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include <stdlib.h>


#include "UART.h"
#include "ADC.h"
#include "SPI.h"


void SRAM_test(void)
{
    volatile char *ext_ram = (char *)0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;

    printf("Starting SRAM test...\n\r");

    // rand() stores some internal state, so calling this function in a loop
    // will yield different seeds each time (unless srand() is called before this
    // function)
    uint16_t seed = rand();

    // Write phase: Immediately check that the correct value was stored
    srand(seed);

    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];

        if (retreived_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r",
                   i, retreived_value, some_value);
            write_errors++;
        }
    }

    // Retrieval phase: Check that no values were changed during or after the
    // write phase
    srand(seed);

    // Reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];

        if (retreived_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r",
                   i, retreived_value, some_value);
            retrieval_errors++;
        }
    }

    printf("SRAM test completed with\n%4d errors in write phase and\n%4d errors in retrieval phase\n\r\n\r",
           write_errors, retrieval_errors);
}


int main(void) {
	
	// set pin to output
	/*
	DDRA = 0xFF;
	DDRE = (1<<DDE1);

	//volatile char *ext_ram = (char *) 0x1800; 
	MCUCR = (1<<SRE);
	PORTA = 0b01010101;
	PORTE = (0<<PORTE1);
	_delay_ms(50);
	PORTE = (1<<PORTE1);
	

	*/
	uart_init( MYUBRR );
	MCUCR |= (1<<SRE);

	
	// Sette port PD4 til output
	DDRD = (1 << DDD4);

    // set counter reset value
    OCR3AL = 1;

	// config timer
    TCCR3A = (1<<COM3A0);
    TCCR3B = (1<<CS30) | (1<<WGM32);

    
    volatile uint8_t joy_x_min;
    volatile uint8_t joy_x_max;
    volatile uint8_t joy_y_min;
    volatile uint8_t joy_y_max;

    //calibrate_joy(&joy_x_min, &joy_x_max, &joy_y_min, &joy_y_max);

    printf("Finnished calibration of joystick!\n\r");
    
    printf("joy_x min: %d\t\max: %d\n\r", joy_x_min, joy_x_max);
    printf("joy_y min: %d\t\max: %d\n\r", joy_y_min, joy_y_max);
    

	SRAM_test();

    char testbyte = 0x2b;

    SPI_master_init();
    

    /* Replace with your application code */
    while (1) 
    {

        uint8_t x = 0;
        uint8_t y = 0;

        uint8_t x1 = 0;
        uint8_t y1 = 0;

        //get_position(&x, &y, &x1, &y1);
        //printf("x-positions: %d\t%d", x, y);
        //get_direction(&x, &y, &x1, &y1);

        //get_analog_position(joy_x_min, joy_x_max, joy_y_min, joy_y_max);
        
        SPI_write_byte(testbyte);
        _delay_us(10);

		/*PORTA = (1<<0);
		_delay_ms(10);
		PORTA = (0<<0);
		_delay_ms(10);*/

		// uart_send('a');
		//char X = uart_read();
		//uart_send(X);

		//printf("yey!\n\r");

		//volatile char *ext_ram = (char *) 0x1800; 


    }
}

