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

/*
void SRAM_test(void)
{
    volatile char *ext_ram = (char *)0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;

    printf("Starting SRAM test...\n\r");

    // rand() stores some internal state, so calling this function in a loop
    // will yield different seeds each time (unless srand() is called before this
    // function)
    uint16_t seed = rand();

    // Write phase: Immediately check that the correct value was stored
    srand(seed);

    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];

        if (retreived_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r",
                   i, retreived_value, some_value);
            write_errors++;
        }
    }

    // Retrieval phase: Check that no values were changed during or after the
    // write phase
    srand(seed);

    // Reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];

        if (retreived_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r",
                   i, retreived_value, some_value);
            retrieval_errors++;
        }
    }

    printf("SRAM test completed with\n%4d errors in write phase and\n%4d errors in retrieval phase\n\r\n\r",
           write_errors, retrieval_errors);
}
*/

int main(void) {
	
	// set pin to output
	/*
	DDRA = 0xFF;
	DDRE = (1<<DDE1);

	//volatile char *ext_ram = (char *) 0x1800; 
	MCUCR = (1<<SRE);
	PORTA = 0b01010101;
	PORTE = (0<<PORTE1);
	_delay_ms(50);
	PORTE = (1<<PORTE1);
	

	*/
	uart_init( MYUBRR );
    printf("TEST1?");

    
    // ???
	MCUCR |= (1<<SRE);

	
	// Sette port PD4 til output
	DDRD = (1 << DDD4);

    // set counter reset value
    OCR3AL = 1;

	// config timer
    TCCR3A = (1<<COM3A0);
    TCCR3B = (1<<CS30) | (1<<WGM32);
    
    

    volatile uint8_t joy_x_min;
    volatile uint8_t joy_x_max;
    volatile uint8_t joy_y_min;
    volatile uint8_t joy_y_max;

    //calibrate_joy();

    // printf("Finished calibration of joystick!\n\r");
    
    // printf("joy_x min: %d\t\max: %d\n\r", joy_x_min, joy_x_max);
    // printf("joy_y min: %d\t\max: %d\n\r", joy_y_min, joy_y_max);
    
    //Sram testen tar for mye plass i minne, så vi kjører den ikke hver gang
	//SRAM_test();
    
    char testbyte[16] = "Hello World";
    char testCHAR = 50;

    char* p_testbyte = &testbyte[0];


    SPI_master_init();

    OLED_init();

    // OLED_set_pixel(1,0,1);
    // OLED_set_pixel(0,1,1);

    
    // _delay_ms(1000);
    //SPI_select_slave(OLED);

    
    // PORTB |= (1<<OLED_DC);
    // unsigned char c = 0x0F;
    // SPI_read_byte(&c);

    // printf("fra screen: %d", c);

    printf("Test\n\r");

    
    mcp2515_init();
    
    //Test av printe funkjson på oled skjermen
    // OLED_print_char('H');
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
        
        // OLED_print_car_animation();


        uint8_t data = 0;
        //mcp2515_load_tx_buffer(0, 171);
        //mcp2515_rts(0);
        //mcp2515_read(0x31, &data);
        //mcp2515_read_rx_buffer(0, &data);

        //mcp2515_load_tx_buffer(0, 0b10101000);


        printf("Data read: %d\n\r", data);
    

        // Test read-write:

        // read inital txb0ctrl
        //uint8_t txb0ctrl;
        //mcp2515_read(MCP_TXB0CTRL, &txb0ctrl);

        //printf("Data txb0ctrl: %d\n\r", txb0ctrl);

        // write to txb0ctrl
        //mcp2515_bitmod(MCP_TXB0CTRL, 1<<1, 1<<1);
        //mcp2515_bitmod(MCP_TXB0CTRL, 1<<3, 1<<3);

        // read final txb0ctrl
        //mcp2515_read(MCP_TXB0CTRL, &txb0ctrl);

        //printf("Data txb0ctrl: %d\n\r", txb0ctrl); // should be 3?

        //test send data oppg 5
        uint16_t id = 1322;
        uint8_t test_data = 100;
        mcp2515_send_byte(id, test_data);
        mcp2515_read_byte(id, test_data);


        while (1)
        {
            //printf("TEST!\n\r");
            MENU_update();
            //printf("Button value: %d", button_pressed());
        
        }
        

        
    }
   

    //OLED_send_command(0xA4);

    // Replace with your application code
    while (1) 
    {

        uint8_t x = 0;
        uint8_t y = 0;

        uint8_t x1 = 0;
        uint8_t y1 = 0;

        //get_position_raw(&x, &y, &x1, &y1);
        //printf("x-positions: %d\t%d", x, y);
        //get_direction(&x, &MCP2515.c:181:27y, &x1, &y1);

        //get_analog_position(joy_x_min, joy_x_max, joy_y_min, joy_y_max);

        //SPI_read(testbyte, sizeof(testbyte));
        

        //printf("read data: %d\n\r", testbyte[0]);
        
        //SPI_write(testbyte, 5);
        _delay_us(100);

		PORTA = (1<<0);
		_delay_ms(10);
		PORTA = (0<<0);
		_delay_ms(10);

		// uart_send('a');
		//char X = uart_read();
		//uart_send(X);

		//printf("yey!\n\r");

		//volatile char *ext_ram = (char *) 0x1800; 


    }

    
}


