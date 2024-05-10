#include <stdio.h>
#include <math.h>
#include <jemalloc/jemalloc.h>

#include "../types/eigenpair.h"
#include "../types/vector.h"
#include "../types/matrix.h"
#include "../common.h"
#include "rng.h"

eigenpair* power_method(matrix *a) {
  eigenpair *result = (eigenpair *)malloc(sizeof(eigenpair));
  vector cur_vec;
  vector eigen_next, eigen_prev;

  size_t flag;

  vector_init(&result->eigenvector, a->rows, sizeof(double));
	vector_init(&cur_vec, a->rows, sizeof(double));
  vector_init(&eigen_next, a->rows, sizeof(double));
	vector_init(&eigen_prev, a->rows, sizeof(double));
  
  vector_fill_smth(&result->eigenvector, INITIAL);
  vector_fill_smth(&cur_vec, INITIAL);
  vector_fill_smth(&eigen_next, INITIAL);
	vector_fill_smth(&eigen_prev, INITIAL);
	
  result->eigenvalue = 0.0;

  for (;;) {
    vector_swap_eff(&cur_vec, &result->eigenvector);
    vector_swap_eff(&eigen_prev, &eigen_next);
    
    vector *tmp_v = matrix_on_vector(a, &cur_vec);
    vector_free(&result->eigenvector);
    vector_from_heap_to_stack(&result->eigenvector, tmp_v);

		for (size_t i = 0; i < a->rows; ++i) {
			double tmp = (vector_val(&result->eigenvector, i) /
										          vector_val(&cur_vec, i));
			if (fabs(vector_val(&cur_vec, i)) < delta_c) tmp = 0.0;
			vector_change(&eigen_next, i, (void *)&tmp);
		}
    vector_normalize(&result->eigenvector);

		flag = 1;
		for (size_t i = 0; i < a->rows; ++i) {
			if (fabs(vector_val(&eigen_next, i) -
               vector_val(&eigen_prev, i)) > rtol) flag = 0;
		}
    if (flag == 1) break;
	}

	for (size_t i = 0; i < a->rows; ++i) {
		result->eigenvalue += vector_val(&eigen_next, i);
	}
  result->eigenvalue /= (double) a->rows;	
  
  vector_free(&cur_vec);
  vector_free(&eigen_next);
  vector_free(&eigen_prev);

  printf("[Log]  Result power iter\n");
  return result;
}
