#include <stdio.h>

#include <jemalloc/jemalloc.h>

#include "funcs/func.h"
#include "types/vector.h"
#include "types/matrix.h"
#include "methods/gauss.h"
#include "draw/draw.h"
#include "common.h"


int main(void) {
  const size_t m = 51;
  const size_t n = 10;

  matrix E;
  vector app_1;
  matrix_init(&E, m, n + 1, sizeof(double));
  vector_init(&app_1, m, sizeof(double));
  
  double add = (BOUND_B - BOUND_A) / ((double) m);
  double x = -1;

  for (size_t i = 0; i < m; ++i, x += add) {
    double tmp = func(x) - 3 * addition;
    vector_push(&app_1, (void *)&tmp);
    //app[i].b = func(x) + 2 * addition;
    //app[i].c = func(x) + 4 * addition;

    for (size_t j = 0; j < E.cols; ++j) {
      double tmp = pow(x, ((double)j));
      matrix_push(&E, (void *)&tmp);
    }
  }
  
  matrix *E_tr = matrix_transpose(&E);
  
  matrix *left = matrix_on_matrix(E_tr, &E);
  vector *right = matrix_on_vector(E_tr, &app_1);

  
  vector_free(&app_1);
  
  matrix_free(E_tr);
  free(E_tr);

  vector *res = gauss(left, right);

  vector_free(right);
  matrix_free(left);
  free(left);
  free(right);

  
  char *or_func = "x^2 tan(x)";
  str_func(or_func);
  add_func(res);
  plot();
  
  vector_free(res);
  free(res);

  /*
  vector orth_poly, q_0, q_1;
  double alpha = 1.0, beta = 0.0;
  
  vector_init(&orth_poly, n, sizeof(vector));
  vector_init(&q_0, 1, sizeof(double));

  vector_push(&q_0, &alpha); 
  vector_push(&q_1, &alpha); 

  for (size_t i = 0; i < m; ++i) {
    beta += matrix_val(&E, i, 1);
  }
  beta /= (double)m;
  vector_push(&q_1, &beta);
  
  vector_push(&orth_poly, &q_0);
  */

  matrix_free(&E);
  
  return 0;
}
