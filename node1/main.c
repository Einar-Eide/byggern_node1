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
	
    //initialize uart
	uart_init( MYUBRR );
    //printf("TEST1?");
    
    // Enable external SRAM
	MCUCR |= (1<<SRE);

	// Set port PD4 til output
	DDRD = (1 << DDD4);

	// config Timer/counter 3
    OCR3AL = 1;                      // set counter reset value
    TCCR3A = (1<<COM3A0);            // Toggle on compare match
    TCCR3B = (1<<CS30) | (1<<WGM32); // No prescaling, CTC mode
    
    // declare joystick varibles
    volatile uint8_t joy_x_min;
    volatile uint8_t joy_x_max;
    volatile uint8_t joy_y_min;
    volatile uint8_t joy_y_max;

    //calibrate_joy();

    //Initialize SPI, OLED, MCP2515
    SPI_master_init();
    OLED_init();
    mcp2515_init();
    
    while (1) 
    {
        //Update display
        MENU_update();

        //Send game state to node 2
        mcp2515_send_byte(GAME_MODE_ID, (uint8_t)g_p_game_state->game_mode);

        //If in game is in play
        if (g_p_game_state->game_mode == 1 || g_p_game_state->game_mode == 2) {
            uint8_t joy_pos_x;
            uint8_t joy_pos_y;
            uint8_t btn_pres = button_pressed();

            //get joystick position and send to node 2. If button pressed, send to node 2
            joy_get_analog_positive_position(&joy_pos_x, &joy_pos_y);
                
            mcp2515_send_byte(JOY_X_ID, (uint8_t)joy_pos_x);

            mcp2515_send_byte(JOY_Y_ID, (uint8_t)joy_pos_y);


            if(btn_pres){
                mcp2515_send_byte(JOY_BTN_ID, (uint8_t)btn_pres);
            }

            //Variables for recieved id and data
            uint16_t received_id;
            uint8_t received_data;

            // if new data of lives left is recieved, update game state lives
            while (mcp2515_receive_byte(&received_id, &received_data) ) {
                if (received_id == GAME_LIVES_ID) {
                    g_p_game_state->lives_left = received_data;
                }
                printf("GOT NEW DATA!!\r\n");
            }
        }
    }
}