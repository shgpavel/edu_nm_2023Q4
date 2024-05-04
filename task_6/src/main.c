#include <stdio.h>

#include <jemalloc/jemalloc.h>

#include "funcs/func.h"
#include "types/vector.h"
#include "types/matrix.h"
#include "methods/gauss.h"
#include "common.h"

int main(void) {
  const size_t m = 5;
  const size_t n = 4;

  matrix E;
  vector app_1;
  matrix_init(&E, m, n + 1, sizeof(double));
  vector_init(&app_1, m, sizeof(double));
  
  double add = (BOUND_B - BOUND_A) / ((double) m);
  double x = -1;

  for (size_t i = 0; i < m; ++i, x += add) {
    double tmp = func(x) - 2 * addition;
    vector_push(&app_1, (void *)&tmp);
    //app[i].b = func(x) + 2 * addition;
    //app[i].c = func(x) + 4 * addition;

    for (size_t j = 0; j < E.cols; ++j) {
      double tmp = pow(x, ((double)j));
      matrix_push(&E, (void *)&tmp);
    }
  }

  matrix_print(&E);
  vector_print(&app_1);
  
  //vector *res = gauss(&E, &app_1);
  //vector_init(&res, n + 1, sizeof(double));
  //vector_fill_smth(&res, 1.0);

  //vector_print(res);
 
  
  matrix_free(&E);
  vector_free(&app_1);
  return 0;
}
