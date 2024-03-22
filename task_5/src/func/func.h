#ifndef FUNC_C
#define FUNC_C

#include <math.h>

#include "../common.h"

inline double func(double x) {
	return (tan(M_PI_2 - x) - x);
}

#endif
