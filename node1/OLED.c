#include "OLED.h"

uint16_t cursor_column_position = 0;


//initialize
void OLED_init(){

    DDRB |= (1<<OLED_DC);

    OLED_send_command(0xAF); 

    _delay_ms(10);

    OLED_send_command(0xA5); // turn entire screen on
    _delay_ms(100);
    OLED_send_command(0xA4); // RESET: turn entire screen on

    SPI_release_slaves();
    OLED_clear_dispay();
    OLED_go_to(0,0);
}


//Send command over SPI
void OLED_send_command(char cmd) {

    SPI_select_slave(OLED);
    
    // set D/C# low
    PORTB &= ~(1<<OLED_DC);
    SPI_write_byte(cmd);

    SPI_release_slaves();
}

//Set cursor at page
void OLED_go_to_page(uint8_t page) {

    OLED_send_command(0xB0 | (0x07 & page)); // Go to first page + page
}

//Set cursor at column
void OLED_go_to_column(uint8_t column) {

    OLED_send_command(0x0F & column);
    OLED_send_command(0x10 | (column >> 4));

    cursor_column_position = column;
}


//Set cursor position at row, column
void OLED_go_to(uint8_t column, uint8_t row){

    OLED_go_to_page(row);
    OLED_go_to_column(column);
}


void OLED_send_data(char data){

    //could remove to improve performance? Beware
    SPI_select_slave(OLED);
    
    // set D/C# low
    PORTB |= (1<<OLED_DC);
    SPI_write_byte(data);

    SPI_release_slaves();
}


//Clear OLED display
void OLED_clear_dispay(){

    for(int j = 0; j < 8; j++){

        OLED_go_to_page(j);
        OLED_go_to_column(0);

        for (int i = 0; i < 128; i++){
            OLED_send_data(0x00);
        }
    }
}

//Module internal function to print char on OLED display
void OLED_print_char(char c) {

    for(uint8_t i = 0; i < 5; i++) {

        OLED_send_data(pgm_read_byte(&font5[c - 32][i]));
    }
}

//String length function
size_t strlen(char *str) {

    int c = 0;

    // Loop until the null terminator '\0' is reached
    while (str[c] != '\0') {
        c++;
    }
    
    return c;
}

//Print string at current pointer position
void OLED_printf(char *str){

    while (*str){
        OLED_print_char(*str);
        OLED_send_data(0x00); //Space between characters
        str++;
    }
}

//Print str at left, center or right side of pos
void OLED_printf_2(char *str, textpos pos){

    size_t px_length = strlen(str) * 6; //5+1
    uint16_t start_column = cursor_column_position;
    uint16_t print_start_column = start_column;
    
    switch (pos)
    {
    case OLED_CENTER:
        print_start_column -= px_length / 2;
        break;
    case OLED_LEFT:
        print_start_column -= px_length;
        break;

    case OLED_RIGHT:
        // stay
        break;
    }

    OLED_go_to_column(print_start_column);

    while (*str){
        OLED_print_char(*str);
        OLED_send_data(0x00); //mellomrom
        str++;
    }

    OLED_go_to_column(start_column);
}

//Print string at left, center or right part of display at given page nr
void OLED_printf_1(char *str, textpos pos, uint8_t page){

    switch (pos){

    case OLED_LEFT:
        OLED_go_to(0, page);
        break;
    
    case OLED_CENTER:
        uint8_t centercol = (128 - (strlen(str) * 6))/2;
        OLED_go_to(centercol, page);
        break;

    case OLED_RIGHT:
        uint8_t rightcol = (128 - (strlen(str)*6));
        OLED_go_to(rightcol, page);
        break;
        
    default:
        break;
    }

    while (*str){
        OLED_print_char(*str);
        OLED_send_data(0x00); //mellomrom
        str++;
    } 
}

//Print emoji of choice
void OLED_print_emoji(uint8_t emoji){

    for(uint8_t i = 0; i < 8; i++) {

        OLED_send_data(pgm_read_byte(&emoji_font[emoji][i]));
    }
}
/*
//Print car on display
void OLED_print_car_animation(){

    _delay_ms(500);

    OLED_clear_dispay();
    OLED_printf_1("Kjor pent!", OLED_CENTER, 10);
    OLED_printf_1("     _________        ", OLED_CENTER,11);
    OLED_printf_1(" __/|_||_||  \\___    ", OLED_CENTER,12);
    OLED_printf_1("/___          ___\\", OLED_CENTER, 13);
    OLED_printf_1("  O            O   ", OLED_CENTER, 14);
        

    _delay_ms(500);

    OLED_clear_dispay();
}

*/
//Print game over
void OLED_game_over(){
    int i;

    for (i = 0; i < 40; i++) {
        OLED_printf_1("GAME OVER!", OLED_CENTER, i);
        _delay_ms(100);
        OLED_clear_dispay(); 
    }
}
    