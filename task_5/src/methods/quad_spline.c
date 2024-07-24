#include <jemalloc/jemalloc.h>
#include <stdio.h>

#include "../common.h"
#include "../types/matrix.h"
#include "../types/pair.h"
#include "../types/vector.h"
#include "gauss.h"

vector *quad_spline(vector *points, size_t index, vector *res) {
  if (res == NULL || index > points->size - 3) {
    return NULL;
  }

  matrix A;
  matrix_init(&A, 6, 6, sizeof(double));
  matrix_fill_smth(&A, 0.0);

  vector b;
  vector_init(&b, 6, sizeof(double));
  vector_fill_smth(&b, 0.0);

  vector_val(&b, 0) = pair_get(points, index).b;
  vector_val(&b, 1) = pair_get(points, index + 1).b;
  vector_val(&b, 3) = pair_get(points, index + 1).b;
  vector_val(&b, 4) = pair_get(points, index + 2).b;

  matrix_val(&A, 0, 0) = pair_get(points, index).a * pair_get(points, index).a;
  matrix_val(&A, 0, 1) = pair_get(points, index).a;
  matrix_val(&A, 0, 2) = 1;

  matrix_val(&A, 1, 0) =
      pair_get(points, index + 1).a * pair_get(points, index + 1).a;
  matrix_val(&A, 1, 1) = pair_get(points, index + 1).a;
  matrix_val(&A, 1, 2) = 1;

  matrix_val(&A, 2, 0) = 2 * pair_get(points, index + 1).a;
  matrix_val(&A, 2, 1) = 1;

  matrix_val(&A, 2, 3) = -matrix_val(&A, 2, 0);
  matrix_val(&A, 2, 4) = -matrix_val(&A, 2, 1);

  matrix_val(&A, 3, 3) =
      pair_get(points, index + 1).a * pair_get(points, index + 1).a;
  matrix_val(&A, 3, 4) = pair_get(points, index + 1).a;
  matrix_val(&A, 3, 5) = 1;

  matrix_val(&A, 4, 3) =
      pair_get(points, index + 2).a * pair_get(points, index + 2).a;
  matrix_val(&A, 4, 4) = pair_get(points, index + 2).a;
  matrix_val(&A, 4, 5) = 1;

  matrix_val(&A, 5, 3) = 2 * pair_get(points, index + 2).a;
  matrix_val(&A, 5, 4) = 1;

  vector *ais = gauss(&A, &b);

  vector *normalized_1 = (vector *)malloc(sizeof(vector));
  vector *normalized_2 = (vector *)malloc(sizeof(vector));

  vector_init(normalized_1, 3, sizeof(double));
  vector_init(normalized_2, 3, sizeof(double));
  for (size_t i = 0; i < 3; ++i) {
    vector_push(normalized_1, vector_get(ais, i));
    vector_push(normalized_2, vector_get(ais, i + 3));
  }
  vector_reverse(normalized_1);
  vector_reverse(normalized_2);

  vector_push(res, normalized_1);
  vector_push(res, normalized_2);

  vector_free(ais);
  free(ais);

  matrix_free(&A);
  vector_free(&b);

  return res;
}
