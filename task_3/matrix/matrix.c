/*
Copyright 2023 Pavel Shago

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdio.h>
#include <string.h>
#include <jemalloc/jemalloc.h>

#include "../vector/vector.h"
#include "matrix.h"


void matrix_init(matrix *m, size_t num_rows, size_t num_cols, size_t type_size) {
    m->rows = (vector *)malloc(num_rows * sizeof(vector));
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    m->type_size = type_size;

    for (size_t i = 0; i < num_rows; ++i) {
        vector_init(&m->rows[i], num_cols, type_size);
    }
}

void matrix_init_copy(matrix *dest, matrix *src) {
    dest->rows = (vector *)malloc(src->num_rows * sizeof(vector));
    dest->num_rows = src->num_rows;
    dest->num_cols = src->num_cols;
    dest->type_size = src->type_size;

    for (size_t i = 0; i < src->num_rows; ++i) {
        vector_init_copy(&dest->rows[i], &src->rows[i]);
    }
}

void matrix_set(matrix *m, size_t row, size_t col, void *data) {
    if ( row < m->num_rows && col < m->num_cols ) {
        vector_set_grow(&m->rows[row], col, data);
    }
}



void* matrix_get(matrix *m, size_t row, size_t col) {
    if ( row < m->num_rows && col < m->num_cols ) {
        return vector_get(&m->rows[row], col);
    }
    return NULL;
}

void matrix_print(matrix *m) {
    for (size_t i = 0; i < m->num_rows; ++i) {
        for (size_t j = 0; j < m->num_cols; ++j) {
            printf("%lf ", *(double *)matrix_get(m, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

void matrix_free(matrix *m) {
    for (size_t i = 0; i < m->num_rows; ++i) {
        vector_free(&m->rows[i]);
    }
    free(m->rows);
    m->num_rows = 0;
    m->num_cols = 0;
    m->type_size = 0;
}
