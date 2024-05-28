#include <stdio.h>
#include <jemalloc/jemalloc.h>

#include "../types/vector.h"
#include "../types/matrix.h"
#include "../types/pair.h"
#include "../common.h"
#include "gauss.h"


vector* qube_spline(vector *points) {
  vector *res = (vector *)malloc(sizeof(vector));
  vector_init(res, points->size - 1, sizeof(vector));

  matrix A;
  matrix_init(&A, 2, 2, sizeof(double));
  matrix_fill_smth(&A, 1.0);

  vector b;
  vector_init(&b, 2, sizeof(double));
  vector_fill_smth(&b, 0.0);
  
  for (size_t i = 0; i < points->size - 1; ++i) {
    matrix_val(&A, 0, 0) = pair_get(points, i).a;
    matrix_val(&A, 1, 0) = pair_get(points, i + 1).a;

    vector_val(&b, 0) = pair_get(points, i).b;
    vector_val(&b, 1) = pair_get(points, i + 1).b;
    
    vector *ais = gauss(&A, &b);
    reverse_vector(ais);
    vector_push(res, ais);
  }

  matrix_free(&A);
  vector_free(&b);
  
  return res;
}
