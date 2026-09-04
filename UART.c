#include "UART.h"



void uart_init(unsigned int ubrr) {
    
    // Baud rate
    UBRR0H = (MYUBRR >> 8);
    UBRR0L = MYUBRR;

    // Aktiver sending og mottak
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // 8 databiter, 1 stoppbit, ingen paritet
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


void uart_send(unsigned char data) {
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = data;
}



char uart_read(void){
    /* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) );
    /* Get and return received data from buffer */
    return UDR0;
}
    