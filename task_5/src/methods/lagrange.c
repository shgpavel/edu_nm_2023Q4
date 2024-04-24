#include <stdio.h>
#include <jemalloc/jemalloc.h>

#include "polynoms.h"
#include "../common.h"
#include "../types/pair.h"
#include "../types/vector.h"

vector* lagrange_poly(vector *points) {
  vector *result = (vector*)malloc(sizeof(vector));
  vector *v;
  vector tmp;
  vector_init(result, points->size, sizeof(double));
  vector_init(&tmp, 2, sizeof(double));
  vector_fill_zero(result);
  vector_fill_zero(&tmp);
  
  double tmp_num;
  tmp_num = 1;
  vector_change(&tmp, 1, &tmp_num);
  
  for (size_t i = 0; i < points->size; ++i) {
    for (size_t j = 0; j < points->size; ++j) {
      if (j == i) continue;
      
      tmp_num = -(unwrap_pair(vector_get(points, j)).a);
      vector_change(&tmp, 0, &tmp_num);
      
      if (j == 0 || (j == 1 && i == 0)) {
        vector_assign(v, &tmp);
      } else {
        v = poly_mult(v, &tmp);
      } 
    }

    for (size_t j = 0; j < points->size; ++j) {
      if (j != i) {
        vector_mult(v, 
        1 / (unwrap_pair(vector_get(points, i)).a -
        unwrap_pair(vector_get(points, j)).a));
      }
    }
    vector_mult(v, unwrap_pair(vector_get(points, i)).b);
    poly_sum(result, v);
  }
  
  vector_free(&tmp);
  vector_free(v);
  return result;
}

