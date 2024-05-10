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
    matrix_copy(a, res);
  }
  matrix_free(&h);
  vector_free(&v);
}

/*
inline void rotation(matrix *a, size_t i, double c, double s) {
  for (size_t k = i; k < a->rows; ++k) {
    double tmp = -c * matrix_val(a, i, k) - s * matrix_val(a, i + 1, k);
    matrix_val(a, i + 1, k) = s * matrix_val(a, i, k) + c * matrix_val(a, i + 1, k);
    matrix_val(a, i, k) = tmp;
  }
}*/

matrix* qr_decomp(matrix *m) {
  matrix *q = (matrix *)malloc(sizeof(matrix));
  matrix_init(q, 3, 3, sizeof(double));

  for (size_t i = 0; i < q->rows; ++i) {
    for (size_t j = 0; j < q->cols; ++j) {
      double tmp = (i == j) ? 1.0 : 0.0;
      matrix_push(q, &tmp);
    }
  }

  double ag[3][3] = {{5, -3, -1}, {-5, 2.08, 0.56}, {0, -0.44, -1.08}};
  matrix *a = (matrix *)malloc(sizeof(matrix));
  matrix_init(a, 3, 3, sizeof(double));
  for (size_t i = 0; i < 3; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      matrix_push(a, &ag[i][j]);
    }
  }

  matrix_print(a);
  printf("\n");


  
  for (size_t i = 0; i < a->rows - 1; ++i) {
    //double t = matrix_val(a, i, i) / matrix_val(a, i + 1, i);
    //double c = 1 / sqrt(1 + t * t);
    //double s = t * c;
 
    double t = hypot(matrix_val(a, i, i), matrix_val(a, i + 1, i));
    double c = matrix_val(a, i, i) / t;
    double s = matrix_val(a, i + 1, i) / t;

    for (size_t k = i; k < a->rows; ++k) {

      double tmp = -c * matrix_val(a, i, k) - s * matrix_val(a, i + 1, k);
      matrix_val(a, i + 1, k) = s * matrix_val(a, i, k) - c * matrix_val(a, i + 1, k);
      matrix_val(a, i, k) = tmp;
    
      tmp = c * matrix_val(q, k, i) + s * matrix_val(q, k, i + 1);
      matrix_val(q, k, i + 1) = -s * matrix_val(q, k, i) + c * matrix_val(q, k, i + 1);
      matrix_val(q, k, i) = tmp;
    }
 

  }


  matrix_print(a);
  printf("\n");
  matrix_print(q);

  /*
  for (size_t i = 0; i < a->rows - 1; ++i) {
    double norm = hypot(matrix_val(a, i, i), matrix_val(a, i + 1, i));
    double c = matrix_val(a, i, i)/norm;
    double s = -matrix_val(a, i + 1, i)/norm;
    rotation(a, i, c, s);
    rotation(q, i, c, s);
  }
  
  for (size_t i = 0; i < a->rows - 1; ++i) {
    double norm = hypot(matrix_val(a, i, i), matrix_val(a, i + 1, i));
    double c = matrix_val(a, i, i)/norm;
    double s = -matrix_val(a, i + 1, i)/norm;
    rotation(q, i, c, s);
  }
  */
  return q;
}

vector *qr(matrix *a) {
  vector *result = (vector *)malloc(sizeof(vector));
  vector_init(result, a->rows, sizeof(double));

  matrix *q = qr_decomp(a);

  //matrix_print(a);
  //printf("\n");
  //matrix_print(q);
  //printf("\n");  
  /*
  for (size_t i = 0; a->rows >= 2 && i < MAX_ITER; ++i) {
    double ann = matrix_val(a, a->rows - 1, a->rows - 1);
    matrix *tmp = matrix_on_matrix(a, q);
    //matrix_print(tmp);
    
    for (size_t j = 0; j < tmp->rows; ++j) {
      matrix_val(tmp, j, j) += ann;
    }
    matrix_copy(a, tmp);


    if (fabs(matrix_val(a, a->rows - 1, a->rows - 2)) < epsilon_c ) {//&& 
        //(fabs(matrix_val(a, a->rows - 1, a->rows - 1) - ann) < 0.3 * ann)) {
      vector_push(result, matrix_get(a, a->rows - 1, a->rows - 1));
      for (size_t k = 0; k < a->rows; ++k) {
        matrix_delete(a, k, a->rows - 1);
        matrix_delete(q, k, q->rows - 1);
        if (k != a->rows - 1) {
          matrix_delete(a, a->rows - 1, k);
          matrix_delete(q, q->rows - 1, k);
        }
      }
      a->rows--;
      a->cols--;
      q->rows--;
      q->cols--;
      //matrix_free(q);
      //free(q);
      //q = qr_decomp(a);
    }
  }
  */
  matrix_free(q);
  return result;
}
