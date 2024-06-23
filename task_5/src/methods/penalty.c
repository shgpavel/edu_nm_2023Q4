#include <stdio.h>
#include <math.h>
#include <jemalloc/jemalloc.h>

#include "polynoms.h"
#include "../common.h"
#include "../types/pair.h"
#include "../types/vector.h"
#include "../types/matrix.h"
#include "gauss.h"


vector* div_diff(size_t n, vector *points) {
  vector *res = (vector *)malloc(sizeof(vector));
  vector_init(res, points->size, sizeof(double));
  vector_fill_smth(res, 0.0);

  for (size_t i = 0; i < n; ++i) {
    vector_val(res, i) = pair_get(points, i).b;
  }
  
  for (size_t j = 1; j < n; ++j) {
    for (size_t i = n - 1; i >= j; --i) {
      vector_val(res, i) =
        (vector_val(res, i) - vector_val(res, i - 1))
        / (pair_get(points, i).a - pair_get(points, i - j).a);
    }
  }
  return res;
}

vector* newton_poly_dd(vector *points) {
  vector *res = (vector *)malloc(sizeof(vector));
  vector_init(res, points->size, sizeof(double));

  vector xpolres;
  vector xminus;

  vector_init(&xpolres, 2, sizeof(double));
  vector_init(&xminus, 2, sizeof(double));

  vector_fill_smth(res, 0.0);
  vector_fill_smth(&xminus, 1.0);

  
  vector *div_d = div_diff(points->size, points);

  for (size_t k = 0; k < points->size; ++k) {
    for (size_t i = 0; i < k; ++i) {
      vector_val(&xminus, 0) = -(pair_get(points, i).a);
      if (i != 0) {
        vector xpolres_next = poly_mult(&xpolres, &xminus);
        vector_swap_eff(&xpolres, &xpolres_next);
        vector_free(&xpolres_next);
      } else {
        vector_assign(&xpolres, &xminus);
      }
    }
    vector_mult(&xpolres, vector_val(div_d, k));
    poly_sum(res, &xpolres);
  }
  
  vector_val(&xminus, 0) = pair_get(points, 0).b;
  vector_val(&xminus, 1) = 0;
  poly_sum(res, &xminus);
  
  vector_free(&xminus);
  vector_free(&xpolres);
  vector_free(div_d);
  free(div_d);

  return res;
}

vector* spline_2_0(vector *points, size_t index, vector *res) {
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
  vector_val(&b, 2) = pair_get(points, index + 1).b;
  vector_val(&b, 3) = pair_get(points, index + 2).b;

  matrix_val(&A, 0, 0) = pair_get(points, index).a * pair_get(points, index).a;
  matrix_val(&A, 0, 1) = pair_get(points, index).a;
  matrix_val(&A, 0, 2) = 1;
  
  matrix_val(&A, 1, 0) = pair_get(points, index + 1).a
                       * pair_get(points, index + 1).a;
  matrix_val(&A, 1, 1) = pair_get(points, index + 1).a;
  matrix_val(&A, 1, 2) = 1;
  
  matrix_val(&A, 2, 3) = pair_get(points, index + 1).a
                       * pair_get(points, index + 1).a;
  matrix_val(&A, 2, 4) = pair_get(points, index + 1).a;
  matrix_val(&A, 2, 5) = 1;

  matrix_val(&A, 3, 3) = pair_get(points, index + 2).a
                       * pair_get(points, index + 2).a;
  matrix_val(&A, 3, 4) = pair_get(points, index + 2).a;
  matrix_val(&A, 3, 5) = 1;

  matrix_val(&A, 4, 0) = 2 * pair_get(points, index).a;
  matrix_val(&A, 4, 1) = 1;
  
  matrix_val(&A, 5, 3) = 2 * pair_get(points, index + 2).a;
  matrix_val(&A, 5, 4) = 1;

  matrix_print(&A);
  
  vector *ais = gauss(&A, &b);

  vector_print(ais);
  
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

vector* lagr_slae(vector *points) {  
  matrix V;
  matrix_init(&V, points->size, points->size, sizeof(double));
  for (size_t i = 0; i < points->size; ++i) {
    for (size_t j = 0; j < V.cols; ++j) {
      double xinj = pow(pair_get(points, i).a, ((double)j));
      matrix_push(&V, &xinj);
    }
  }

  vector y;
  vector_init(&y, points->size, sizeof(double));
  for (size_t i = 0; i < points->size; ++i) {
    vector_push(&y, &pair_get(points, i).b);
  }
  vector *res = gauss(&V, &y);
  
  matrix_free(&V);
  vector_free(&y);
  return res;
}
