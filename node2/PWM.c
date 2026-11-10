#include "PWM.h"

void pwm_init() {

    // Klokke til PIOB og PWM
    PMC->PMC_PCER0 |= (1 << ID_PIOB);
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));

    // ########################################
    //         HERFRA FUNGERER DET
    // ########################################

    // PB13 (PWMH1) og PC18 (PWMH6) -> periferi B
    PIOB->PIO_ABSR |= PIO_ABSR_P13; //| PIO_ABSR_P19;
    PIOB->PIO_PDR   = PIO_PDR_P13; // | PIO_PDR_P19;
   
    // Kanal 1 (pinne PC13 - 21)
    PWM->PWM_CH_NUM[1].PWM_CMR  = PWM_CMR_CPRE_MCK_DIV_32 | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_CPRD_SERVO;
    PWM->PWM_CH_NUM[1].PWM_CDTY = PWM_MID_CDTY_SERVO;

    // Kanal 6 (pinne 45)
    //PWM->PWM_CH_NUM[6].PWM_CMR  = PWM_CMR_CPRE_MCK_DIV_32 | PWM_CMR_CPOL;
    //PWM->PWM_CH_NUM[6].PWM_CPRD = PWM_CPRD_SERVO;
    //PWM->PWM_CH_NUM[6].PWM_CDTY = PWM_MID_CDTY_SERVO;

    // Start begge kanalene
    PWM->PWM_ENA = PWM_ENA_CHID1; // | PWM_ENA_CHID6;
}

void pwm_set_duty_cycle_cdty( uint32_t CDTY ) {

    // clamp (max and min value safe for servo)
    if (CDTY < PWM_MIN_CDTY_SERVO){
        CDTY = PWM_MIN_CDTY_SERVO;
    } else if ( CDTY > PWM_MAX_CDTY_SERVO) {
        CDTY = PWM_MAX_CDTY_SERVO;
    }

    //update dutycycle
    PWM->PWM_CH_NUM[1].PWM_CDTYUPD = CDTY;
}

// moving average 3 values
uint8_t filter_process(uint8_t new_value){

    // start with value indicating that the ball is not blocking.
    static uint8_t x1 = 160;
    static uint8_t x2 = 160;
    static uint8_t x3 = 160;
    static uint8_t x4 = 160;
    static uint8_t x5 = 160;


    x5 = x4;
    x4 = x3;
    x3 = x2;   
    x2 = x1;
    x1 = new_value;
    
    uint8_t filtered_value = (x1 + x2 + x3 + x4 + x5) / 5;

    return filtered_value;
}

void pwm_set_duty_cycle_from_byte( uint8_t b ) {

    uint8_t dcs = filter_process(b);

    if (dcs > 100 && dcs < 150) { dcs = 150; }

    uint32_t cdty = PWM_MIN_CDTY_SERVO + ( (uint32_t)dcs * (PWM_MAX_CDTY_SERVO - PWM_MIN_CDTY_SERVO)) / 255; 
    pwm_set_duty_cycle_cdty(cdty);
}