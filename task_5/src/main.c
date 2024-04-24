#include <stdio.h>

#include "types/vector.h"
#include "types/matrix.h"
#include "types/pair.h"
#include "methods/polynoms.h"
#include "methods/lagrange.h"
#include "methods/newton.h"
#include "methods/lup.h"
#include "draw/draw.h"
#include "funcs/funcs.h"

#include "common.h"

int main(void) {

  pair segm = {1.0, 2.542};

  /*
   evenly distributed nodes  
  for (size_t s = 10; s <= 15; ++s) {
    vector points;
    vector_init(&points, s, sizeof(pair));
    for (size_t i = 0; i < s; ++i) {
      double point = segm.a 
        + (double)i * ((segm.b - segm.a) / (double)(s - 1));
      pair point_p = {point, func(point)};
      vector_push(&points, (void *)&point_p);
    }

    vector *res = lagrange_poly(&points);
    vector_free(&points);
    
    add_func(res);
    vector_free(res);
  }
  */
  /* optimal nodes */
  for (size_t s = 10; s <= 20; ++s) {
    vector points;
    vector_init(&points, s, sizeof(pair));
    for (size_t i = 0; i < s; ++i) {
      double point = opt_point(segm, i, s);
      pair point_p = {point, func(point)};
      vector_push(&points, (void *)&point_p);
    }
    vector *res = lagrange_poly(&points);
    vector_free(&points);
    
    add_func(res);
    vector_free(res);
  }
  
  str_func("cot (x) - x");

  plot();
  return 0;
}
