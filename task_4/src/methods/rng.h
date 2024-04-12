#ifndef RNG_H
#define RNG_H

#include <stdlib.h>
#include <time.h>

inline double rng(double bound_a, double bound_b) {
  return (bound_a + (double)rand() / (RAND_MAX / (bound_b - bound_a)));
}

#endif
