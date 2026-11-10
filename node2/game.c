#include "game.h"

static uint8_t lives = 5;

void play_pong(){
    
    //show menu
    //pick player
    //play game
    //lose 
}

void during_game(){

    //if (is_diode_blocked()) {
    //    lives -= 1;
    //}
}

bool is_game_over(){

    if (lives == 0) {
        return true;
    }
    return false;
}

void reset_game(){

    lives = 5;
}