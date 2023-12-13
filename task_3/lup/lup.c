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

void lup(matrix *m, vector *c, vector *solution) {
    matrix a;
    matrix_init_copy(&a, m);
    vector b;
    vector_init_copy(&b, c);

    
    vector perm;
    vector_init(&perm, a.num_rows, sizeof(size_t));
    for (size_t i = 0; i < a.num_rows; ++i) {
        vector_set_grow(&perm, i, &i);
    }
    
    for (size_t i = 0; i < a.num_rows; ++i) {
        double lead = 0;
        size_t swap = i;
        for (size_t j = i + 1; j < a.num_cols; ++j) {
            double *a_j_i = (double *)matrix_get(&a, j, i);
            if (fabs(lead) < fabs(*a_j_i)) {
                lead = *a_j_i;
                swap = j;
            }
        }

        if (swap != i) {
            vector_swap(&perm, i, swap);
            for (size_t k = 0; k < a.num_cols; ++k) {
                double *a_i = (double *)matrix_get(&a, i, k);
                double *a_swap = (double *)matrix_get(&a, swap, k);
                matrix_set(&a, i, k, (void *)a_swap);
                matrix_set(&a, swap, k, (void *)a_i);
            }
        }
        
        for (size_t j = i + 1; j < a.num_rows; ++j) {
            double *a_j_i = (double *)matrix_get(&a, j, i);
            double cache = *a_j_i;
            *a_j_i = cache / lead;
            matrix_set(&a, j, i, (void *)a_j_i);
            for (size_t k = i + 1; k < a.num_cols; ++k) {
                double tmp;
                double *a_j_k = (double *)matrix_get(&a, j, k);
                double *a_i_k = (double *)matrix_get(&a, i, k);
                tmp = *a_j_k - (*a_j_i * *a_i_k);
                matrix_set(&a, j, k, (void *)&tmp);
            }
        }
    }
    
    for (size_t i = 0; i < b.size; ++i) {
        double *c_perm = (double *)vector_get(c, 
                         *(size_t *)vector_get(&perm, i));
        vector_set_grow(&b, i, (void *)c_perm);
    }


    for (size_t i = 0; i < a.num_rows; ++i) {
        for (size_t j = 0; j < i; ++j) {
            double tmp;
            double *b_i = (double *)vector_get(&b, i);
            double *b_j = (double *)vector_get(&b, j);
            double *a_i_j = (double *)matrix_get(&a, i, j);
            tmp = *b_i - (*a_i_j * *b_j);
            vector_set_grow(&b, i, (void *)&tmp);
        }
    }
    
    for (size_t i = a.num_rows; i > 0; ) {
        --i;
        double *b_i = (double *)vector_get(&b, i);
        vector_set_grow(solution, i, (void *)b_i);
        for (size_t j = a.num_rows; j > i + 1; ) {
            --j;
            double tmp;
            tmp = *(double *)vector_get(solution, i) - 
                    (*(double *)matrix_get(&a, i, j) * *(double *)vector_get(solution, j));
            vector_set_grow(solution, i, (void *)&tmp);
        }
        double tmp;
        tmp = *(double *)vector_get(solution, i) / *(double *)matrix_get(&a, i, i);
        vector_set_grow(solution, i, (void *)&tmp);
    }

    matrix_free(&a);
    vector_free(&b);
}
