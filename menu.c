#include "menu.h"
#include "IO_inputs.h"

MENU_state current_menu_state = MENU;
MENU_state prev_menu_state = MENU;

uint8_t menu_position = 0;
uint8_t *p_menu_position = &menu_position;
uint8_t screen_should_updated = 1;

volatile uint8_t joy_x_1;
volatile uint8_t joy_y_1;
volatile Dir dir_joy_x_1;
volatile Dir dir_joy_y_1;

volatile uint8_t *p_joy_x = &joy_x_1;
volatile uint8_t *p_joy_y= &joy_y_1;
volatile Dir *p_dir_joy_x = &dir_joy_x_1;
volatile Dir *p_dir_joy_y = &dir_joy_y_1;


void MENU_init(){
    DDRB &= ~(1 << DDB0); 
    OLED_init();

    //Button init
    
}

void MENU_update() {

    if (current_menu_state != prev_menu_state) {
        OLED_clear_dispay();
        screen_should_updated = 1;
    }

    prev_menu_state = current_menu_state;


    switch (current_menu_state)
    {
    case MENU:
        MENU_scroll(p_menu_position, p_joy_x, p_joy_y, p_dir_joy_x, p_dir_joy_y);
        MENU_draw();
        if(button_pressed()){
            current_menu_state = (MENU_state)(menu_position+1);
        }
        break;
    
    case SERINE:
        /*OLED_printf_1("/\\_/\\\\", OLED_CENTER, 0);
        OLED_printf_1("/     \\", OLED_CENTER, 1);
        OLED_printf_1("(  o o  )", OLED_CENTER, 2);
        OLED_printf_1("/   ^   \\", OLED_CENTER, 3);
        OLED_printf_1("(  \\___/  )", OLED_CENTER, 4);
        OLED_printf_1("\\       /", OLED_CENTER, 5);
        OLED_printf_1("|     |", OLED_CENTER, 6);
        OLED_printf_1("(_|_|_)", OLED_CENTER, 7);*/
        OLED_printf_1("HEI SERINE!", OLED_CENTER,4);
        if(button_pressed()){
            current_menu_state = MENU;
        }
        break;
    
    case EINAR:
        /*OLED_printf_1(".---.  .---.", OLED_CENTER, 0);
        OLED_printf_1("/  o \\/  o  \\", OLED_CENTER, 1);
        OLED_printf_1("(      ^      )", OLED_CENTER, 2);
        OLED_printf_1("\\    ---    /", OLED_CENTER, 3);
        OLED_printf_1("'._     _.'", OLED_CENTER, 4);
        OLED_printf_1("/|     |\\", OLED_CENTER, 5);
        OLED_printf_1("__/ |     | \\__", OLED_CENTER, 6);
        OLED_printf_1("(____/     \\____)", OLED_CENTER, 7);*/
        OLED_printf_1("HEI EINAR!", OLED_CENTER,4);
        if(button_pressed()){
            current_menu_state = MENU;
        }
        break;
    
    case LEA:
        OLED_printf_1("HEI LEA!", OLED_CENTER,4);
        if(button_pressed()){
            current_menu_state = MENU;
        }
        break;
    }
    
}


const char menu_options[3][10] = {
        "Serine",
        "Einar",
        "Lea"
    };

const uint8_t menu_options_length = sizeof(menu_options) / sizeof(menu_options[0]);


void MENU_draw(){

    if( screen_should_updated ){

        OLED_clear_dispay();

        OLED_go_to(SCREEN_CENTER_COLUMN, 0);
        
        for(int i=0; i<menu_options_length; i++){
            OLED_go_to(SCREEN_CENTER_COLUMN, i);
            char* s = menu_options[i];
            OLED_printf_2(s, OLED_CENTER);
        }
        uint8_t pointer_column_position = SCREEN_CENTER_COLUMN - strlen(menu_options[menu_position]) * 6/2 - 2;
        OLED_go_to(pointer_column_position, menu_position);
        OLED_printf_2(">", OLED_LEFT);

        screen_should_updated = 0;
    }

}


void MENU_scroll(uint8_t *pos, volatile uint8_t *joy_x, volatile uint8_t *joy_y,  volatile Dir *dir_joy_x, volatile Dir *dir_joy_y){
    Dir prev_y_dir = *dir_joy_y;
    Dir prev_x_dir = *dir_joy_x;

    get_joy_direction(joy_x, joy_y, dir_joy_x, dir_joy_y);
    
    if (prev_y_dir != *dir_joy_y){
        if (*dir_joy_y == JOY_UP){
            if(*pos < menu_options_length -1){
                (*pos)++;
                screen_should_updated = 1;
            }
        return;
        }
        if (*dir_joy_y == JOY_DOWN){
            if(*pos > 0){
                (*pos)--;
                screen_should_updated = 1;
            }
        return;
        }}
    return;
}