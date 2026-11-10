#include "menu.h"
#include "IO_inputs.h"

MENU_state g_current_menu_state = MENU;
MENU_state g_prev_menu_state = MENU;

uint8_t g_menu_position = 0;
uint8_t *g_p_menu_position = &g_menu_position;
uint8_t g_screen_should_updated = 1;

volatile uint8_t joy_x_1;
volatile uint8_t joy_y_1;
volatile Dir dir_joy_x_1;
volatile Dir dir_joy_y_1;

volatile uint8_t *p_joy_x = &joy_x_1;
volatile uint8_t *p_joy_y= &joy_y_1;
volatile Dir *p_dir_joy_x = &dir_joy_x_1;
volatile Dir *p_dir_joy_y = &dir_joy_y_1;


const char g_menu_options[3][10] = {
    "Serine",
    "Einar",
    "Lea"
};

const uint8_t g_menu_options_length = sizeof(g_menu_options) / sizeof(g_menu_options[0]);

void MENU_update() {

    // se om menystaten har endret seg
    if (g_current_menu_state != g_prev_menu_state) {
        OLED_clear_dispay();
        g_screen_should_updated = 1;
    }
    g_prev_menu_state = g_current_menu_state;

    // state machine
    switch (g_current_menu_state)
    {
    case MENU:
        MENU_scroll();
        MENU_draw();
        if(button_pressed()){
            g_current_menu_state = (MENU_state)(g_menu_position+1);
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
            g_current_menu_state = MENU;
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
            g_current_menu_state = MENU;
        }
        break;
    
    case LEA:
        OLED_printf_1("HEI LEA!", OLED_CENTER,4);
        if(button_pressed()){
            g_current_menu_state = MENU;
        }
        break;

    } 
}


void MENU_draw(){

    if( g_screen_should_updated ){

        OLED_clear_dispay();

        OLED_go_to(SCREEN_CENTER_COLUMN, 0);
        
        for(int i=0; i<g_menu_options_length; i++){
            OLED_go_to(SCREEN_CENTER_COLUMN, i);
            char* s = g_menu_options[i];
            OLED_printf_2(s, OLED_CENTER);
        }
        uint8_t pointer_column_position = SCREEN_CENTER_COLUMN - strlen(g_menu_options[g_menu_position]) * 6/2 - 2;
        OLED_go_to(pointer_column_position, g_menu_position);
        OLED_printf_2(">", OLED_LEFT);

        g_screen_should_updated = 0;
    }
}


void MENU_scroll(){

    static Dir prev_y_dir = JOY_NEUTRAL;
    static Dir prev_x_dir = JOY_NEUTRAL;

    Dir dir_joy_x;
    Dir dir_joy_y;

    get_joy_direction(&dir_joy_x, &dir_joy_y);
    
    if (prev_y_dir != dir_joy_y){
        if (dir_joy_y == JOY_UP){
            if(g_menu_position < g_menu_options_length -1){
                g_menu_position++;
                g_screen_should_updated = 1;
            }
        }
        else if (dir_joy_y == JOY_DOWN){
            if(g_menu_position > 0){
                g_menu_position--;
                g_screen_should_updated = 1;
            }
        }
    }

    prev_y_dir = dir_joy_y;
    prev_x_dir = dir_joy_x;
}