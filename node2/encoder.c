#include "encoder.h"

void encoder_init() {
    //Configure PMC to enable the Timer clock counter
    PMC->PMC_PCER1 = ((1 << (ID_TC6 - 32)) | (1 << (ID_TC7 - 32)) | (1 <<(ID_TC8 - 32)));

    //enable TC2 in quadrature mode
    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN;

    //Select external clock XC0
    TC2->TC_CMR0 = (TC_CMR_TCCLKS_XC0);
}