#pragma once

#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "uart.h"
#include "PWM.h"
#include "time.h"

typedef enum {JOY_LEFT, JOY_RIGHT, JOY_UP, JOY_DOWN, JOY_NEUTRAL} Dir;

#define PWM_CPRD_MOTOR 105 // 1/f = X * CPRD / MCK => X = MCK / (f * X) = 84000000 / (200000 * 4) = 105

#define PWM_MIN_CDTY_MOTOR 30 // min 16%: CPRD * 0.16 = 16.8
#define PWM_MAX_CDTY_MOTOR 88 // max 84%: CPRD * 0.84 = 88.2
#define PWM_MID_CDTY_MOTOR 53 // (min + max) / 2 = 52.5



// motor controller 
#define MOTOR_POS_CONTROLLER_K_P 1
#define MOTOR_POS_CONTROLLER_K_I 0.5f
#define CODE_EXEC_FREQUENCY 10.0f

#define MOTOR_POS_JOY_TO_ENC_SCALE_FACTOR 22 // 5614 / 255 = 22.0157   
#define ENC_MAX_VALUE 5614



void motor_init();
void motor_set_duty_cycle_from_byte(uint8_t b);

//Uten PI regulator
Dir joy_get_direction(volatile uint8_t *joy_x);
void motor_set_dir(Dir dir);
uint8_t motor_get_speed(volatile uint8_t *joy_x);

//Med PI regulator
void motor_pi_controller(uint8_t r_, uint32_t y);
void motor_set_dir_bool(uint8_t dir);

//void motor_set_duty_cycle_from_byte( uint8_t b );
//void motor_set_duty_cycle_cdty( uint32_t CDTY );

