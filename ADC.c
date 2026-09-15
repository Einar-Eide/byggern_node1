#include "ADC.h"


void get_position(volatile uint8_t *joy_x, volatile uint8_t *joy_y, volatile uint8_t *sli_x, volatile uint8_t * sli_y) {

    volatile char* p_ADC = (char *)0x1200;

    *p_ADC = 0;
    //_delay_us(1); 


    _delay_us(5000);

    *joy_x = *p_ADC;
    *joy_y = *p_ADC;

    *sli_x = *p_ADC;
    *sli_y = *p_ADC; 

    

    //printf("verdier: %d %d %d %d\n\r", *joy_x, *joy_y, *sli_x, *sli_y);

    

}

/**/

    typedef enum {LEFT, RIGHT, UP, DOWN, NEUTRAL} Dir;

    const char *dir_to_string(Dir dir){    
    switch (dir) {
        case LEFT:    return "LEFT";
        case RIGHT:   return "RIGHT";
        case UP:      return "UP";
        case DOWN:    return "DOWN";
        case NEUTRAL: return "NEUTRAL";
        default:      return "UNKNOWN";
        }   
    }

void get_direction(volatile uint8_t *joy_x, volatile uint8_t *joy_y, volatile uint8_t *sli_x, volatile uint8_t *sli_y) {

    Dir dir_joy_x = NEUTRAL;
    Dir dir_joy_y = NEUTRAL;
    Dir dir_sli_x = NEUTRAL;
    Dir dir_sli_y = NEUTRAL;

    int neutral_pos_x = 160;
    int neutral_pos_y = 160;

    if (*joy_x < neutral_pos_x-5) {
        dir_joy_x = LEFT;       
    } else if (*joy_x > neutral_pos_x + 5) {
        dir_joy_x = RIGHT;
    }   
    
    if (*joy_y < neutral_pos_y-5) {
        dir_joy_y = DOWN;       
    } else if (*joy_y > neutral_pos_y+5) {
        dir_joy_y = UP;
    }
    
    if (*sli_x < 122) {
        dir_sli_x = DOWN;       
    } else if (*sli_x > 132) {
        dir_sli_x = UP;
    }   
    
     if (*sli_y < 122) {
        dir_sli_y = LEFT;       
    } else if (*sli_y > 132) {
        dir_sli_y = RIGHT;
    }

    printf("Dir joy x: %s, Dir joy y: %s, Dir sli x: %s, Dir sli y: %s\n\r",
       dir_to_string(dir_joy_x),
       dir_to_string(dir_joy_y),
       dir_to_string(dir_sli_x),
       dir_to_string(dir_sli_y));

}

void calibrate_joy(uint8_t *joy_x_min, uint8_t *joy_x_max, uint8_t *joy_y_min, uint8_t *joy_y_max){
    printf("Starting Calibration for Joystick!\n\r");
    
    // set up values
    uint8_t joy_x = 0;
    uint8_t joy_y = 0;

    uint8_t sli_x = 0;
    uint8_t sli_y = 0;

    uint8_t neutral_joy_x = 0;
    uint8_t neutral_joy_y = 0;

    uint8_t callibration_samples = 100;
    uint16_t hold_delay_ms = 1000; 

    get_position(&joy_x, &joy_y, &sli_x, &sli_y);
    


    // ###########################
    //  Callibrate joy_x
    // ###########################
    
    // Neutral
    // left
    printf("Hold joystick in NEUTRAL\n\r");

    for(int i=0; i<callibration_samples; i++){
        get_position(&joy_x, &joy_y, &sli_x, &sli_y); 
    
        neutral_joy_x += joy_x;
        neutral_joy_y += joy_y; 
    }

    neutral_joy_x /= callibration_samples;
    neutral_joy_y /= callibration_samples;

    // left
    printf("Move joyStick to the LEFT\n\r");
    
    while (joy_x > 100) {
        get_position(&joy_x, &joy_y, &sli_x, &sli_y);
    }

    printf("Hold!\n\r");
    _delay_ms(hold_delay_ms);
    
  
    *joy_x_min = joy_x;

    for(int i=0; i<callibration_samples; i++){
        get_position(&joy_x, &joy_y, &sli_x, &sli_y); 
        
        if (*joy_x_min > joy_x) {
            *joy_x_min = joy_x;
        }
    }
    
    // right

    printf("Move joyStick to the RIGHT\n\r");
    
    while (joy_x < 200) {
        get_position(&joy_x, &joy_y, &sli_x, &sli_y);
    }

    printf("Hold!\n\r");
    _delay_ms(hold_delay_ms);

    *joy_x_max = joy_x;

    for(int i=0; i<callibration_samples; i++){
        get_position(&joy_x, &joy_y, &sli_x, &sli_y);   
        
        if (*joy_x_max < joy_x) {
            *joy_x_max = joy_x;
        }
    }
    

    // ###########################
    //      Callibrate joy_y
    // ###########################

    // down
    printf("Move joyStick to the DOWN\n\r");
    
    while (joy_y > 100) {
        get_position(&joy_x, &joy_y, &sli_x, &sli_y);
    }

    printf("Hold!\n\r");
    _delay_ms(hold_delay_ms);

    *joy_y_min = joy_y;

    for(int i=0; i<callibration_samples; i++){
        get_position(&joy_x, &joy_y, &sli_x, &sli_y);   
        
        if (*joy_y_min > joy_y) {
            *joy_y_min = joy_y;
        }
    }

    // up

    printf("Move joyStick to the UP\n\r");
    
    while (joy_y < 200) {
        get_position(&joy_x, &joy_y, &sli_x, &sli_y);
    }

    printf("Hold!\n\r");
    _delay_ms(hold_delay_ms);

    *joy_y_max = joy_y;

    for(int i=0; i<callibration_samples; i++){
        get_position(&joy_x, &joy_y, &sli_x, &sli_y);   
        
        if (*joy_y_max < joy_y) {
            *joy_y_max = joy_y;
        }
    }





    printf("Finnished calibration of joystick!\n\r");
    printf("joy_x min: %d\t\tmax: %d\n\r", (*joy_x_min), (*joy_x_max));
    printf("joy_y min: %d\t\tmax: %d\n\r", (*joy_y_min), (*joy_y_max));
    
} 
