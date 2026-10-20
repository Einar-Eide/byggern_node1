#include "MCP2515.h"


uint8_t mcp2515_init() {
    
    uint8_t value;
    mcp2515_reset();
    
    mcp2515_read(MCP_CANSTAT, &value);
    if ( (value & MODE_MASK) != MODE_CONFIG ) {
        printf (" MCP2515 is NOT in configuration mode after reset !\n" );
        return 1;
    }
    
    mcp2515_write_byte(MCP_CANCTRL, MODE_CONFIG | CLKOUT_DISABLE); // Set to config mode, disable CLKOUT
    mcp2515_write_byte(MCP_CNF1, (SJW1 | BRP3) ); // sync jump width = 1xTq, Baud rate prescaler = 3 for Tq = 500ns
    mcp2515_write_byte(MCP_CNF2, BTLMODE | SAMPLE_1X | PS1_7 | PRSEG_2); //Set PS2 manually, PS1 = 7, PRSEG = 2
    mcp2515_write_byte(MCP_CNF3, WAKFIL_ENABLE |PS2_6 ); //Wake-up filter enabled, PS2 = 6


    //mcp2515_write_byte(MCP_TXB0CTRL, )


    mcp2515_write_byte(MCP_CANCTRL, MODE_LOOPBACK | CLKOUT_DISABLE);     // loopback mode

    return 0;
}


void mcp2515_read( uint8_t address, uint8_t* data ){
    SPI_select_slave(CAN);
    SPI_write_byte(MCP_READ);
    SPI_write_byte(address);
    SPI_read_byte(data);
    SPI_release_slaves();
}

void mcp2515_write_byte( uint8_t address, uint8_t data){
    SPI_select_slave(CAN);
    SPI_write_byte(MCP_WRITE);
    SPI_write_byte(address);
    SPI_write_byte(data);
    SPI_release_slaves();
}

void mcp2515_write(uint8_t address, char* buffer, size_t length){
    if (buffer == 0 || length == 0){
        return;
    }
    SPI_select_slave(CAN);
    SPI_write_byte(MCP_WRITE);
    SPI_write_byte(address);
    SPI_write(buffer, length);
    SPI_release_slaves();
}

void mcp2515_bitmod(uint8_t address, uint8_t mask, uint8_t data){
    SPI_select_slave(CAN);
    SPI_write_byte(MCP_BITMOD);
    SPI_write_byte(address);
    SPI_write_byte(mask);
    SPI_write_byte(data);
    SPI_release_slaves();
}

void mcp2515_reset(){
    SPI_select_slave(CAN);
    SPI_write_byte(MCP_RESET);
    SPI_release_slaves();
}

uint8_t mcp2515_read_status(){
    char status;

    SPI_select_slave(CAN);
    SPI_write_byte(MCP_READ_STATUS);
    SPI_read_byte(&status);
    SPI_release_slaves(); 

    return (uint8_t)status;  
}

void mcp2515_send_byte(uint16_t id, uint8_t data) {
    #define DEBUGGING_MCP2515_SEND_BYTE 1

    // set ID
    mcp2515_write_byte(MCP_TXB0SIDH, id>>3);
    mcp2515_bitmod(MCP_TXB0SIDL, 0b111<<5, id<<5);

    // set data-length and data
    mcp2515_write_byte(MCP_TXB0DLC, (0<<6) | 1); //transmits 1 byte as a dataframe
    mcp2515_write_byte(MCP_TXB0D0, data); 

    if (DEBUGGING_MCP2515_SEND_BYTE) {
        uint8_t read_MCP_TXB0SIDH;
        uint8_t read_MCP_TXB0SIDL; 
        uint8_t read_MCP_TXB0DLC; 
        uint8_t read_MCP_TXB0D0; 

        mcp2515_read(MCP_TXB0SIDH, &read_MCP_TXB0SIDH);
        mcp2515_read(MCP_TXB0SIDL, &read_MCP_TXB0SIDL);
        mcp2515_read(MCP_TXB0DLC,  &read_MCP_TXB0DLC);
        mcp2515_read(MCP_TXB0D0,   &read_MCP_TXB0D0);

        printf("wrote %d to MCP_TXB0SIDH\n\r", id>>3);
        printf("read  %d\n\n\r", read_MCP_TXB0SIDH);
        printf("wrote %d to MCP_TXB0SIDL\n\r", (id<<5) & (0b111<<5));
        printf("read  %d\n\n\r", read_MCP_TXB0SIDL);
        printf("wrote %d to MCP_TXB0DLC\n\r", (0<<6) | 1);
        printf("read  %d\n\n\r", read_MCP_TXB0DLC);
        printf("wrote %d to MCP_TXB0D0\n\r", data);
        printf("read  %d\n\n\r", read_MCP_TXB0D0);
        
    }
    
}

void mcp2515_read_byte(uint16_t id, uint8_t data){
        // request to send
    mcp2515_bitmod(MCP_TXB0CTRL, 1<<3, 1<<3);

    _delay_us(500);

    uint8_t TXB0CTRL = 0;
    mcp2515_read(MCP_TXB0CTRL, &TXB0CTRL);
    printf("TXB0CTRL: %d\n\n\r", TXB0CTRL);
    
    //check receive interrupt flags
    uint8_t RX0_buffer = 0;
    uint8_t RX1_buffer = 0;
    mcp2515_read(0x66, &RX0_buffer);
    mcp2515_read(0x76, &RX1_buffer);
    printf("RX0_buffer: %d\n\n\r", RX0_buffer);
    printf("RX1_buffer: %d\n\n\r", RX1_buffer);

    uint8_t rx0_data = 91;
    uint8_t rx1_data = 91;

    mcp2515_read_rx_buffer(0, &rx0_data);
 

    printf("Read can message %d\n\n\r", rx0_data);
}

void mcp2515_load_tx_buffer(uint8_t TXn, uint8_t data) {
    SPI_select_slave(CAN);
    switch(TXn){
        case 0:
            SPI_write_byte(MCP_LOAD_TX0);
            SPI_write_byte(data);
            break;
        case 1:
            SPI_write_byte(MCP_LOAD_TX1);
            SPI_write_byte(data);
            break;

        case 2:
            SPI_write_byte(MCP_LOAD_TX2);
            SPI_write_byte(data);
            break;

    }
    SPI_release_slaves();
}

void mcp2515_rts(uint8_t TXn){
    SPI_select_slave(CAN);
    switch(TXn){
        case 0:
            SPI_write_byte(MCP_RTS_TX0);
            break;
        case 1:
            SPI_write_byte(MCP_RTS_TX1);
            break;

        case 2:
            SPI_write_byte(MCP_RTS_TX2);
            break;

        default:
            SPI_write_byte(MCP_RTS_ALL - 7); //Send n = 000
            break;
    }
    SPI_release_slaves();

}


void mcp2515_read_rx_buffer(uint8_t RXn, uint8_t *data) {
    SPI_select_slave(CAN);
    switch(RXn){
        case 0:
            SPI_write_byte(0b10010010);
            SPI_read_byte(data);
            break;
        case 1:
            SPI_write_byte(MCP_READ_RX1);
            SPI_read_byte(data);
            break;

    }
    SPI_release_slaves();
}

// typedef struct {
//     uint16_t id;
//     uint8_t length;
//     uint8_t data;
// } CAN_frame


