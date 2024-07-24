#include <jemalloc/jemalloc.h>
#include <stdio.h>

#include "../common.h"
#include "../types/matrix.h"
#include "../types/pair.h"
#include "../types/vector.h"
#include "gauss.h"

vector *linear_spline(vector *points, size_t index, vector *res) {
  if (res == NULL || index > points->size - 2) {
    return NULL;
  }

  matrix A;
  matrix_init(&A, 2, 2, sizeof(double));
  matrix_fill_smth(&A, 1.0);

  vector b;
  vector_init(&b, 2, sizeof(double));
  vector_fill_smth(&b, 0.0);

  for (size_t i = index; i < index + 1; ++i) {
    matrix_val(&A, 0, 0) = pair_get(points, i).a;
    matrix_val(&A, 1, 0) = pair_get(points, i + 1).a;

    vector_val(&b, 0) = pair_get(points, i).b;
    vector_val(&b, 1) = pair_get(points, i + 1).b;

    vector *ais = gauss(&A, &b);
    vector_reverse(ais);
    vector_push(res, ais);
  }

  matrix_free(&A);
  vector_free(&b);

  return res;
}
