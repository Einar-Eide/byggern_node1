#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "uart.h"
#include "can.h"
#include "CAN_ID.h"
#include "PWM.h"
#include "ADC.h"
#include "encoder.h"
#include "motor.h"
#include "game.h"
#include "time.h"
#include "solenoid.h"


#define F_CPU 84000000

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    uart_init(F_CPU, 9600);
    printf("Hello World4\r\n");

    CanInit init = {};
    init.phase1 = 6;
    init.phase2 = 5;
    init.brp = 41; //skal kanskje halvveres fordi can clk = MCK/2
    init.propag = 1;
    init.sjw = 0;
    init.smp = 0;

    // init functions:
    can_init(init, 0);
    pwm_init();
    adc_init();
    encoder_init();
    motor_init();
    solenoid_init();

    printf("Hello World3\r\n");


    CanMsg message = {};
    
    uint8_t joy_x;
    uint8_t joy_y;
    uint8_t btn_pressed = 0;

    uint8_t can_trigger_new_point = 0;

    uint8_t game_is_running = 0;
    uint8_t prev_game_is_running = 0;
    static bool pi = false;
    uint8_t lives = GAME_LIVES;
    Game_Mode prev_mode;
    Game_Mode mode;

    CanMsg lives_msg;
    lives_msg.id = GAME_LIVES_ID;
    lives_msg.length = 1;
    lives_msg.byte[0] = lives;

    uint32_t last_time_blocked = 0;

    pwm_set_duty_cycle_from_byte(255);

    
    while(1) {
        uint8_t received = can_rx(&message);

        if (received) {

            if (message.id == JOY_X_ID ) {

                joy_x = message.byte[0];
            }
            else if (message.id == JOY_Y_ID) {

                joy_y = message.byte[0];
                pwm_set_duty_cycle_from_byte(joy_y);
            }

            else if (message.id == JOY_BTN_ID) {
                solenoid_push();
            }

            else if (message.id == GAME_MODE_ID){
                prev_mode = mode;
                mode = message.byte[0];

                if(prev_mode == MENU && mode != prev_mode ){
                    encoder_reset();
                    game_reset();
                    lives = GAME_LIVES; 
                    game_is_running = 1;
                    printf("lives: %d\r\n", lives);

                    // send start lives
                    lives_msg.byte[0] = lives;
                    can_tx(lives_msg);
                }
            }
        }

        //PI regulator
        if (mode == 2) {

            uint32_t m_positon;
            encoder_read(&m_positon);

            motor_pi_controller(joy_x, m_positon);

        } 
        else if (mode == 1) {

            motor_set_dir(&joy_x);
            motor_set_speed(&joy_x);

        } else if (mode == 0) {
            lives = GAME_LIVES;
        }

        // set prev game state
        prev_game_is_running =  game_is_running;


        //Test to check if the diode is blocked


        if (is_ir_diode_blocked()) {
            
            last_time_blocked = totalMsecs(time_now());

            if (can_trigger_new_point ) {

                lives -=1;
                printf("ir sensor is blocked! %d\r\n", lives);

                if(lives == 0){
                    game_is_running = 0;
                }

                lives_msg.byte[0] = lives;
                can_tx(lives_msg);

                //printf("Sent can message");
            }

            can_trigger_new_point = false;

        } else {

            if( totalMsecs(time_now()) > last_time_blocked + 2000 ){ // wait 2 sek before next loose point
                can_trigger_new_point = true;
            } else {
                can_trigger_new_point = false;
            }
        }
    }
}