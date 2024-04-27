#include <stdio.h>
#include <math.h>
#include <jemalloc/jemalloc.h>

#include "../types/vector.h"
#include "../types/matrix.h"
#include "../common.h"

#define matrix_val(a, i, j) unwrap_double(matrix_get(a, i, j))
#define vector_val(v, i) unwrap_double(vector_get(v, i))

void finalize(matrix *a, vector *b, vector *solution) {
  for (size_t i = a->rows; i > 0; ) {
    --i;
    vector_val(solution, i) = vector_val(b, i);
    for (size_t j = a->rows; j > i + 1; ) {
      --j;
      vector_val(solution, i) = vector_val(solution, i) -
        matrix_val(a, i, j) * vector_val(solution, j);
    }
    vector_val(solution, i) = vector_val(solution, i) / matrix_val(a, i, i);
  }
}


vector* qr(matrix *a, vector *b) {}

vector *gauss(matrix *m, vector *c) {

  matrix a;
  vector b;
  matrix_init_copy(&a, m);
  vector_init_copy(&b, c);
  
  vector *solution = (vector *)malloc(sizeof(vector));
  vector_init_copy(solution, c);

  
  for (size_t i = 0; i < a.rows; ++i) {
    size_t lead = i;
    for (size_t k = i + 1; k < a.rows; ++k) {
      if (fabs(matrix_val(&a, k, i)) > fabs(matrix_val(&a, lead, i))) lead = k;
    }

    if (lead != i) {
      vector_swap(&b, i, lead);
      for (size_t k = 0; k < a.rows; ++k) {
        matrix_swap(&a, i, k, lead, k);
      }
    }

    for (size_t k = i + 1; k < a.rows; ++k) {
      double fcr = matrix_val(&a, k, i) / matrix_val(&a, i, i);
      for (size_t j = i; j < a.rows; ++j) {
        matrix_val(&a, k, j) = matrix_val(&a, k, j) - fcr * matrix_val(&a, i, j);
      }
      vector_val(&b, k) = vector_val(&b, k) - fcr * vector_val(&b, i);
    }
  }

  finalize(&a, &b, solution);

  matrix_free(&a);
  vector_free(&b);

  return solution;
}
