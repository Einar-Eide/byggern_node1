#include "SPI.h"





void SPI_master_init(){
    
    //Configure ss pins B1, B3 & B4 as outputs
    DDRB = (1<<SPI_MOSI) | (1<<SPI_SCK) | (1 << SPI_IOBOARD_SS) | (1 << SPI_OLED_SS) | (1 << SPI_CAN_SS);

    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

    SPI_release_slaves();


}


void SPI_select_slave(Slave slave){

    SPI_release_slaves();

    switch(slave)
    {
    case IOBOARD:

        //set pin PB4 low
        PORTB &= ~(1<<PORTB4);
        break;
    
    case OLED:

        //pin PB3 low
        PORTB &= ~(1<<PORTB3);
        break;

    case CAN:
        //pin PB2 low
        PORTB &= ~(1<<PORTB0);
        break;

    default:
        break;
    }
}

void SPI_release_slaves(){
    PORTB |= (1<<PORTB3) | (1<<PORTB4) | (1<<PORTB0);
}



void SPI_write_byte(char cData){
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));
    
}

void SPI_read_byte(char *cData){

    SPI_write_byte(*cData);

    *cData = SPDR;
}


void SPI_write(char* buffer, size_t length) {
    for (size_t i=0; i<length; i++){
        SPI_write_byte(*(buffer+i));
    }

    SPI_release_slaves();
    
}

void SPI_read(char* buffer, size_t length) {

    for (size_t i=0; i<length; i++){
        SPI_read_byte(buffer+i);
    }

    SPI_release_slaves();
    
}

