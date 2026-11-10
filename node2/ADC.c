#include "ADC.h"

void adc_init() {

    //Enable clock for adc controller
    PMC->PMC_PCER1 = (1 << (ID_ADC - 32));

    //software reset adc (simulates hardware reset)
    ADC->ADC_CR = (ADC_CR_SWRST);

    //
    ADC->ADC_MR = (ADC_MR_TRGEN_DIS)       // Disable hardware triggers 
                 |(ADC_MR_LOWRES_BITS_12)  // 12 bit resolution
                 |(ADC_MR_SLEEP_NORMAL)    // Normal mode
                 |(ADC_MR_STARTUP_SUT512)  // Startuptime 512*adc_period = ca 36 mikrosek
                 |(ADC_MR_SETTLING_AST9)   // Settling time 9*adc period = 640 ns
                 |(ADC_MR_FREERUN_ON)      // kontinuerlig sampling
                 |(ADC_MR_PRESCAL(2))      // Gir f_adc = 14MHz
                 |(ADC_MR_TRACKTIM(2));    // Tracking time, time needed to track the analog signal. Calculated using: racking Time = (TRACKTIM + 1) * ADCClock periods (ADCClock period = 14Mhz) (racking Time>160ns)
 
    //Enable channel 0
    ADC->ADC_CHER = (ADC_CHER_CH0);   

    // Start analog-to-digital conversion
    ADC->ADC_CR = ADC_CR_START;
}

void adc_analog_read(uint16_t *data) {
    
    // verdi på channel 0
    // *data = ADC->ADC_CDR[0];

    // wait until End of Conversion 0 is ready (EOC0 = 1)
    while ( (~(ADC->ADC_ISR)) & 0b1 );

    // nyeste verdi konvertert:
    *data = (ADC->ADC_LCDR) & ( 0xFFF ); // only the last 12 bits are read out
}

// moving average 3 values
uint16_t adc_filter_process(uint16_t new_value){

    // start with value indicating that the ball is not blocking.
    static uint16_t x1 = 4095;
    static uint16_t x2 = 4095;
    static uint16_t x3 = 4095;

    x3 = x2;
    x2 = x1;
    x1 = new_value;
    
    uint16_t filtered_value = (x1 + x2 + x3) / 3;

    return filtered_value;
}

uint8_t is_ir_diode_blocked() {

    // get new ADC reading
    uint16_t data;
    adc_analog_read(&data);

    // filter new value
    uint16_t photo_diode_signal = adc_filter_process(data);

    // return true if value is less than 512 ( < 1.6V )
    return photo_diode_signal < 2048;
}