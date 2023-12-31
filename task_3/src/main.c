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

#include "func/func.h"
#include "vector/vector.h"
#include "matrix/matrix.h"
#include "lup/lup.h"

#define STOP_SEQ 10000


typedef struct segment {
    double start;
    double end;
} segment_t;


int sequential_search(segment_t *a, size_t n) {
    double h = (a->end - a->start)/n;

    for (size_t i = 1; i <= n; ++i) {
        if ( (func(a->start + ((i - 1) * h)) * func(a->start + i * h)) < 0 ) {
            a->start = a->start + ((i - 1) * h);
            a->end = a->start + i * h;
            return 0;
        }
    }
    return 1;
}

void secant_method(segment_t *a, double *x) {
    *x = a->start - 
        ((a->end - a->start) * func(a->start))/(func(a->end) - func(a->start));
}

double newton_method(segment_t *a, double x_0, double epsilon) {
    double x_1 = x_0;

    while (1) {
        x_0 = x_1;

        if ((x_0 > a->end) || (x_0 < a->start)) {
            secant_method(a, &x_1);
        } else {
            x_1 = x_0 - func(x_0)/func_d(x_0);
        }

        if ((x_1 >= a->start) && (x_1 <= a->end)) { 
            if (func(x_1) < 0) {
                a->start = x_1 - func(x_1)/func_d(x_1);
            } else if(func(x_1) > 0) {
                a->end = x_1 - func(x_1)/func_d(x_1);
            }
        }

        if (fabs(x_1 - x_0) < epsilon) {
            return x_1;
        }
    }
}

/*
    Next block is very straightforward solution.
    Something like control function or flags wrap is needed
    to prevent lots of memory alloc/free when using automated init approx.
*/

void newton_method_sys(double *x_0, double *y_0, double epsilon, double lambda) {
    matrix a;
    vector x_k, x_k_next;
    vector b;
    vector sol;

    matrix_init(&a, 2, sizeof(double));
    matrix_fill_zero(&a);

    vector_init(&x_k, 2, sizeof(double));
    vector_init(&b, 2, sizeof(double));
    vector_init(&sol, 2, sizeof(double));
    
    vector_push(&x_k, (void *)x_0);
    vector_push(&x_k, (void *)y_0);

    vector_init_copy(&x_k_next, &x_k);
    
    vector_fill_zero(&b);
    vector_fill_zero(&sol);

    double tmp;
    static int flag = 0;

    if (lambda == 1.0) {    flag = 0; }

    while (1) {

        tmp = *(double *)vector_get(&x_k_next, 0);
        vector_change(&x_k, 0, (void *)&tmp);
        tmp = *(double *)vector_get(&x_k_next, 1);
        vector_change(&x_k, 1, (void *)&tmp);

        tmp = func_1_x(*(double *)vector_get(&x_k, 0),
                        *(double *)vector_get(&x_k, 1), lambda);
        matrix_change(&a, 0, 0, &tmp);
        tmp = func_1_y(*(double *)vector_get(&x_k, 0),
                        *(double *)vector_get(&x_k, 1), lambda);
        matrix_change(&a, 0, 1, &tmp);
        tmp = func_2_x(*(double *)vector_get(&x_k, 0),
                        *(double *)vector_get(&x_k, 1), lambda);
        matrix_change(&a, 1, 0, &tmp);
        tmp = func_2_y(*(double *)vector_get(&x_k, 0),
                        *(double *)vector_get(&x_k, 1), lambda);
        matrix_change(&a, 1, 1, &tmp);

        tmp = func_1_neg(*(double *)vector_get(&x_k, 0), 
                        *(double *)vector_get(&x_k, 1), lambda);
        vector_change(&b, 0, (void *)&tmp);
        tmp = func_2_neg(*(double *)vector_get(&x_k, 0), 
                        *(double *)vector_get(&x_k, 1), lambda);
        vector_change(&b, 1, (void *)&tmp);

        lup(&a, &b, &sol);
        
        tmp = *(double *)vector_get(&x_k, 0) +
                    *(double *)vector_get(&sol, 0);
        vector_change(&x_k_next, 0, (void *)&tmp);
        
        tmp = *(double *)vector_get(&x_k, 1) +
                    *(double *)vector_get(&sol, 1);
        vector_change(&x_k_next, 1, (void *)&tmp);

        if (lambda != 1.0 || flag == 1) {
            if (flag == 0) { flag = 1; }
            break;
        }
        if (vector_diff(&x_k_next, &x_k) < epsilon) break;
    }

    *x_0 = *(double *)vector_get(&x_k_next, 0);
    *y_0 = *(double *)vector_get(&x_k_next, 1);

    matrix_free(&a);
    vector_free(&x_k);
    vector_free(&x_k_next);
    vector_free(&b);
    vector_free(&sol);
}

void init_approx(double epsilon) {
    double base_x = -0.4, base_y = 0;
    double n = 10;

    for (double i = 1; i <= n; ++i) {
        newton_method_sys(&base_x, &base_y, 1.0, i/n);
    }

    newton_method_sys(&base_x, &base_y, epsilon, 1.0);
    printf("initial approx nm %lf %lf\n", base_x, base_y);
}

int main(void) {
    segment_t a;
    double epsilon, x_0, y_0;
    int k, n = 16;

    printf("interval?\n");
    scanf("%lf%lf", &a.start, &a.end);

    printf("x_0?\n");
    scanf("%lf", &x_0);

    printf("epsilon value?\n");
    scanf("%lf", &epsilon);

    k = sequential_search(&a, n); 
    while (k == 1) {
        if (n >= STOP_SEQ) {
            fprintf(stderr,
                "Error: The provided range does not contain root\n");
            return 2;
        }
        n *= 2;
        k = sequential_search(&a, n);
    }


    x_0 = newton_method(&a, x_0, epsilon);
    printf("%lf %lf %lf\n", a.start, a.end, x_0);


    printf("(x_0, y_0, epsilon)?\n");
    scanf("%lf%lf%lf", &x_0, &y_0, &epsilon);

    newton_method_sys(&x_0, &y_0, epsilon, 1.0);
    printf("graphical init nm %lf %lf\n", x_0, y_0);

    init_approx(epsilon);

    return 0;
}
