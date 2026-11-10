#include "motor.h"


void motor_init(){

    // PWM signal på PB12

    // Klokke til PIOB og PWM
    PMC->PMC_PCER0 |= (1 << ID_PIOB);
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));

    // FEIL: PC23 (PWMH1) og PC18 (PWMH6) -> periferi B // <--- dette er feil (er fra PWM.c)
    PIOB->PIO_ABSR |= PIO_ABSR_P12;
    PIOB->PIO_PDR   = PIO_PDR_P12;

    // Kanal 0 (pinne ?? - ??)
    // setter prescaler og polaritet
    // bestemer periode
    // setter duty cycle
    PWM->PWM_CH_NUM[0].PWM_CMR  = PWM_CMR_CPRE_MCK_DIV_4 | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[0].PWM_CPRD = PWM_CPRD_MOTOR;
    PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_MID_CDTY_MOTOR;

    // Start kanal 0
    PWM->PWM_ENA = PWM_ENA_CHID0;

    // direction pin PC23
    PIOC->PIO_PER  |= PIO_PER_P23;  // enable PIO control
    PIOC->PIO_OER  |= PIO_OER_P23;  // sett til output

    //PIOC->PIO_ODSR |= PIO_ODSR_P23; // sett pin til høy
    PIOC->PIO_SODR |= PIO_SODR_P23; // endre pin verdien
    
    
}

void motor_set_duty_cycle_cdty(uint32_t CDTY) {

    // clamp (max and min value safe for motor)
    if (CDTY < PWM_MIN_CDTY_MOTOR){
        CDTY = PWM_MIN_CDTY_MOTOR;
    } else if ( CDTY > PWM_MAX_CDTY_MOTOR) {
        CDTY = PWM_MAX_CDTY_MOTOR;
    }
    //update dutycycle
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = CDTY;
}

void motor_set_duty_cycle_from_byte(uint8_t b) {

    uint8_t dcs = b; // filter_process(b);

    uint32_t cdty = PWM_MIN_CDTY_MOTOR + ( (uint32_t)dcs * (PWM_MAX_CDTY_MOTOR - PWM_MIN_CDTY_MOTOR)) / 255; 
    motor_set_duty_cycle_cdty(cdty);

}

void motor_game_over(){

    motor_set_duty_cycle_from_byte(0);
}

Dir joy_get_direction(volatile uint8_t *joy_x) {

    Dir dir_joy_x = JOY_NEUTRAL;

    int neutral_pos_x = 128;

    if (*joy_x < neutral_pos_x-10) {
        dir_joy_x = JOY_LEFT;       
    } else if (*joy_x > neutral_pos_x + 10) {
        dir_joy_x = JOY_RIGHT;
    } 
    
    return dir_joy_x;
}

void motor_set_dir(volatile uint8_t *joy_x) {

    Dir dir = joy_get_direction(joy_x);

    if (dir == JOY_LEFT) {
        PIOC->PIO_SODR |= PIO_SODR_P23; // endre pin til høy
    } else if (dir == JOY_RIGHT) {
        PIOC->PIO_CODR |= PIO_CODR_P23; // sett pin til lav
    }

}

void motor_set_speed(volatile uint8_t *joy_x){
    
    int neutral_pos_x = 128;
    int deadzone = 1;
    int max_local_speed = 200;
    
    int pos_from_x = abs((int)*joy_x - neutral_pos_x);
    int distance;
    
    if (pos_from_x < deadzone){
        distance = 0;
    } else {
        distance = (pos_from_x-deadzone);
    }
    
    int linear_speed = (distance*max_local_speed)/(neutral_pos_x-deadzone);

    if (linear_speed > max_local_speed){
        linear_speed = max_local_speed;
    }

    motor_set_duty_cycle_from_byte((uint8_t)linear_speed); 
}

void motor_pi_controller(uint8_t r_, uint32_t y) {

    static Butterworth2ndOrder filter;
    static uint8_t filter_inizielized = 0;

    if (!filter_inizielized) {
        butterworth_init(&filter);
        filter_inizielized = 1;
    }
    
    // integrated error
    static double e_int = 0;
    static uint64_t motor_pi_controller_last_exec = 0;

    // run at the defined frequency
    if ( totalMsecs(time_now()) > motor_pi_controller_last_exec + 1000 / CODE_EXEC_FREQUENCY ) {

        if ( y > ENC_MAX_VALUE * 2) {
            y = 0;
        } 

        // scale joy input to be same size as y
        uint32_t r =  butterworth_process( &filter, r_ * MOTOR_POS_JOY_TO_ENC_SCALE_FACTOR );
        
        int e = r-y;

        if (e < MIN_ERROR_INTEGRATOR_CUTOFF && e > -MIN_ERROR_INTEGRATOR_CUTOFF) {
            e_int += e / CODE_EXEC_FREQUENCY;
        }
         
        
        double u = e*MOTOR_POS_CONTROLLER_K_P + e_int*MOTOR_POS_CONTROLLER_K_I; 

        
        // choose motor direction
        if (u < 0) {
            u *= -1;
            motor_set_dir_bool(1);
        } else {
            motor_set_dir_bool(0);
        }

        // clamp u
        if (u > ENC_MAX_VALUE ) {
            u = ENC_MAX_VALUE;
        }

        // calculate min max dutysycle (u) given encoder max value:
        uint32_t motor_pwm_range = ( PWM_MAX_CDTY_MOTOR - PWM_MIN_CDTY_MOTOR);
        uint32_t cdty = PWM_MIN_CDTY_MOTOR + ( u * motor_pwm_range ) / ENC_MAX_VALUE ;
        motor_set_duty_cycle_cdty( cdty );


        motor_pi_controller_last_exec = totalMsecs(time_now());
    }
}

void motor_set_dir_bool(uint8_t dir) {

    if (dir) {
        PIOC->PIO_SODR |= PIO_SODR_P23; // endre pin til høy
    } else {
        PIOC->PIO_CODR |= PIO_CODR_P23; // sett pin til lav
    }
}

// PB12 - D7

// PC23 - D20

// 16-84% duty cycle