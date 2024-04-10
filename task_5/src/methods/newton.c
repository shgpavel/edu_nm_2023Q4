#include <stdio.h>
#include <jemalloc/jemalloc.h>

#include "polynoms.h"
#include "../common.h"
#include "../types/pair.h"
#include "../types/vector.h"

inline double div_diff(vector *points, size_t i, size_t j) {
  return ((unwrap_pair(vector_get(points, i)).b
      - unwrap_pair(vector_get(points, j)).b)
      / (unwrap_pair(vector_get(points, i)).a
        - unwrap_pair(vector_get(points, j)).a));
}

vector* newton_poly(vector *points) {
  vector *result = (vector*)malloc(sizeof(vector));
  vector_init(result, points->size, sizeof(double));
  vector_push(result, &unwrap_pair(vector_get(points, 0)).b);

  //for (size_t i = 0; i < points->size; ++i) {
      
  //}
  vector_print(result); 
  return result;
}
