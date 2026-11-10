#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include <stdlib.h>

#include "UART.h"
#include "IO_inputs.h"
#include "SPI.h"
#include "OLED.h"
#include "menu.h"
#include "MCP2515.h"
#include "CAN_ID.h"



int main(void) {
	
	uart_init( MYUBRR );
    printf("TEST1?");
    
    // Enable external SRAM
	MCUCR |= (1<<SRE);

	// Set port PD4 til output
	DDRD = (1 << DDD4);

    // set counter reset value
    OCR3AL = 1;

	// config timer
    TCCR3A = (1<<COM3A0);
    TCCR3B = (1<<CS30) | (1<<WGM32);
    
    // declare joystick varibles
    volatile uint8_t joy_x_min;
    volatile uint8_t joy_x_max;
    volatile uint8_t joy_y_min;
    volatile uint8_t joy_y_max;

    //calibrate_joy();
    
    char testbyte[16] = "Hello World";
    char testCHAR = 50;

    char* p_testbyte = &testbyte[0];

    SPI_master_init();

    OLED_init();

    mcp2515_init();
    
    while (1) 
    {
        //Test display OLED print function
        OLED_clear_dispay();
        OLED_go_to_page(0x00);
        OLED_go_to_column(128/2);
        OLED_printf_2("Hello World!", OLED_CENTER);
        OLED_go_to_page(0x32);
        OLED_printf_2("Goodnight:)", OLED_CENTER);
        OLED_go_to_page(0x64);
        OLED_printf_2("<3", OLED_CENTER);
        
        OLED_go_to(0, 0x96);

        for(int i=0; i<12; i++){
            OLED_print_emoji(i);
            OLED_send_data(0x00);
        }
        
        uint8_t data = 0;
    
        printf("Data read: %d\n\r", data);
    

        
        uint8_t ctrl_reg = 100;

        
        mcp2515_read_byte(MCP_TXB0CTRL, &ctrl_reg);

        printf("%d\n\r", ctrl_reg);

        while (1)
        {
            //MENU_update();

            uint8_t joy_pos_x;
            uint8_t joy_pos_y;
            uint8_t btn_pres = button_pressed();

            joy_get_analog_positive_position(&joy_pos_x, &joy_pos_y);
            
            mcp2515_send_byte(JOY_X_ID, (uint8_t)joy_pos_x);
            _delay_ms(10); 

            mcp2515_send_byte(JOY_Y_ID, (uint8_t)joy_pos_y);
            _delay_ms(10); 

            if(btn_pres){
            mcp2515_send_byte(JOY_BTN_ID, (uint8_t)btn_pres);
            _delay_ms(10); 
            }
        }
        
    }
  
}


