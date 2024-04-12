#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "types/vector.h"
#include "types/matrix.h"
#include "methods/rng.h"
#include "common.h"

#define BOUND_A 1.0
#define BOUND_B 10.0

int main(void) {
  size_t n;
  scanf("%zu", &n);

  srand(time(NULL));
  
  matrix lambda, c;
  matrix_init(&lambda, n, sizeof(double));
  matrix_init(&c, n, sizeof(double));
  matrix_fill_zero(&lambda);
  matrix_fill_zero(&c);

  for (size_t i = 0; i < lambda.rows; ++i) {
    unwrap_double(matrix_get(&lambda, i, i)) = rng(BOUND_A, BOUND_B);
  }

  for (size_t i = 0; i < c.rows; ++i) {
    for (size_t j = 0; j < c.rows; ++j) {
      unwrap_double(matrix_get(&c, i, j)) = rng(BOUND_A, BOUND_B);
    }
  }

  matrix_print(&lambda);
  matrix_print(&c);

  matrix *inv = matrix_inverse(&c);
  matrix *res = matrix_on_matrix(inv, &lambda);
  matrix *fin = matrix_on_matrix(res, &c);

  matrix_print(fin);

  matrix_free(res);
  matrix_free(inv);
  matrix_free(fin);
  matrix_free(&lambda);
  matrix_free(&c);
  
  return 0;
}
