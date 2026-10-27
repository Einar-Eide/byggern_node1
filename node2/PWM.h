#pragma once

#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "uart.h"


void pwm_init();

void pwm_set_duty_cycle_cdty(uint32_t CPRD);
void pwm_set_duty_cycle_from_byte( uint8_t b );

void pwm_start();

void pwm_stop();





#define PWM_CPRD_SERVO 52500       // 20ms at MCK/32 (2.625MHz)

#define PWM_MAX_CDTY_SERVO 5250    // 2ms
#define PWM_MIN_CDTY_SERVO 2625    // 1ms
#define PWM_MID_CDTY_SERVO 3938    // 1.5ms