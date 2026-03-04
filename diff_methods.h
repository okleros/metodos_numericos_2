#include <math.h>
#include <stdint.h>

#ifndef DIFF_METHODS_H
#define DIFF_METHODS_H

float fw(float (*f)(float), float x_bar, float delta_x, uint8_t dx_order);
float bw(float (*f)(float), float x_bar, float delta_x, uint8_t dx_order);
float ct(float (*f)(float), float x_bar, float delta_x, uint8_t dx_order);

#ifdef DIFF_METHODS_IMPLEMENTATION
float fw(float (*f)(float), float x_bar, float delta_x, uint8_t dx_order){
	if (dx_order > 1)
		return fw(f, (f(x_bar + delta_x) - f(x_bar)) / delta_x, delta_x, dx_order-1);
	else
		return (f(x_bar + delta_x) - f(x_bar)) / delta_x;
}

float bw(float (*f)(float), float x_bar, float delta_x, uint8_t dx_order){
	if (dx_order > 1)
		return bw(f, (f(x_bar) - f(x_bar - delta_x)) / delta_x, delta_x, dx_order-1);
	else
		return (f(x_bar) - f(x_bar - delta_x)) / delta_x;
}

float ct(float (*f)(float), float x_bar, float delta_x, uint8_t dx_order){
	if (dx_order > 1)
		return ct(f, (f(x_bar + delta_x) - f(x_bar - delta_x)) / (2*delta_x), delta_x, dx_order-1);
	else
		return (f(x_bar + delta_x) - f(x_bar - delta_x)) / (2*delta_x);
}

#endif
#endif