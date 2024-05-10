#include <jemalloc/jemalloc.h>

#include "../types/vector.h"
#include "../types/matrix.h"
#include "gauss.h"


vector* normal_equations(matrix *E, vector *f) {
  matrix *E_tr = matrix_transpose(E);
  
  matrix *left = matrix_on_matrix(E_tr, E);
  vector *right = matrix_on_vector(E_tr, f);

  matrix_free(E_tr);
  free(E_tr);

  vector *res = gauss(left, right);

  vector_free(right);
  matrix_free(left);
  free(left);
  free(right);

  return res;
}
