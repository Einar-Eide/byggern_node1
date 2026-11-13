#include "solenoid.h"

void solenoid_init(){
    
    //konfigure pin PC1 til kontrollsignal transistor
    PIOC->PIO_PER  |= PIO_PER_P1;  // enable PIO control
    PIOC->PIO_OER  |= PIO_OER_P1;  // sett til output

    //PIOC->PIO_ODSR |= PIO_ODSR_P1; // sett pin til høy
    PIOC->PIO_SODR |= PIO_SODR_P1; // sett pin lav
}

void solenoid_push(){

    PIOC->PIO_CODR |= PIO_CODR_P1;
    uint64_t toggle_time = time_now();
    while (time_now() - toggle_time < msecs(100)){}
    PIOC->PIO_SODR |= PIO_SODR_P1;
    printf("solenoid triggered\r\n");
}