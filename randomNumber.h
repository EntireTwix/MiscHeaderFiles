#pragma once
#include <random>
#include <ctime>
#include <assert.h>

int findPrecision(double x) {
	assert(x >= 0);
	int i = 0;
	for (; (x * pow(10, i)) > ((int)(x * pow(10, i))); ++i);
	
	++i;
	return i;
}


/* lowest , highest (both positive)*/
double pickNumber(double a, double b) {
	assert((a >= 0) && (b >= 0));
	assert(a < b);

	short increment;
	if (findPrecision(a) > findPrecision(b)) {
		//if A is more precise
		increment = pow(10, findPrecision(a));
	}
	else {
		//if B is more precise
		increment = pow(10, findPrecision(b));
	}

	if(increment == 0){
		//random number that is between 0 and (highest-lowest)+1, then add lowest to the result
		return (double)((rand() % ((int)((b - a)  + 1))) + a);
	}
	else {
		//otherwise add precision to random result
		return (double)( (rand() % ((int)( ((b-a) * increment) + 1)) ) + (a*increment) )/increment;
	}
}
