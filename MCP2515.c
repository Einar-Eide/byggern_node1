#include "MCP2515.h"


uint8_t mcp2515_init() {
    uint8_t value;
    mcp2515_reset();
    
    mcp2515_read(MCP_CANSTAT, &value);
    if ( (value & MODE_MASK) != MODE_CONFIG ) {
        printf (" MCP2515 is NOT in configuration mode after reset !\n" );
        return 1;
    }

    //mcp2515_write_byte(MCP_CNF1, 0b01000001); //BRP = 1, sync jump width = 2xTq
    //mcp2515_write_byte(MCP_CNF2, 0b01000001); //BRP = 1, sync jump width = 2xTq
    //mcp2515_write_byte(MCP_CNF3, 0b01000001); //BRP = 1, sync jump width = 2xTq


    //mcp2515_write_byte(MCP_TXB0CTRL, )


    mcp2515_write_byte(MCP_CANCTRL, 0b01000000);     // loopback mode

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


void mcp2515_read_rx_buffer(uint8_t RXn, uint8_t* data) {
    SPI_select_slave(CAN);
    switch(RXn){
        case 0:
            SPI_write_byte(MCP_READ_RX0);
            SPI_read_byte(data);
            break;
        case 1:
            SPI_write_byte(MCP_READ_RX0);
            SPI_read_byte(data);
            break;

    }
    SPI_release_slaves();
}



