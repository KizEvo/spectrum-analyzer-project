#ifndef DFT_H
#define DFT_H

#include <stdint.h>
#include "adc.h"


#define COMPLEX_LENGTH		(SAMPLES_LENGTH)

#define PI                  (3.1415926)

#define s_3_deg(x)          (float)((x*x*x)/(6.0))
#define s_5_deg(x)          (float)((x*x*x*x*x)/(120.0))
#define s_7_deg(x)          (float)((x*x*x*x*x*x*x)/(5040.0))
#define s_9_deg(x)          (float)((x*x*x*x*x*x*x*x*x)/(362880.0))
#define sin_calc(x)         (float)((x) - (s_3_deg(x)) + (s_5_deg(x)) - (s_7_deg(x)) + (s_9_deg(x)))

#define c_2_deg(x)          (float)((x*x)/(2.0))
#define c_4_deg(x)          (float)((x*x*x*x)/(24.0))
#define c_6_deg(x)          (float)((x*x*x*x*x*x)/(720.0))
#define c_8_deg(x)          (float)((x*x*x*x*x*x*x*x)/(40320.0))
#define cos_calc(x)         (float)((1) - (c_2_deg(x)) + (c_4_deg(x)) - (c_6_deg(x)) + (c_8_deg(x)))

typedef struct {
    float a;
    float b;
} Complex;

extern Complex X_Complex[COMPLEX_LENGTH];
extern uint8_t freqs[COMPLEX_LENGTH];

float my_sqrt(float number);
float my_sin(float x);
float my_cos(float x);
void dft(volatile float *xn, Complex *xk, uint16_t length);
void freqs_normalize(uint8_t *f, Complex *xk, uint16_t length);

#endif