#include <stdio.h>
#include <math.h>
#include <jemalloc/jemalloc.h>

#include "../types/vector.h"
#include "../types/matrix.h"
#include "../common.h"

#define MAX_ITER 1200

void go_hessenberg(matrix *a) {
  matrix h;
  vector v;
  matrix_init(&h, a->rows, a->rows, sizeof(double));
  vector_init(&v, a->rows, sizeof(double));
  
  matrix_fill_smth(&h, 0.0);
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
    matrix_copy(a, res);
  }
  matrix_free(&h);
  vector_free(&v);
}

matrix* qr_decomp(matrix *a) {  
  matrix *q = (matrix *)malloc(sizeof(matrix));
  matrix_init(q, a->rows, a->cols, sizeof(double));

  matrix_fill_smth(q, 0.0);
  for (size_t i = 0; i < q->rows; ++i) {
    matrix_val(q, i, i) = 1.0;
  }


  for (size_t i = 0; i < a->rows - 1; ++i) {
    double t, s = 0, c = 1;
    t = matrix_val(a, i, i) / matrix_val(a, i + 1, i);
    c = 1 / sqrt(1 + t * t);
    s = t * c;
    
    matrix g;
    matrix_init(&g, a->rows, a->cols, sizeof(double));
    matrix_fill_smth(&g, 0.0);
    for (size_t k = 0; k < g.rows; ++k) {
      matrix_val(&g, k, k) = 1.0;
    }
    
    matrix_val(&g, i, i) = s;
    matrix_val(&g, i, i + 1) = c;
    matrix_val(&g, i + 1, i) = -c;
    matrix_val(&g, i + 1, i + 1) = s;

    matrix *g_trs = matrix_transpose(&g);
    
    matrix *a_next = matrix_on_matrix(&g, a);
    matrix_copy(a, a_next);
    
    matrix *q_next = matrix_on_matrix(q, g_trs);
    matrix_copy(q, q_next);

    matrix_free(&g);
    matrix_free(g_trs);
    free(g_trs);
  }
  
  return q;
}

vector *qr(matrix *a) {
  vector *result = (vector *)malloc(sizeof(vector));
  vector_init(result, a->rows, sizeof(double));

  matrix *q = qr_decomp(a);
  
  for (; a->rows >= 2; ) {
    double ann = matrix_val(a, a->rows - 1, a->rows - 1);
    matrix *r_next = matrix_on_matrix(a, q);
    
    for (size_t j = 0; j < r_next->rows; ++j) {
      matrix_val(r_next, j, j) += ann;
    }
    matrix_copy(a, r_next);


    if (fabs(matrix_val(a, a->rows - 1, a->rows - 2)) < 1e-3) { // && 
        //(fabs(matrix_val(a, a->rows - 1, a->rows - 1) - ann) < 0.3 * ann)) {
      vector_push(result, (void *)&ann);
      for (size_t k = 0; k < a->rows; ++k) {
        matrix_delete(a, k, a->rows - 1);
        if (k != a->rows - 1) {
          matrix_delete(a, a->rows - 1, k);
        }
      }
      a->rows--;
      a->cols--;
      matrix *q_next = qr_decomp(a);
      matrix_copy(q, q_next);
    }

    //matrix *q_next = qr_decomp(a);
    //matrix_copy(q, q_next);
  }

  matrix_free(q);
  return result;
}
