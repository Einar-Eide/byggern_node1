#include "IO_inputs.h"

uint8_t g_joy_x_min = 70;
uint8_t g_joy_x_max = 255;
uint8_t g_joy_y_min = 70;
uint8_t g_joy_y_max = 255;



void get_position_raw(volatile uint8_t *joy_x, volatile uint8_t *joy_y, volatile uint8_t *sli_x, volatile uint8_t * sli_y) {

    volatile char* p_ADC = (char *)0x1200;

    *p_ADC = 0;

    _delay_us(100); // random value ?? var 5000 før

    *joy_x = *p_ADC;
    *joy_y = *p_ADC;

    *sli_x = *p_ADC;
    *sli_y = *p_ADC; 

    //printf("verdier: %d %d %d %d\n\r", *joy_x, *joy_y, *sli_x, *sli_y);
}

const char *dir_to_string(Dir dir){    
switch (dir) {
    case JOY_LEFT:    return "LEFT";
    case JOY_RIGHT:   return "RIGHT";
    case JOY_UP:      return "UP";
    case JOY_DOWN:    return "DOWN";
    case JOY_NEUTRAL: return "NEUTRAL";
    default:          return "UNKNOWN";
    }   
}

void joy_get_analog_position(int* analog_x, int* analog_y){
    
    uint8_t joy_x = 0;
    uint8_t joy_y = 0;

    get_position_raw(&joy_x, &joy_y, NULL, NULL);
    
    int x_range = g_joy_x_max - g_joy_x_min;
    int y_range = g_joy_y_max - g_joy_y_min;

    *analog_x = -100 + (200 * (float)(joy_x - g_joy_x_min))/x_range;
    *analog_y = -100 + (200 * (float)(joy_y - g_joy_y_min))/y_range;

    //Sikrer at posisjonen aldri kan bli større enn 100 eller mindre enn -100 selv med upresis kalibrering
    if (*analog_x < -100) {*analog_x = -100;}   
    if (*analog_x > 100) {*analog_x = 100;}
    if (*analog_y < -100) {*analog_y = -100;}   
    if (*analog_y > 100) {*analog_y = 100;}
}

// not needed ??
void joy_get_direction(volatile uint8_t *joy_x, volatile uint8_t *joy_y, volatile uint8_t *sli_x, volatile uint8_t *sli_y) {

    Dir dir_joy_x = JOY_NEUTRAL;
    Dir dir_joy_y = JOY_NEUTRAL;
    Dir dir_sli_x = JOY_NEUTRAL;
    Dir dir_sli_y = JOY_NEUTRAL;

    int neutral_pos_x = 160;
    int neutral_pos_y = 160;

    if (*joy_x < neutral_pos_x-10) {
        dir_joy_x = JOY_LEFT;       
    } else if (*joy_x > neutral_pos_x + 10) {
        dir_joy_x = JOY_RIGHT;
    }   
    
    if (*joy_y < neutral_pos_y-10) {
        dir_joy_y = JOY_DOWN;       
    } else if (*joy_y > neutral_pos_y+10) {
        dir_joy_y = JOY_UP;
    }
    
    if (*sli_x < 122) {
        dir_sli_x = JOY_DOWN;       
    } else if (*sli_x > 132) {
        dir_sli_x = JOY_UP;
    }   
    
     if (*sli_y < 122) {
        dir_sli_y = JOY_LEFT;       
    } else if (*sli_y > 132) {
        dir_sli_y = JOY_RIGHT;
    }

    //printf("Dir joy x: %s, Dir joy y: %s, Dir sli x: %s, Dir sli y: %s\n\r", dir_to_string(dir_joy_x), dir_to_string(dir_joy_y), dir_to_string(dir_sli_x), dir_to_string(dir_sli_y));
}


/**
 * returns the direction of the joystick as a Dri variable for x and y direction
 */
void get_joy_direction(volatile Dir *dir_joy_x, volatile Dir *dir_joy_y ){
    
    int joy_x;
    int joy_y;
    joy_get_analog_position(&joy_x, &joy_y);
    
    int neutral_pos_x = 0;
    int neutral_pos_y = 0;

    if (joy_x < neutral_pos_x - 80) {
        *dir_joy_x = JOY_LEFT;       
    } else if (joy_x > neutral_pos_x + 80) {
        *dir_joy_x = JOY_RIGHT;
    } else {*dir_joy_x = JOY_NEUTRAL;}
    
    if (joy_y < neutral_pos_y - 80) {
        *dir_joy_y = JOY_DOWN;       
    } else if (joy_y > neutral_pos_y + 80) {
        *dir_joy_y = JOY_UP;
    } else {*dir_joy_y = JOY_NEUTRAL;}

    //printf("Dir joy x: %s, Dir joy y: %s\n\r", dir_to_string(*dir_joy_x), dir_to_string(*dir_joy_y));
}







