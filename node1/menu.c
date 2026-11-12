#include "menu.h"

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


Game_state g_game_state;
Game_state* g_p_game_state = &g_game_state;

//what is written to display
const char g_menu_options[5][12] = {
    "Serine",
    "Einar",
    "Lea",
    "GAME SPEED",
    "GAME POS"
    //"PLAY GAME (HARD)"
};

const uint8_t g_menu_options_length = sizeof(g_menu_options) / sizeof(g_menu_options[0]);

//Updates oled display and handles menu state
void MENU_update() {

    uint8_t is_first_iteration_of_new_menu_state = 0;

    // se om menystaten har endret seg
    if (g_current_menu_state != g_prev_menu_state) {
        OLED_clear_dispay();
        g_screen_should_updated = 1;
        is_first_iteration_of_new_menu_state = 1;

    }
    g_prev_menu_state = g_current_menu_state;

    // state machine
    switch (g_current_menu_state)
    {
    case MENU:
        //Set game mode to menu
        g_game_state.game_mode = 0;
        MENU_scroll();
        MENU_draw();

        //When button is pressed, select state based on g_menu_position
        if(button_pressed()){
            g_current_menu_state = (MENU_state)(g_menu_position+1);
            g_screen_should_updated = 1;
        }
        break;
    

    case SERINE:

        // Show text and return when button is pressed
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
        // Show text and return when button is pressed
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
        // Show text and return when button is pressed
        OLED_printf_1("HEI LEA!", OLED_CENTER,4);
        if(button_pressed()){
            g_current_menu_state = MENU;
        }
        break;

    case GAME_JOY_SPEED: 
        g_game_state.game_mode = 1;
        if (is_first_iteration_of_new_menu_state) {
            game_joy_speed_init();
        }
        game_joy_speed();
        break;

    case GAME_JOY_POS: 
        g_game_state.game_mode = 2;
        if (is_first_iteration_of_new_menu_state) {
            game_joy_speed_init();
        }
        game_joy_speed();
        break;

    } 
}

//Draw menu and pointer arrow
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


//reset number of lives
void game_joy_speed_init() {
    g_game_state.lives_left = 3;
}

// Update game display and return when game over
void game_joy_speed() {

    static uint8_t last_lives_left = 0;

    // set screen to update if lives left changes
    if (g_game_state.lives_left != last_lives_left) {
        g_screen_should_updated = 1;
    }

    if( g_screen_should_updated ){
        OLED_clear_dispay();

        if (g_game_state.game_mode == 1) {
            OLED_printf_1("PLAYING IN EASY MODE", OLED_CENTER, 0);
        } else if (g_game_state.game_mode == 2) {
            OLED_printf_1("PLAYING IN HARD MODE", OLED_CENTER, 0);
        }

        if ( g_game_state.lives_left > 0 ) {
            OLED_go_to(40, 2);
            for( int i=0; i<g_game_state.lives_left; i++ ) {
                OLED_print_emoji(1); // hjerte
                OLED_printf(" ");
            }
        } else {
            // out off lives, game over
            OLED_game_over();

            OLED_go_to(60, 4);
            OLED_print_emoji(4); // skull

            _delay_ms(1000);
            g_current_menu_state = MENU;
        }
        

        g_screen_should_updated = 0;
        
    }

    last_lives_left = g_game_state.lives_left;
}

//Move menu selection when joystick is moved up or down
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