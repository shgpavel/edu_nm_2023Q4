#include <stdio.h>

#include "../types/eigenpair.h"
#include "../types/vector.h"
#include "../types/matrix.h"
#include "../common.h"

eigenpair* power_method(matrix *a) {
  
  eigenpair *result = (eigenpair *)malloc(sizeof(eigenpair));
  vector cur_vec;
  double cur_val;

  vector_init(&cur_vec, a->rows, sizeof(double));
  vector_fill_smth(&cur_vec, 1.0);
  vector_normalize(&cur_vec);
  
  do {
    vector_assign(&cur_vec, result.eigenvector);
    result.eigenvector = matrix_on_vector(a, &cur_vec);
    vector_normalize(result.eigenvector);
    
  } while ();
  
  return result;
}
