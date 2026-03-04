#include <stdio.h>

#define DIFF_METHODS_IMPLEMENTATION
#include "diff_methods.h"

float f(float x) {
	return x*x*x + 9*x*x;
}

int main(int argc, char const *argv[])
{
	printf("Derivative using fw = %f\n", fw(&f, 2.0, 0.1, 1));
	printf("Derivative using bw = %f\n", bw(&f, 2.0, 0.1, 1));
	printf("Derivative using ct = %f\n", ct(&f, 2.0, 0.1, 1));	

	return 0;
}