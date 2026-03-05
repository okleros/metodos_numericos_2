#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define DIFF_METHODS_IMPLEMENTATION
#include "diff_methods.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

float f(float x) {
	return x*x*x + 9*x*x;
}

void compute_image_gradients(unsigned char*, unsigned char*, int, int);

int main(int argc, char const *argv[])
{
	const char* filename = "assets/coins.png";
	
	int w, h, c;

	// loads the image from disk, normally you would have to convert the image to 
	// grayscale but in this case the image is already in this format
	unsigned char *data = stbi_load(filename, &w, &h, &c, 0);
	printf("%s info: W = %d, H = %d, C = %d", filename, w, h, c);

	// initialize empty image, normally you would need 3 channels to represent an
	// image but in this case we are using only one since it's in grayscale format
	unsigned char *pixels = (unsigned char*)malloc(w*h*sizeof(unsigned char));

	compute_image_gradients(data, pixels, w, h);

	// saves result image to disk
	stbi_write_bmp("assets/coins_edges.bmp", w, h, c, pixels);

	return 0;
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