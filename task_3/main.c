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
#include <sqlite3.h>

#include "vector/vector.h"
#include "matrix/matrix.h"
#include "lup/lup.h"

#define MAX_ITER (~(size_t)0)
#define STOP_SEQ 5000
#define TOO_SMALL 1e-9

typedef struct segment {
    double start;
    double end;
} segment_t;


double func(double x) {
    static const double pi = 3.14159265358979323846;
    return (tan(pi/2 - x) - x);
}

double func_d(double x) {
    return (-1.0/(sin(x) * sin(x)) - 1);
}

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


void secant_method(segment_t *a, double epsilon) {
    while (fabs(a->end - a->start) > epsilon) {
        a->start = 
            a->start - (a->end - a->start)/(func(a->end) - func(a->start)) 
                                                    * func(a->start);

        a->end =
            a->end - (a->start - a->end) / (func(a->start) - func(a->end))
                                                    * func(a->end);
    }
}

int newton_method(segment_t *a, double epsilon) {
    segment_t b;
    for (size_t i = 0; i < MAX_ITER; ++i) {
        if (func(a->start) * func(a->end) > 0) {
            secant_method(a, epsilon);
        }

        printf("%lf %lf\n", a->start, a->end);

        b = *a;
        
        if (func(a->start) < 0) {
            a->start = a->start - (func(a->start)/func_d(a->start));
        } else {
            a->end   = a->end   - (func(a->end)/func_d(a->end));
        }

        if ( fabs(b.start - a->start) < epsilon && fabs(b.end - a->end) < epsilon ) {
            return 0;
        }
    }
    return -1;
}


double func_1_x(double x) {
    return sin(x + 1);
}

double func_1_y(void) {
    return 2.0;
}

double func_2_x(void) {
    return 1.0;
}

double func_2_y(double y) {
    return cos(y);
}

double func_1(double x, double y) {
    return (2*y - cos(x + 1));
}

double func_2(double x, double y) {
    return (x + sin(y) + 0.4);
}


int newton_method_sys(double *x_0, double *y_0, double epsilon) {
    matrix a;
    vector x_k, x_k_next;
    vector b;
    vector sol;

    matrix_init(&a, 2, sizeof(double));
    matrix_fill_rand(&a);

    vector_init(&x_k, 2, sizeof(double));
    
    vector_push(&x_k, (void *)x_0);
    vector_push(&x_k, (void *)y_0);

    vector_init_copy(&x_k_next, &x_k);
    vector_init_copy(&b, &x_k);
    vector_init_copy(&sol, &x_k);

    double tmp;

    while (1) {

        tmp = *(double *)vector_get(&x_k_next, 0);
        vector_change(&x_k, 0, (void *)&tmp);
        tmp = *(double *)vector_get(&x_k_next, 1);
        vector_change(&x_k, 1, (void *)&tmp);

        tmp = func_1_x(*(double *)vector_get(&x_k, 0));
        matrix_change(&a, 0, 0, &tmp);
        tmp = func_1_y();
        matrix_change(&a, 0, 1, &tmp);
        tmp = func_2_x();
        matrix_change(&a, 1, 0, &tmp);
        tmp = func_2_y(*(double *)vector_get(&x_k, 0));
        matrix_change(&a, 1, 1, &tmp);

        tmp = -func_1(*(double *)vector_get(&x_k, 0), 
                        *(double *)vector_get(&x_k, 1));
        vector_change(&b, 0, (void *)&tmp);
        tmp = -func_2(*(double *)vector_get(&x_k, 0), 
                        *(double *)vector_get(&x_k, 1));
        vector_change(&b, 1, (void *)&tmp);

        lup(&a, &b, &sol);
        
        tmp = *(double *)vector_get(&x_k, 0) +
                    *(double *)vector_get(&sol, 0);
        vector_change(&x_k_next, 0, (void *)&tmp);
        
        tmp = *(double *)vector_get(&x_k, 1) +
                    *(double *)vector_get(&sol, 1);
        vector_change(&x_k_next, 1, (void *)&tmp);

        if (vector_diff(&x_k_next, &x_k) < epsilon) break;
    }    

    *x_0 = *(double *)vector_get(&x_k_next, 0);
    *y_0 = *(double *)vector_get(&x_k_next, 1);

    matrix_free(&a);
    vector_free(&x_k);
    vector_free(&x_k_next);
    vector_free(&b);
    vector_free(&sol);

    return 0;
}

int main(int argc, char **argv) {
    /*
    segment_t b;
    */
    double epsilon, x_0, y_0;
    /*
    int k, g, n = 16;
    */

    if ( argc != 2 ) {
        fprintf(stderr, "Error: Usage: %s database\n", argv[0]);
        return 1;
    }
    /*    
    printf("Please provide me with an interval:\n");
    scanf("%lf%lf", &b.start, &b.end);

    printf("Please provide me with an epsilon value:\n");
    scanf("%lf", &epsilon);


    k = sequential_search(&b, n); 
    while ( k == 1 ) {
        if ( n >= STOP_SEQ ) {
            fprintf(stderr, "Error: The provided range does"
                            " not contain root\n");
            return 2;
        }
        n *= 2;
        k = sequential_search(&b, n);
    }


    g = newton_method(&b, epsilon);
    if ( g == 0 ) {
        printf("%lf %lf\n", b.start, b.end);
    } else if ( g == -1 ) {
        fprintf(stderr, "Error: Runtime fault contact developer\n");
        return 3;
    }
    */

    printf("Give me x_0, y_0, epsilon\n");
    scanf("%lf%lf%lf", &x_0, &y_0, &epsilon);

    newton_method_sys(&x_0, &y_0, epsilon);

    printf("%lf %lf\n", x_0, y_0);

    return 0;
}
