#include <stdio.h>
#include <math.h>

#include <jemalloc/jemalloc.h>

#include "polynoms.h"
#include "../types/vector.h"
#include "../types/matrix.h"
#include "../common.h"


vector* orth_init(size_t n, matrix *E) {
  vector *orth_poly = (vector *)malloc(sizeof(vector));
  vector *q_0 = (vector *)malloc(sizeof(vector));
  vector *q_1 = (vector *)malloc(sizeof(vector));
  double alpha = 1.0, beta = 0.0;
  
  vector_init(orth_poly, n, sizeof(vector));
  vector_init(q_0, 1, sizeof(double));
  vector_init(q_1, 2, sizeof(double));

  vector_push(q_0, &alpha); 
  
  for (size_t i = 0; i < E->rows; ++i) {
    beta += matrix_val(E, i, 1);
  }
  beta /= -(double)E->rows;
  vector_push(q_1, &beta);
  vector_push(q_1, &alpha);
  
  vector_push(orth_poly, q_0);
  vector_push(orth_poly, q_1);

  free(q_0);
  free(q_1);

  return orth_poly;
}

vector* orth_step(size_t n, matrix *E, vector *f, vector *orth_poly) {
  static vector x_vec;
  double alpha = 1.0, beta = 0.0;
  if (x_vec.size == 0) {
    vector_init(&x_vec, 2, sizeof(double));
    vector_push(&x_vec, &beta);
    vector_push(&x_vec, &alpha);
  }

  for (size_t i = orth_poly->size; orth_poly->size < n; ++i) {
    vector q_i, q_i_mo;
    double sum_uppr = 0.0, sum_bott= 0.0, sum_uppr_b = 0.0, sum_bott_b = 0.0;
    
    vector_init_copy(&q_i, vector_get(orth_poly, i - 1));
    vector_init_copy(&q_i_mo, vector_get(orth_poly, i - 2));
    vector *xq_i = poly_mult(&x_vec, vector_get(orth_poly, i - 1));
    
    for (size_t j = 0; j < E->rows; ++j) {
      sum_uppr += matrix_val(E, j, 1) * pow(poly_val(vector_get(orth_poly, i - 1), matrix_val(E, j, 1)), 2.0);
      sum_bott += pow(poly_val(vector_get(orth_poly, i - 1), matrix_val(E, j, 1)), 2.0);
      sum_uppr_b += matrix_val(E, j, 1) * poly_val(vector_get(orth_poly, i - 1), matrix_val(E, j, 1)) *
        poly_val(vector_get(orth_poly, i - 2), matrix_val(E, j, 1));
      sum_bott_b += pow(poly_val(vector_get(orth_poly, i - 2), matrix_val(E, j, 1)), 2.0);
    }
    alpha = -sum_uppr/sum_bott;
    beta = -sum_uppr_b/sum_bott_b;
    vector_mult(&q_i, alpha);
    vector_mult(&q_i_mo, beta);
    
    poly_sum(xq_i, &q_i);
    poly_sum(xq_i, &q_i_mo);
    vector_push(orth_poly, xq_i);
    
    vector_free(&q_i);
    vector_free(&q_i_mo);
    free(xq_i);
  }
  
  for (size_t i = 0; i < orth_poly->size; ++i) {
    double sum_uppr = 0.0, sum_bott = 0.0;
    for (size_t j = 0; j < E->rows; ++j) {
      sum_uppr += poly_val(vector_get(orth_poly, i), matrix_val(E, j, 1)) * vector_val(f, j);
      sum_bott += pow(poly_val(vector_get(orth_poly, i), matrix_val(E, j, 1)), 2.0);
    }
    vector_mult(vector_get(orth_poly, i), sum_uppr/sum_bott);
  }


  vector *res = (vector *)malloc(sizeof(vector));
  vector_init_copy(res, vector_get(orth_poly, orth_poly->size - 1));

  for (size_t i = 0; i < orth_poly->size - 1; ++i) {
    poly_sum(res, vector_get(orth_poly, i));
  }

  vector_free(&x_vec);
  return res;
}
