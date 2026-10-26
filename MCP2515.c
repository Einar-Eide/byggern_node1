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

void mcp2515_receive_byte(uint16_t *id, uint8_t* data){

    _delay_us(500);

    uint8_t RXB0CTRL;
    mcp2515_read(MCP_RXB0CTRL, &RXB0CTRL);

    // les kun hvis man har mottatt ny verdig? Hvordan gjør man dette?

    mcp2515_read_rx_buffer(0, &id, &data);
 
    printf("Read can message %d\n\n\r", data);
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


void mcp2515_read_rx_buffer(uint8_t RXn, uint16_t* id, uint8_t *data) {
    SPI_select_slave(CAN);
    uint8_t idu;
    uint8_t idl;
    switch(RXn){
        case 0:
            SPI_write_byte(MCP_READ_RX0);
            SPI_read_byte(data);
            mcp2515_read_byte(MCP_RXB0SIDH, &idu);
            mcp2515_read_byte(MCP_RXB0SIDL, &idl);
            break;
        case 1:
            SPI_write_byte(MCP_READ_RX1);
            SPI_read_byte(data);
            mcp2515_read_byte(MCP_RXB1SIDH, &idu);
            mcp2515_read_byte(MCP_RXB1SIDL, &idl);
            break;

    }
    SPI_release_slaves();
    id = (idu<<3) | (idl>>5);
}

// typedef struct {
//     uint16_t id;
//     uint8_t length;
//     uint8_t data;
// } CAN_frame


