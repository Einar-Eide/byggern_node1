#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "uart.h"
#include "can.h"
#include "CAN_ID.h"
#include "PWM.h"
#include "ADC.h"
#include "game.h"

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
    printf("Hello World4\r\n"); //what? \r MÅ vær før \n

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

    printf("Hello World3\r\n"); //what? \r MÅ vær før \n


    CanMsg message = {};
    
    uint8_t joy_x;
    uint8_t joy_y;

    uint8_t can_trigger_new_point = 0;


    while(1) {
        uint8_t received = can_rx(&message);

        if (received) {

            if (message.id == JOY_X_ID ) {
                joy_x = message.byte[0];
                pwm_set_duty_cycle_from_byte(joy_x);

                //printf("Joystick  X  %d\r\n", joy_x);

            }
            else if (message.id == JOY_Y_ID) {
                joy_y = message.byte[0];

                //printf("Joystick y:  %d\r\n", joy_y);
            }

            //printf("Joystick x: %d, y: %d\r\n", joy_x, joy_y);

           
        }
        
        if (is_ir_diode_blocked()) {
            printf("ir sensor is blocked! \r\n");
            
            if ( can_trigger_new_point ) {
                printf("current score: %d\r\n", increase_points());
                can_trigger_new_point = false;
            } 
        } else {
            can_trigger_new_point = true;
        }

    }
    
}