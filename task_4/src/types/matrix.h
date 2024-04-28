#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix_s {
  size_t rows;
  size_t type_size;
  vector *data;
} matrix;

void     matrix_init(matrix *, size_t, size_t);
void     matrix_init_copy(matrix *, matrix *);
void     matrix_push(matrix *, void *);
void     matrix_change(matrix *, size_t, size_t, void *);
void     matrix_fill_zero(matrix *);
void     matrix_print(matrix *);
void     matrix_swap(matrix *, size_t, size_t, size_t, size_t);
void*    matrix_get(matrix *, size_t, size_t);
void     matrix_free(matrix *);
vector*  matrix_on_vector(matrix *, vector *);
void     matrix_normalize_vect(matrix *, vector *);
matrix*  matrix_on_matrix(matrix *, matrix *);
matrix*  matrix_inverse(matrix *);
double   matrix_norm_inf(matrix *);
void     matrix_copy_from_heap(matrix *, matrix *);

#endif
