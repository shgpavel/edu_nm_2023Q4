#include <stdio.h>
#include <string.h>
#include <math.h>
#include <jemalloc/jemalloc.h>

#include "vector.h"
#include "matrix.h"
#include "../common.h"


void matrix_init(matrix *m, size_t rows, size_t cols, size_t type_size) {
  m->rows = rows;
  m->cols = cols;
  m->data = (vector *)malloc(sizeof(vector));
  vector_init(m->data, m->rows * m->cols, type_size);
}

void matrix_init_copy(matrix *dest, matrix *src) {
  if (src != NULL) {
  dest->rows = src->rows;
  dest->cols = src->cols;
  dest->data = (vector *)malloc(sizeof(vector));
  vector_init_copy(dest->data, src->data);
  }
}

void matrix_push(matrix *m, void *data) {
  vector_push(m->data, data);
}

void matrix_change(matrix *m, size_t row, size_t col, void *data) {
  if (row < m->rows && col < m->cols) {
    vector_change(m->data, (row * m->rows) + col, data);
  }
}

void matrix_swap(matrix *m, size_t i, size_t j, size_t k, size_t l) {
  if (i < m->rows && j < m->cols && k < m->rows && l < m->cols) {
    vector_swap(m->data, (i * m->rows + j), (k * m->rows + l));
  }
}

void matrix_row_swap(matrix *m, size_t i, size_t j) {
  if (i == j) return;
  for (size_t col = 0; col < m->cols; ++col) {
    matrix_swap(m, i, col, j, col);
  }
}

void* matrix_get(matrix *m, size_t row, size_t col) {
  if ((row < m->rows) && (col < m->cols)) {
    return vector_get(m->data, row * m->cols + col);
  }
  return NULL;
}

void matrix_free(matrix *m) {
	vector_free(m->data);
	free(m->data);
	m->rows = 0;
  m->cols = 0;
}

void matrix_movenfree(matrix *v, matrix *c) {
  swap_xor_st(&v->rows, &c->rows);
  swap_xor_st(&v->cols, &c->cols);
  vector_swap_eff(v->data, c->data);
  if (c->data != NULL) matrix_free(c);
  if (on_heap(c)) free(c);
}

void matrix_print(matrix *m) {
  for (size_t i = 0; i < m->rows; ++i) {
    for (size_t j = 0; j < m->cols; ++j) {
      printf("%lf ", matrix_val(m, i, j));
    }
    printf("\n");
  }
}

void matrix_fill_smth(matrix *m, double t) {
  vector_fill_smth(m->data, t);
}

double matrix_norm_inf(matrix *a) {
  double norm_inf = 0.0;
  for (size_t i = 0; i < a->rows; ++i) {
    double row_sum = 0.0;
    for (size_t j = 0; j < a->cols; ++j) {
      row_sum += fabs(matrix_val(a, i, j));
    }
    norm_inf = row_sum > norm_inf ? row_sum : norm_inf;
  }
  return norm_inf;
}

matrix* matrix_transpose(matrix *a) {
  matrix *res = (matrix *)malloc(sizeof(matrix));
  matrix_init(res, a->cols, a->rows, a->data->type_size);
  for (size_t i = 0; i < res->rows; ++i) {
    for (size_t j = 0; j < res->cols; ++j) {
      matrix_push(res, matrix_get(a, j, i));
    }
  }
  return res;
}

vector* matrix_on_vector(matrix *a, vector *v) {
  if (v->size != a->cols) return NULL;

  vector *res = (vector *)malloc(sizeof(vector));
  vector_init(res, a->rows, sizeof(double));
  vector_fill_smth(res, 0.0);

  for (size_t i = 0; i < a->rows; ++i) {
    double sum = 0.0;
    for (size_t j = 0; j < a->cols; ++j) {
      sum += (matrix_val(a, i, j) * vector_val(v, j));
    }
    vector_val(res, i) = sum;
  }
  return res;
}

void matrix_row_normalize(matrix *m, size_t row, double coef) {
  for (size_t col = 0; col < m->cols; ++col) {
    matrix_val(m, row, col) = matrix_val(m, row, col) / coef;
  }
}

void matrix_row_subtract(matrix *m, size_t from, size_t what, double coef) {
  for (size_t col = 0; col < m->rows; ++col) {
    matrix_val(m, from, col) -= matrix_val(m, what, col) * coef;
  }
}

void matrix_normalize_vect(matrix *a, vector *v) {
  matrix_on_vector(a, v);
  double norm_inf = sqrt(vector_sclr_prod(v, v));
  for (size_t i = 0; i < v->size; ++i) {
    vector_val(v, i) = vector_val(v, i) / norm_inf;
  }
}

matrix* matrix_on_matrix(matrix *a, matrix *b) {
  if (a->data->type_size != b->data->type_size || a->cols != b->rows) {
    return NULL;
  }
  matrix *res = (matrix *)malloc(sizeof(matrix));
  matrix_init(res, a->rows, b->cols, a->data->type_size);
  matrix_fill_smth(res, 0.0);
  for (size_t i = 0; i < a->rows; ++i) {
    for (size_t j = 0; j < b->cols; ++j) {
      double sum = 0.0;
      for (size_t k = 0; k < a->cols; ++k) {
        sum += matrix_val(a, i, k) * matrix_val(b, k, j);
      }
      matrix_val(res, i, j) = sum;
    }
  }
  return res;
}

matrix* matrix_inverse(matrix *m) {
  if (m->cols != m->rows)  return NULL;
  matrix *inv = (matrix *)malloc(sizeof(matrix));
  matrix_init(inv, m->rows, m->cols, sizeof(double));

  for (size_t i = 0; i < m->rows; ++i) {
    for (size_t j = 0; j < m->cols; ++j) {
      double val = (i == j) ? 1.0 : 0.0;
      matrix_push(inv, &val);
    }
  }

  for (size_t col = 0; col < m->rows; ++col) {
    if (matrix_val(m, col, col) == 0) {
      for (size_t row = col + 1; row < m->cols; ++row) {
        if (matrix_val(m, row, col) != 0) {
          matrix_row_swap(m, col, row);
          matrix_row_swap(inv, col, row);
          break;
        }
      }
    }

    double diag_val = matrix_val(m, col, col);
    matrix_row_normalize(m, col, diag_val);
    matrix_row_normalize(inv, col, diag_val);

    for (size_t row = 0; row < m->rows; ++row) {
      if (row != col) {
        double coef = matrix_val(m, row, col);
        matrix_row_subtract(m, row, col, coef);
        matrix_row_subtract(inv, row, col, coef);
      }
    }
  }
  
  return inv;
}
