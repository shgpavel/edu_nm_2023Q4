#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types/vector.h"
#include "types/matrix.h"
#include "types/eigenpair.h"
#include "methods/rng.h"
#include "methods/qr.h"
#include "methods/power_method.h"
#include "methods/inverse_iteration.h"
#include "common.h"

int main(void) {
  size_t n;
  scanf("%zu", &n);

  srand(time(NULL));
  
  matrix lambda, c, c_copy;
  matrix_init(&lambda, n, n, sizeof(double));
  matrix_init(&c, n, n, sizeof(double));
  matrix_fill_smth(&lambda, 0.0);
  matrix_fill_smth(&c, 0.0);

  for (size_t i = 0; i < lambda.rows; ++i) {
    matrix_val(&lambda, i, i) = rng(BOUND_A, BOUND_B);
  }

  for (size_t i = 0; i < c.rows; ++i) {
    for (size_t j = 0; j < c.rows; ++j) {
      matrix_val(&c, i, j) = rng(BOUND_A, BOUND_B);
    }
  }

  matrix_init_copy(&c_copy, &c);

  matrix_print(&lambda);
  printf("\n");
  
  matrix *inv = matrix_inverse(&c);
  matrix_free(&c);

  matrix *res = matrix_on_matrix(inv, &lambda);
  matrix_free(inv);
  matrix_free(&lambda);
  free(inv);

  matrix *fin = matrix_on_matrix(res, &c_copy);
  matrix_free(res);
  matrix_free(&c_copy);
  free(res);

  matrix_print(fin);
  printf("\n");
  
  eigenpair *pm_res = power_method(fin);
  if (pm_res != NULL) {
    vector_print(&pm_res->eigenvector);
    printf("%lg\n\n", pm_res->eigenvalue);
    vector_free(&pm_res->eigenvector);
    free(pm_res);
   } else {
    printf("[Error]  Power iter failed\n");
   }

 
  vector *it_res = inverse_iter(fin);
  for (size_t i = 0; it_res != NULL && i < it_res->size; ++i) {
    vector_print(&((eigenpair *)vector_get(it_res, i))->eigenvector);
    vector_free(&((eigenpair *)vector_get(it_res, i))->eigenvector);
    printf("%lg\n\n", ((eigenpair *)vector_get(it_res, i))->eigenvalue);
  }
 
  if (it_res != NULL) {
    vector_free(it_res);
    free(it_res);
  }
  

  go_hessenberg(fin);

  vector *res_qr = qr(fin);
  if (res_qr != NULL) {
    printf("\n[Log]  Result qr\n");
    vector_print(res_qr);
    vector_free(res_qr);
    free(res_qr);
  } else {
    printf("\n[Error]  Qr\n");
  }
  
  matrix_free(fin);
  free(fin);
  return 0;
}
