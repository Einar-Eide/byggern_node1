#include "UART.h"

void uart_init(unsigned int ubrr) {
    
    // Baud rate
    UBRR0H = (MYUBRR >> 8);
    UBRR0L = MYUBRR;

    // Aktiver sending og mottak
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // 8 databiter, 2 stoppbit, ingen paritet
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

    fdevopen(uart_send, uart_read);
}

int uart_send(char data, FILE *stream) {

    // Wait for empty transmit buffer
    while ( !( UCSR0A & (1<<UDRE0)) );
    // Put data into buffer, sends the data
    UDR0 = data;
    
    return 0;
}

int uart_read(FILE *stream){

    // Wait for data to be received 
    while ( !(UCSR0A & (1<<RXC0)) );
    // Get and return received data from buffer
    return UDR0;
}  