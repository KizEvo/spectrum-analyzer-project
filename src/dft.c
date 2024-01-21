#include <stdint.h>
#include <math.h>
#include "dft.h"
#include "gpio.h"

Complex X_Complex[COMPLEX_LENGTH] = {};
uint8_t freqs[COMPLEX_LENGTH] = {0};

float my_sqrt(float number)
{
    float sqrt, temp;
    
    sqrt = number / 2;
    temp = 0;
    
    while(sqrt != temp){
        temp = sqrt;
        sqrt = ( number/temp + temp) / 2;
    }
    return sqrt;
}

float my_sin(float x)
{
    uint16_t temp = 0;
    if (x < (-PI))
    {
        temp = -(x/PI);
        x = temp % 2 == 0 ? (PI * temp + x) : (PI * (temp + 1) + x);
    }
    else if (x > PI)
    {
        temp = x/PI;
        x = temp % 2 == 0 ? (PI * (temp + 1) - x) : (PI * temp - x);
    }
    return sin_calc(x);
}

float my_cos(float x)
{
    uint16_t temp = 0;
    if (x < (-PI))
    {
        temp = -(x/PI);
        x = temp % 2 == 0 ? (PI * temp + x) : (PI * (temp + 1) + x);
    }
    else if (x > PI)
    {
        temp = x/PI;
        x = temp % 2 == 0 ? (PI * temp - x) : (PI * (temp + 1) - x);
    }
    return cos_calc(x);
}

void dft(volatile float *xn, Complex *xk, uint16_t length)
{
    for(int k = 0; k < length; k++)
    {
        Complex temp = {0, 0};
        for(int n = 0; n < length; n++)
        {
            float exponent = 2*PI*k*n/length;
            temp.a += xn[n] * my_cos(exponent);
            temp.b += xn[n] * (-my_sin(exponent));
		}
        xk[k] = temp;
    }
}

void freqs_normalize(uint8_t *f, Complex *xk, uint16_t length)
{
	for(uint16_t i = 0; i < length; i++)
	{
		f[i] = (uint8_t)(my_sqrt(xk[i].a * xk[i].a + xk[i].b * xk[i].b));
		f[i] = f[i] == 1 ? f[i] * 2 : f[i];
	}
}