#ifndef FUNCS_H
#define FUNCS_H

#include <math.h>

#include "../common.h"
#include "../types/pair.h"

inline double func(double x) { return (tan(M_PI_2 - x) - x); }

inline double opt_point(pair segm, size_t i, size_t n) {
  return (0.5 * ((segm.b - segm.a) *
                     cos((2 * (double)i + 1) / (2 * ((double)n + 1)) * M_PI) +
                 (segm.b + segm.a)));
}

#endif
