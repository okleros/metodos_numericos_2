#include <stdio.h>
#include <stdint.h>
#include <math.h>

#ifndef DIFF_METHODS_H
#define DIFF_METHODS_H

typedef enum {
	FW,
	BW,
	CT
} method;

float df(float (*f)(float), float, float, uint8_t, method);
float fw(float (*f)(float), float, float, uint8_t);
float bw(float (*f)(float), float, float, uint8_t);
float ct(float (*f)(float), float, float, uint8_t);

void compute_image_gradients(unsigned char*, unsigned char*, int, int);

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

float df(float (*f)(float), float x_bar, float delta_x, uint8_t dx_order, method m) {
	switch (m) {
		case FW:
			return fw(f, x_bar, delta_x, dx_order);
			break;
		case BW:
			return bw(f, x_bar, delta_x, dx_order);
			break;
		case CT:
			return ct(f, x_bar, delta_x, dx_order);
			break;
		default:
			printf("Method not found, the options are FW, BW and CT. Aborting...");
			return -0xffffffff;
	}
}

void compute_image_gradients(unsigned char* src_img, unsigned char* dst_img, int w, int h) {
	float right_px, left_px, up_px, down_px, gradient_x, gradient_y;
	
	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {
			// uses ct method to compute the derivative approximation
			// (f(x_bar + delta_x) - f(x_bar - delta_x)) / (2*delta_x)
			// for both x and y
			right_px = x+1 < w ? src_img[y*w+(x + 1)]/255.0 : 0.0;
			left_px = x-1 >= 0 ? src_img[y*w+(x - 1)]/255.0 : 0.0;
			gradient_x = (right_px - left_px) / 2.0;
			
			// also padding the image with black (zeroes) if the neighboring
			// pixel falls out of bounds
			down_px = y+1 < w ? src_img[(y+1)*w+x]/255.0 : 0.0;
			up_px = y-1 >= 0 ? src_img[(y-1)*w+x]/255.0 : 0.0;
			gradient_y = (down_px - up_px) / 2.0;
			
			// this computes the magnitude of the gradient using sqrt(gx²+gy²) and
			// assigns this result to the output image * 255 since we're working in
			// the 0 to 1 float range
			dst_img[y*w+x] = 255.0*fabs(sqrt(gradient_x*gradient_x+gradient_y*gradient_y));
		}
	}
}

#endif
#endif