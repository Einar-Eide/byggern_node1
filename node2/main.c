#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "uart.h"
#include "can.h"
#include "CAN_ID.h"

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
    printf("Hello World2\r\n"); //what? \r MÅ vær før \n

    CanInit init = {};
    init.phase1 = 6;
    init.phase2 = 5;
    init.brp = 41; //skal kanskje halvveres fordi can clk = MCK/2
    init.propag = 1;
    init.sjw = 0;
    init.smp = 0;
   
    can_init(init, 0);

    CanMsg message = {};
    
    int8_t joy_x;
    int8_t joy_y;

    while(1) {
        uint8_t received = can_rx(&message);

        if (received) {

            if (message.id == JOY_X_ID ) {
                joy_x = (int8_t)message.byte[0];

                //printf("Joystick  X  %d\r\n", joy_x);

            }
            else if (message.id == JOY_Y_ID) {
                joy_y = (int8_t)message.byte[0];

                //printf("Joystick y:  %d\r\n", joy_y);
            }

            printf("Joystick x: %d, y: %d\r\n", joy_x, joy_y);
           
        }
    }
    

    // can_init()
    // can_message_send()
    // can_data_receive()
    // can_int_vect()
    // // Tip: Structs could be useful for CAN messages
    // struct can_message {
    // uint16_t id ;
    // char data_length ;
    // char data [8];
    // };

    while (1)
    {
        /* code */
    }
    
}