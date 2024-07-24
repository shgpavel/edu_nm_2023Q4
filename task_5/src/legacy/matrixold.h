#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct matrix_s {
  size_t rows;
  size_t type_size;
  vector *data;
} matrix;

void matrix_init(matrix *, size_t, size_t);
void matrix_init_copy(matrix *dest, matrix *src);
void matrix_push(matrix *, void *);
void matrix_change(matrix *, size_t, size_t, void *);
void matrix_fill_zero(matrix *);
void matrix_print(matrix *);
void matrix_swap(matrix *, size_t, size_t, size_t, size_t);
void *matrix_get(matrix *, size_t, size_t);
void matrix_free(matrix *);

#endif
