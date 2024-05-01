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
      vector_val(solution, i) -= matrix_val(a, i, j) * vector_val(solution, j);
    }
    vector_val(solution, i) /= matrix_val(a, i, i);
  }
}

void rotation(matrix *a, size_t i, size_t j, double c, double s) {
  for (size_t k = 0; k < a->rows; ++k) {
    double tmp = c * matrix_val(a, k, i) - s * matrix_val(a, k, j);
    matrix_val(a, k, j) = s * matrix_val(a, k, i) + c * matrix_val(a, k, j);
    matrix_val(a, k, i) = tmp;
  }
}


void go_hessenberg(matrix *a) {
  matrix h;
  vector v;
  matrix_init(&h, a->rows, sizeof(double));
  vector_init(&v, a->rows, sizeof(double));
  
  matrix_fill_zero(&h);
  vector_fill_smth(&v, 0.0);
    
  for (size_t i = 0; i < a->rows - 2; ++i) {
    double s_n = matrix_val(a, i + 1, i) >= 0 ? 1 : -1;
    double sum = 0.0, mi_n;

    for (size_t j = i + 1; j < a->rows; ++j) {
      sum += matrix_val(a, j, i) * matrix_val(a, j, i);
    }
    s_n = s_n * sqrt(sum);
    mi_n = 1 / sqrt(2 * s_n * (s_n - matrix_val(a, i + 1, i)));

    for (size_t j = i; j < a->rows; ++j) {
      double tmp = 0.0;
      if (j == i + 1) tmp = matrix_val(a, j, i) - s_n;
      else if (j != i) tmp  = matrix_val(a, j, i);
      vector_change(&v, j, &tmp);
    }
    vector_mult(&v, mi_n);
    
    for (size_t j = i; j < a->rows; ++j) {
      for (size_t k = i; k < a->rows; ++k) {
        matrix_val(&h, j, k) = (j == k) ? 1 : 0;
        matrix_val(&h, j, k) -= 2 * vector_val(&v, j) * vector_val(&v, k);
      }
    }
    
    matrix *tmp = matrix_on_matrix(&h, a);
    matrix *res = matrix_on_matrix(tmp, &h);
    matrix_free(tmp);
    free(tmp);
    matrix_copy_not_emp(a, res);
    matrix_print(&h);
    matrix_print(a);
  }
  matrix_free(&h);
  vector_free(&v);
}


/*
vector *qr(matrix *a, vector *b) {
  matrix r, q;
  matrix_init(&r, a->rows, sizeof(double));
  matrix_init(&q, a->rows, sizeof(double));

  matrix_

}
*/

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
        matrix_val(&a, k, j) -= fcr * matrix_val(&a, i, j);
      }
      vector_val(&b, k) -= fcr * vector_val(&b, i);
    }
  }

  finalize(&a, &b, solution);

  matrix_free(&a);
  vector_free(&b);

  return solution;
}
