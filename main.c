#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include <stdlib.h>


#include "UART.h"


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

	SRAM_test();

    /* Replace with your application code */
    while (1) 
    {
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