void calibrate_joy(){
    printf("Starting Calibration for Joystick!\n\r");
    
    // set up values
    uint8_t joy_x = 0;
    uint8_t joy_y = 0;

    uint8_t sli_x = 0;
    uint8_t sli_y = 0;

    uint32_t neutral_joy_x = 0;
    uint32_t neutral_joy_y = 0;

    uint8_t callibration_samples = 25;
    uint16_t hold_delay_ms = 0; 


    printf("joy_x: %d\n\r", joy_x);

    get_position_raw(&joy_x, &joy_y, &sli_x, &sli_y);
    
    printf("joy_x: %d\n\r", joy_x);

    // ###########################
    //  Callibrate joy_x
    // ###########################
    
    // Neutral
    // left
    printf("Hold joystick in NEUTRAL\n\r");

    for(int i=0; i<callibration_samples; i++){
        get_position_raw(&joy_x, &joy_y, &sli_x, &sli_y); 
    
        neutral_joy_x += joy_x;
        neutral_joy_y += joy_y; 
    }

    neutral_joy_x /= callibration_samples;
    neutral_joy_y /= callibration_samples;

    // left
    printf("Move joyStick to the LEFT\n\r");
    
    while (joy_x > 100) {
        get_position_raw(&joy_x, &joy_y, &sli_x, &sli_y);
        _delay_ms(10);
    }

    printf("Hold!\n\r");
    _delay_ms(hold_delay_ms);

    g_joy_x_min = joy_x;

    for(int i=0; i<callibration_samples; i++){
        get_position_raw(&joy_x, &joy_y, &sli_x, &sli_y); 
        _delay_ms(10);
        
        if (g_joy_x_min > joy_x) {
            g_joy_x_min = joy_x;
        }
    }
    
    // right

    printf("Move joyStick to the RIGHT\n\r");
    
    while (joy_x < 200) {
        get_position_raw(&joy_x, &joy_y, &sli_x, &sli_y);
    }

    printf("Hold!\n\r");
    _delay_ms(hold_delay_ms);

    g_joy_x_max = joy_x;

    for(int i=0; i<callibration_samples; i++){
        get_position_raw(&joy_x, &joy_y, &sli_x, &sli_y);   
        _delay_ms(10);

        if (g_joy_x_max < joy_x) {
            g_joy_x_max = joy_x;
        }
    }
    

    // ###########################
    //      Callibrate joy_y
    // ###########################

    // down
    printf("Move joyStick DOWN\n\r");
    
    while (joy_y > 100) {
        get_position_raw(&joy_x, &joy_y, &sli_x, &sli_y);
        _delay_ms(10);
    }

    printf("Hold!\n\r");
    _delay_ms(hold_delay_ms);

    g_joy_y_min = joy_y;

    for(int i=0; i<callibration_samples; i++){
        get_position_raw(&joy_x, &joy_y, &sli_x, &sli_y);   
        _delay_ms(10);
        
        if (g_joy_y_min > joy_y) {
            g_joy_y_min = joy_y;
        }
    }

    // up

    printf("Move joyStick UP\n\r");
    
    while (joy_y < 200) {
        get_position_raw(&joy_x, &joy_y, &sli_x, &sli_y);
        _delay_ms(10);
    }

    printf("Hold!\n\r");
    _delay_ms(hold_delay_ms);

    g_joy_y_max = joy_y;

    for(int i=0; i<callibration_samples; i++){
        get_position_raw(&joy_x, &joy_y, &sli_x, &sli_y);   
        _delay_ms(10);

        if (g_joy_y_max < joy_y) {
            g_joy_y_max = joy_y;
        }
    }





    printf("Finnished calibration of joystick!\n\r");
    printf("joy_x min: %d\t\tmax: %d\n\r", (g_joy_x_min), (g_joy_x_max));
    printf("joy_y min: %d\t\tmax: %d\n\r", (g_joy_y_min), (g_joy_y_max));
    
} 


uint8_t button_down(){
    return (PINB & 0b00000010) != 2;
}

uint8_t button_pressed(){
    static uint8_t prev_button_state = 0;
    uint8_t button_state = button_down();
    
    uint8_t pressed = 0;

    if (button_state && (prev_button_state != 1) ) {
        pressed = 1;
    }

    prev_button_state = button_state;
    return pressed;
}