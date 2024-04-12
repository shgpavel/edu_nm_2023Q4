#include <stdio.h>

#include "types/vector.h"
#include "types/matrix.h"
#include "types/pair.h"
#include "methods/polynoms.h"
#include "methods/lagrange.h"
#include "methods/newton.h"
#include "methods/lup.h"
#include "draw/draw.h"
#include "func/func.h"

#include "common.h"

#define interval_l 1.542

int main(void) {
 
  /* evenly distributed nodes */
  for (size_t s = 10; s <= 15; ++s) {
    vector points;
    vector_init(&points, s, sizeof(pair));
    for (size_t i = 0; i < s; ++i) {
      double point = 1.0 + (double)i * (interval_l / (double)(s - 1));
      pair point_p = {point, func(point)};
      vector_push(&points, (void *)&point_p);
    }

    vector *res = lagrange_poly(&points);
    vector_free(&points);
    
    add_func(res);
    vector_free(res);
  }

  plot();
  return 0;
}
