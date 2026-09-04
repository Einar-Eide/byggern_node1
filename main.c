#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>

#include "UART.h"


int main(void)
{
		
	// set pin to output
	DDRA = (1<<DDA0);
	

	uart_init( MYUBRR );

	
	
    /* Replace with your application code */
    while (1) 
    {
		PORTA = (1<<0);
		_delay_ms(10);
		PORTA = (0<<0);
		_delay_ms(10);

		uart_send('a');
    }
}

