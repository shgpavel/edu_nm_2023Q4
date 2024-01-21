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
