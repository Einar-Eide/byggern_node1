#pragma once

#include <stdint.h>
#include <math.h>
#include "motor.h"

// This file and the corresponding c file is partially written by AI


// Q15 Scaling macros
#define Q15_SHIFT 15
#define FLOAT_TO_Q15(x) ((int16_t)((x) * (1 << Q15_SHIFT) + ((x) >= 0 ? 0.5 : -0.5)))


// Filter state structure to hold coefficients and history
typedef struct {
    double b0, b1, b2; // Feedforward coefficients
    double a1, a2;     // Feedback coefficients
    double x1, x2;     // Input history (z^-1, z^-2)
    double y1, y2;     // Output history (z^-1, z^-2)
} Butterworth2ndOrder;


void butterworth_init(Butterworth2ndOrder *filter);
int32_t butterworth_process(Butterworth2ndOrder *filter, int16_t input);