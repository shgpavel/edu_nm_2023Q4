#include <stdio.h>
#include <jemalloc/jemalloc.h>

#include "polynoms.h"
#include "../common.h"
#include "../funcs/funcs.h"
#include "../types/pair.h"
#include "../types/vector.h"

vector* newton_poly(vector *points) {
  vector *result = (vector*)malloc(sizeof(vector));
  vector tmp;
  
  vector tmp_small;
  vector_init(result, points->size, sizeof(double));
  vector_init(&tmp, points->size, sizeof(double));
  vector_init(&tmp_small, 2, sizeof(double));
  vector_push(result, &unwrap_pair(vector_get(points, 0)).b);
  vector_fill_zero(&tmp_small);

  double g = 1;
  vector_push()

  for (size_t i = 0; i < points->size; ++i) {
    for (size_t j = 1; j < points->size; ++j) {
      if (i != j) {
        g /= unwrap_pair(vector_get(points, i)).a - 
              unwrap_pair(vector_get(points, j)).a;
      }
    }
    g *= unwrap_pair(vector_get(points, i)).b;
    for (size_t j = 0; j < i; ++j) {
      vector_change();
    }
  }


  vector_print(result); 
  return result;
}
