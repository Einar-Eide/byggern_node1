#include "OLED.h"

void OLED_init(){
    DDRB |= (1<<OLED_DC);

    OLED_send_command(0xAF); 

    _delay_ms(10);

    OLED_send_command(0xA5); // turn entire screen on
    _delay_ms(100);
    OLED_send_command(0xA4); // RESET: turn entire screen on

    SPI_release_slaves();
    OLED_clear_dispay();

}

void OLED_send_command(char cmd) {

    SPI_select_slave(OLED);
    
    // set D/C# low
    PORTB &= ~(1<<OLED_DC);
    SPI_write_byte(cmd);

    SPI_release_slaves();
}

void OLED_go_to_line(uint8_t line) {
    OLED_send_command(0xB0 | (0x07 & line)); // Go to first page + page
}


void OLED_go_to_column(uint8_t column) {
    OLED_send_command(0x0F & column);
    OLED_send_command(0x10 | (column >> 4));
}

void OLED_send_data(char data){

    //could remove to improve performance? Beware
     SPI_select_slave(OLED);
    
    // set D/C# low
    PORTB |= (1<<OLED_DC);
    SPI_write_byte(data);

    SPI_release_slaves();
}



void OLED_clear_dispay(){ //clears display
    for(int j = 0; j < 8; j++){
        OLED_go_to_line(j);
        OLED_go_to_column(0);
        for (int i = 0; i < 128; i++){
            OLED_send_data(0x00);
    }}
}


void OLED_set_pixel(char x, char y, char value){
    OLED_go_to_line(y/8);
    OLED_go_to_column(x);

    uint8_t row_in_page = y%8;
    
    OLED_send_data(value<<row_in_page);
}

void OLED_print_char(char c) {
    for(uint8_t i = 0; i < 5; i++) {
        OLED_send_data(pgm_read_byte(&font5[c - 32][i]));
    }
}

void OLED_printf(char *str){
    while (*str){
        OLED_print_char(*str);
        OLED_send_data(0x00); //mellomrom
        str++;
    } 
}


