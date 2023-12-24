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
#include <math.h>

#include "../vector/vector.h"
#include "../matrix/matrix.h"

void lup(matrix *a, vector *b, vector *solution) {
    
    vector perm;
    vector c;

    vector_init_copy(&c, b);
    vector_init(&perm, a->rows, sizeof(size_t));
    for (size_t i = 0; i < a->rows; ++i) {
        vector_push(&perm, &i);
    }
    
    for (size_t i = 0; i < a->rows; ++i) {
        double lead = 0;
        size_t swap = i;
        for (size_t j = i + 1; j < a->rows; ++j) {
            double *a_j_i = (double *)matrix_get(a, j, i);
            if (fabs(lead) < fabs(*a_j_i)) {
                lead = *a_j_i;
                swap = j;
            }
        }

        if (swap != i) {
            vector_swap(&perm, i, swap);
            for (size_t k = 0; k < a->rows; ++k) {
                matrix_swap(a, i, k, swap, k);
            }
        }
        
        for (size_t j = i + 1; j < a->rows; ++j) {
            double *a_j_i = (double *)matrix_get(a, j, i);
            double cache = *a_j_i;
            *a_j_i = cache / lead;
            matrix_change(a, j, i, (void *)a_j_i);
            for (size_t k = i + 1; k < a->rows; ++k) {
                double tmp;
                double *a_j_k = (double *)matrix_get(a, j, k);
                double *a_i_k = (double *)matrix_get(a, i, k);
                tmp = *a_j_k - (*a_j_i * *a_i_k);
                matrix_change(a, j, k, (void *)&tmp);
            }
        }
    }

    for (size_t i = 0; i < b->size; ++i) {
        double *c_perm = (double *)vector_get(&c, 
                         *(size_t *)vector_get(&perm, i));
        vector_change(b, i, (void *)c_perm);
    }

    for (size_t i = 0; i < a->rows; ++i) {
        for (size_t j = 0; j < i; ++j) {
            double tmp;
            double *b_i = (double *)vector_get(b, i);
            double *b_j = (double *)vector_get(b, j);
            double *a_i_j = (double *)matrix_get(a, i, j);
            tmp = *b_i - (*a_i_j * *b_j);
            vector_change(b, i, (void *)&tmp);
        }
    }

    for (size_t i = a->rows; i > 0; ) {
        --i;
        double *b_i = (double *)vector_get(b, i);
        vector_change(solution, i, (void *)b_i);
        for (size_t j = a->rows; j > i + 1; ) {
            --j;
            double tmp;
            tmp = *(double *)vector_get(solution, i) - 
                    (*(double *)matrix_get(a, i, j) * *(double *)vector_get(solution, j));
            vector_change(solution, i, (void *)&tmp);
        }
        double tmp;
        tmp = *(double *)vector_get(solution, i) / *(double *)matrix_get(a, i, i);
        vector_change(solution, i, (void *)&tmp);
    }
   
    vector_free(&c);
    vector_free(&perm);
}
