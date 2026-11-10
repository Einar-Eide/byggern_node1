#include "game.h"
#include "can.h"
#include "CAN_ID.h"

static uint8_t lives = 5;

void play_pong(){
    
    //show menu
    //pick player
    //play game
    //lose 
}

void during_game(){

    //if (is_diode_blocked()) {
    //    lose_life();
    //}
}

bool is_game_over(){

    if (lives == 0) {
        return true;
    }
    return false;
}

uint8_t game_reset(){
    uint8_t lives = GAME_LIVES;
    return lives;
}

void lose_life(uint8_t *lives) {
    lives -= 1;
}