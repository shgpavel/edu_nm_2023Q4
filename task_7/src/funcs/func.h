#ifndef FUNC_H
#define FUNC_H

#include <math.h>
inline double func(double x) {
  return 3.5 * cos(0.7 * x) * exp(-5*x/3) +
      2.4 * sin(5.5 * x) * exp(-0.75*x) + 5;
}

#define A 1.1
#define B 2.3
#define ALPHA 0.8
#define BETA 0

#endif
