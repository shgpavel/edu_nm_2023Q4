#ifndef MATRIX_H_
#define MATRIX_H_

#include "../vector/vector.h"

typedef struct matrix_s {
    vector *rows;
    size_t num_rows;
    size_t num_cols;
    size_t type_size;
} matrix;

void  matrix_init(matrix *, size_t, size_t, size_t);
void  matrix_init_copy(matrix *dest, matrix *src);
void  matrix_set(matrix *, size_t, size_t, void *);
void  matrix_print(matrix *m);
void* matrix_get(matrix *, size_t, size_t);
void  matrix_free(matrix *);

#endif
