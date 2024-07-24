#include <jemalloc/jemalloc.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"
#include "matrix.h"
#include "vector.h"

void matrix_init(matrix *m, size_t rows, size_t type_size) {
  m->data = (vector *)malloc(sizeof(vector));
  m->rows = rows;
  m->type_size = type_size;

  vector_init(m->data, m->rows * m->rows, type_size);
}

void matrix_init_copy(matrix *dest, matrix *src) {
  dest->data = (vector *)malloc(sizeof(vector));
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

void matrix_push(matrix *m, void *data) { vector_push(m->data, data); }

void matrix_change(matrix *m, size_t row, size_t col, void *data) {
  if (row < m->rows && col < m->rows) {
    vector_change(m->data, (row * m->rows) + col, data);
  }
}

void matrix_swap(matrix *m, size_t i, size_t j, size_t k, size_t l) {
  if (i < m->rows && j < m->rows && k < m->rows && l < m->rows) {
    vector_swap(m->data, (i * m->rows + j), (k * m->rows + l));
  }
}

void *matrix_get(matrix *m, size_t row, size_t col) {
  if (row < m->rows && col < m->rows) {
    return vector_get(m->data, (row * m->rows) + col);
  }
  return NULL;
}

void matrix_print(matrix *m) {
  for (size_t i = 0; i < m->rows; ++i) {
    for (size_t j = 0; j < m->rows; ++j) {
      printf("%lf ", *(double *)matrix_get(m, i, j));
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
