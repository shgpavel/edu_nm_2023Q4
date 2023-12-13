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
} segm_t;


double func(double x) {
    static const double pi = 3.14159265358979323846;
    return (tan(pi/2 - x) - x);
}

double func_d(double x) {
    return (-1.0/(sin(x) * sin(x)) - 1);
}

int sequential_search(segm_t *a, size_t n) {
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

void secant_method(segm_t *a, double epsilon) {
    while (fabs(a->end - a->start) > epsilon) {
        a->start = 
            a->start - (a->end - a->start) * func(a->start) /
                        (func(a->end) - func(a->start));

        a->end = 
            a->end - (a->start - a->end) * func(a->end) /
                        (func(a->start) - func(a->end));
    }
}

int newton_method(double *x_0, double epsilon) {
    double y, y_d, x_1;
    for (size_t i = 0; i < MAX_ITER; ++i) {
        y = func(*x_0);
        y_d = func_d(*x_0);
        
        if (fabs(y_d) < TOO_SMALL) {
            return 1;
        }

        x_1 = *x_0 - (y/y_d);
        if ( fabs(x_1 - *x_0) < epsilon ) {
            *x_0 = x_1;
            return 0;
        }
        *x_0 = x_1;
    }
    return -1;
}

double func_1_x(double x, double y) {
    return sin(x + 1);
}

double func_1_y() {
    return 2.0;
}

double func_2_x() {
    return 1.0;
}

double func_2_y(double x, double y) {
    return cos(y);
}

double func_1(double x, double y) {
    return (2*y - cos(x + 1));
}

double func_2(double x, double y) {
    return (x + sin(y) + 0);
}

int newton_method_sys(double x_0, double y_0, double epsilon) {
    vector x_k, x_k_next;
    double h_n, g_n;
    do {
        


    } while (vector_diff(&x_k_next, &x_k) > epsilon);
}

int main(int argc, char **argv) {
    segm_t b;
    double epsilon, x_0, y_0;
    int k, g, n = 16;


    if ( argc != 2 ) {
        fprintf(stderr, "Error: Usage: %s database\n", argv[0]);
        return 1;
    }
    
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

    g = newton_method(&b.start, epsilon);
    if ( g == 0 ) {
        printf("%lf\n", b.start);
    } else if ( g == 1 ) {
        fprintf(stderr, "Error: The differential of f" 
                        " has approached near 0 values\n");
        return 3;
    } else if ( g == -1 ) {
        fprintf(stderr, "Error: Newton's method doesn't converge\n");
        return 3;
    }

    printf("x_0?, y_0?:\n");
    scanf("%lf%lf", &x_0, &y_0);

/*
    matrix a;
    matrix_init(&a, 3, 3, sizeof(double));
    vector c;
    vector_init(&c, 3, sizeof(double));
    vector sol;
    vector_init(&sol, 3, sizeof(double));


    printf("Give me matrix\n");
    for (size_t i = 0; i < a.num_rows; ++i) {
        for(size_t j = 0; j < a.num_cols; ++j) {
            double tmp;
            scanf("%lf", &tmp);
            matrix_set(&a, i, j, &tmp);
        }
    }
    
    printf("Give me b vector\n");
    for (size_t i = 0; i < c.capacity; ++i) {
        double tmp;
        scanf("%lf", &tmp);
        vector_set_grow(&c, i, &tmp);
    }

    lup(&a, &c, &sol);
    vector_print(&sol);
*/

    return 0;
}
