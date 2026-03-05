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

int main(int argc, char const *argv[])
{
	const char* filename = "assets/coins.png";
	const char* out_filename = "assets/coins_edges.png";
	
	int w, h, c;

	// loads the image from disk, normally you would have to convert the image to 
	// grayscale but in this case the image is already in this format
	unsigned char *data = stbi_load(filename, &w, &h, &c, 0);

	// initialize empty image, normally you would need 3 channels to represent an
	// image but in this case we are using only one since it's in grayscale format
	unsigned char *pixels = (unsigned char*)malloc(w*h*sizeof(unsigned char));

	compute_image_gradients(data, pixels, w, h);

	// saves result image to disk
	stbi_write_png(out_filename, w, h, c, pixels, 0);
	printf("Edges written to %s\n\n", out_filename);

	float x = 2.0;

	printf("Derivative of x³ + 9x² (x = %.2f) with FW approach: %.6f\n", x, df(f, x, 1e-1, 1, FW));
	printf("Derivative of x³ + 9x² (x = %.2f) with BW approach: %.6f\n", x, df(f, x, 1e-1, 1, BW));
	printf("Derivative of x³ + 9x² (x = %.2f) with CT approach: %.6f\n", x, df(f, x, 1e-1, 1, CT));

	return 0;
}