#include <stdio.h>
#include <string.h>
#include <jemalloc/jemalloc.h>

#include "vector.h"
#include "matrix.h"
#include "../common.h"

void matrix_init(matrix *m, size_t rows, size_t type_size) {
  m->data = (vector *) malloc(sizeof(vector));
  m->rows = rows;
  m->type_size = type_size;

  vector_init(m->data, m->rows * m->rows, type_size);
}

void matrix_init_copy(matrix *dest, matrix *src) {
  dest->data = (vector *) malloc(sizeof(vector));
  dest->rows = src->rows;
  dest->type_size = src->type_size;
  
  vector_init_copy(dest->data, src->data);
}

void matrix_fill_zero(matrix *m) {
  for (size_t i = 0; i < m->rows; ++i) {
    for (size_t j = 0; j < m->rows; ++j) {
      double tmp = 0.0;
      matrix_push(m, (void *)&tmp);
    }
  }
}

void matrix_push(matrix *m, void *data) {
  vector_push(m->data, data);
}

void matrix_change(matrix *m, size_t row, size_t col, void *data) {
  if ( row < m->rows && col < m->rows ) {
    vector_change(m->data, (row * m->rows) + col, data);
  }
}

void matrix_swap(matrix *m, size_t i, size_t j, size_t k, size_t l) {
  if ( i < m->rows && j < m->rows && k < m->rows && l < m->rows ) {
    vector_swap(m->data, (i * m->rows + j), (k * m->rows + l));
  }
}

void* matrix_get(matrix *m, size_t row, size_t col) {
  if ( row < m->rows && col < m->rows ) {
    return vector_get(m->data, (row * m->rows) + col);
  }
  return NULL;
}

void matrix_print(matrix *m) {
  for (size_t i = 0; i < m->rows; ++i) {
    for (size_t j = 0; j < m->rows; ++j) {
      printf("%lf ", unwrap_double(matrix_get(m, i, j)));
    }
    printf("\n");
  }
  printf("\n");
}

void matrix_free(matrix *m) {
	vector_free(m->data);
	free(m->data);
	m->rows = 0;
	m->type_size = 0;
}

vector matrix_on_vector(matrix *a, vector *v) {
  vector result;
  vector_init_copy(&result, v);
  for (size_t i = 0; i < a->rows; ++i) {
    double sum = 0.0;
    for (size_t j = 0; j < a->rows; ++j) {
      sum += 
        unwrap_double(matrix_get(a, i, j)) * unwrap_double(vector_get(v, j));
    }
    unwrap_double(vector_get(&result, i)) = sum;
  }
  return result;
}

void matrix_dm(matrix *a, vector *v) {
  for (size_t i = 0; i < a->rows; ++i) {
    unwrap_double(matrix_get(a, i, i)) = 
      unwrap_double(matrix_get(a, i, i)) - unwrap_double(vector_get(v, i));
  }
}

void matrix_normalize_vect(matrix *a, vector *v) {
  matrix_on_vector(a, v);
  double norm_inf = vector_norm(v);
  for (size_t i = 0; i < v->size; ++i) {
    unwrap_double(vector_get(v, i)) = 
      unwrap_double(vector_get(v, i)) / norm_inf;
  }
}

matrix* matrix_on_matrix(matrix *a, matrix *b) {
  if (a->type_size != b->type_size || a->rows != b->rows) {
    return NULL;
  }
  matrix *result = (matrix *)malloc(sizeof(matrix));
  matrix_init(result, a->rows, a->type_size);
  matrix_fill_zero(result);
  for (size_t i = 0; i < a->rows; ++i) {
    for (size_t j = 0; j < a->rows; ++j) {
      double sum = 0.0;
      for (size_t k = 0; k < a->rows; ++k) {
        sum += unwrap_double(matrix_get(a, i, k)) * unwrap_double(matrix_get(b, k, j));
      }
      unwrap_double(matrix_get(result, i, j)) = sum;
    }
  }
  return result;
}

void matrix_row_swap(matrix *m, size_t i, size_t j) {
  if (i == j) return;
  for (size_t col = 0; col < m->rows; ++col) {
    void *temp = matrix_get(m, i, col);
    matrix_change(m, i, col, matrix_get(m, j, col));
    matrix_change(m, j, col, temp);
  }
}

void matrix_row_normalize(matrix *m, size_t row, double coef) {
  for (size_t col = 0; col < m->rows; ++col) {
    unwrap_double(matrix_get(m, row, col)) =
        unwrap_double(matrix_get(m, row, col)) / coef;
  }
}

void matrix_row_subtract(matrix *m, size_t from, size_t what, double coef) {
  for (size_t col = 0; col < m->rows; ++col) {
    unwrap_double(matrix_get(m, from, col)) -=
          unwrap_double(matrix_get(m, what, col)) * coef;
  }
}

matrix* matrix_inverse(matrix *m) {
  matrix *inv = (matrix *)malloc(sizeof(matrix));
  matrix_init(inv, m->rows, sizeof(double));

  for (size_t i = 0; i < m->rows; ++i) {
    for (size_t j = 0; j < m->rows; ++j) {
      double val = (i == j) ? 1.0 : 0.0;
      matrix_push(inv, &val);
    }
  }

  for (size_t col = 0; col < m->rows; ++col) {
    if (unwrap_double(matrix_get(m, col, col)) == 0) {
      for (size_t row = col + 1; row < m->rows; ++row) {
        if (unwrap_double(matrix_get(m, row, col)) != 0) {
          matrix_row_swap(m, col, row);
          matrix_row_swap(inv, col, row);
          break;
        }
      }
    }

    double diag_val = unwrap_double(matrix_get(m, col, col));
    matrix_row_normalize(m, col, diag_val);
    matrix_row_normalize(inv, col, diag_val);

    for (size_t row = 0; row < m->rows; ++row) {
      if (row != col) {
        double coef = unwrap_double(matrix_get(m, row, col));
        matrix_row_subtract(m, row, col, coef);
        matrix_row_subtract(inv, row, col, coef);
      }
    }
  }
  
  return inv;
}
