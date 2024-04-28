#include <stdio.h>
#include <math.h>
#include <jemalloc/jemalloc.h>

#include "../types/eigenpair.h"
#include "../types/vector.h"
#include "../types/matrix.h"
#include "../common.h"

eigenpair* power_method(matrix *a) {

  eigenpair *result = (eigenpair *)malloc(sizeof(eigenpair));
  vector cur_vec;
  vector eigen_next, eigen_prev;

  vector_init(&result->eigenvector, a->rows, sizeof(double));
	vector_init(&cur_vec, a->rows, sizeof(double));
  vector_init(&eigen_next, a->rows, sizeof(double));
	vector_init(&eigen_prev, a->rows, sizeof(double));
  vector_fill_smth(&result->eigenvector, 0.0);
  vector_fill_smth(&cur_vec, 1.0);
  vector_fill_smth(&eigen_next, 1.0);
	vector_fill_smth(&eigen_prev, 1.0);
	result->eigenvalue = 0.0;

  for (;;) {
    vector *tmp_v = matrix_on_vector(a, &cur_vec);
    vector_free(&result->eigenvector);
    vector_copy_from_heap(&result->eigenvector, tmp_v);

		for (size_t i = 0; i < a->rows; ++i) {
			double tmp = (unwrap_double(vector_get(&result->eigenvector, i)) /
										          unwrap_double(vector_get(&cur_vec, i)));
			if (tmp < delta_c) tmp = 0.0;
			vector_change(&eigen_next, i, (void *)&tmp);
		}

		size_t flag = 1;
		for (size_t i = 0; i < a->rows; ++i) {
			if (fabs(unwrap_double(vector_get(&eigen_next, i)) -
               unwrap_double(vector_get(&eigen_prev, i))) > rtol) flag = 0;
		}

		if (flag == 1) break;

    vector_swap_eff(&eigen_prev, &eigen_next);
    vector_swap_eff(&cur_vec, &result->eigenvector);
    vector_normalize(&cur_vec);
	}


	for (size_t i = 0; i < a->rows; ++i) {
		result->eigenvalue += unwrap_double(vector_get(&eigen_next, i));
	}
  result->eigenvalue = result->eigenvalue / (double) a->rows;	
  
  vector_free(&cur_vec);
  vector_free(&eigen_next);
  vector_free(&eigen_prev);

  return result;
}
