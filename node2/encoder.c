#include "encoder.h"

void encoder_init() {

    //Enable klokke for PIOC og TC2             //Configure PMC to enable the Timer clock counter
    PMC->PMC_PCER0 |= (1 << ID_PIOC);
    PMC->PMC_PCER1 |= ((1 << (ID_TC6 - 32)) | (1 << (ID_TC7 - 32)) | (1 <<(ID_TC8 - 32)));


    //Gi PC26 og PC26 til peripheral B (TC2)
    PIOC->PIO_ABSR |= PIO_ABSR_P25 | PIO_ABSR_P26;
  
    //Disable PIO control of TIO-pins
    PIOC->PIO_PDR |= PIO_PDR_P25 | PIO_PDR_P26;


    //enable TC2 in quadrature mode
    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_EDGPHA;

    //Select external clock XC0
    TC2->TC_CHANNEL[0].TC_CMR = (TC_CMR_TCCLKS_XC0);

    // enable, reset and start counter clock
    TC2->TC_CHANNEL[0].TC_CCR = (TC_CCR_CLKEN) | (TC_CCR_SWTRG);
}

void encoder_reset() {
    TC2->TC_CHANNEL[0].TC_CCR = (TC_CCR_CLKEN) | (TC_CCR_SWTRG);
}

void encoder_read(uint32_t* data) {
    
    *data = TC2->TC_CHANNEL[0].TC_CV;
    // printf("encoder value: %d \r\n", *data);
}