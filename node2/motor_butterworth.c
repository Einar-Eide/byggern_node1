#include "motor_butterworth.h"

#define Q15_SHIFT 15

void butterworth_init(Butterworth2ndOrder *filter) {

    // Hardcoded coefficients (Example for 5Hz cutoff at 1000Hz sample rate)
    filter->b0 = 679;
    filter->b1 = 1359;
    filter->b2 = 679;
    filter->a1 = -51853;
    filter->a2 = 21838;
    
    // Reset history states
    filter->x1 = 0; filter->x2 = 0;
    filter->y1 = 0; filter->y2 = 0;
}

int32_t butterworth_process(Butterworth2ndOrder *filter, int16_t input) {
    int64_t accumulator = 0;

    accumulator += (int32_t)filter->b0 * input;
    accumulator += (int32_t)filter->b1 * filter->x1;
    accumulator += (int32_t)filter->b2 * filter->x2;
    accumulator -= (int32_t)filter->a1 * filter->y1;
    accumulator -= (int32_t)filter->a2 * filter->y2;

    // Scale back down from Q30 to Q15 with rounding
    int32_t output = (int32_t)((accumulator + (1 << (Q15_SHIFT - 1))) >> Q15_SHIFT);

    // Update history
    filter->x2 = filter->x1;
    filter->x1 = input;
    filter->y2 = filter->y1;
    filter->y1 = output;

    return output;
}
