#pragma once
#include <random>
#include <ctime>
#include <assert.h>

int findPrecision(double x) {
	assert(x >= 0);
	for (int i = 0; (x * pow(10, i)) > ((int)(x * pow(10, i))); ++i) { }
	return ++i;
}

/* lowest , highest (both positive)*/
double pickNumber(double a, double b) {
	assert((a >= 0) && (b >= 0));
	assert(a < b);

	short increment;
	(findPrecision(a) > findPrecision(b))?increment = pow(10, findPrecision(a)):increment = pow(10, findPrecision(b));
	(!increment)? return (double)((rand() % ((int)((b - a)  + 1))) + a):return (double)( (rand() % ((int)( ((b-a) * increment) + 1)) ) + (a*increment) )/increment;
}
