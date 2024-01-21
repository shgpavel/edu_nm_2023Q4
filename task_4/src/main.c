#include <stdio.h>
#include <math.h>

#include "../../task_3/src/lup/lup.h"
#include "../../task_3/src/matrix/matrix.h"
#include "../../task_3/src/vector/vector.h"

double vector_norm(vector *v) {
    double result = 0.0;
    for (size_t i = 0; i < v->size; ++i) {
        if (result < fabs(*(double *)vector_get(v, i))) {
            result = fabs(*(double *)vector_get(v, i));
        }
    }
    return result;
}

void matrix_on_vector(matrix *a, vector *v) {
    vector result;
    vector_init_copy(&result, v);
    for (size_t i = 0; i < a->rows; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < a->rows; ++j) {
            sum += *(double *)matrix_get(a, i, j) * *(double *)vector_get(v, j);
        }
        *(double *)vector_get(&result, i) = sum;
    }
    vector_assign(v, &result);
    vector_free(&result);
}

void vector_normalize(matrix *a, vector *v) {
    matrix_on_vector(a, v);
    double norm_inf = vector_norm(v);
    for (size_t i = 0; i < v->size; ++i) {
        *(double *)vector_get(v, i) = 
            *(double *)vector_get(v, i) / norm_inf;
    }
}

void matrix_init_(matrix *a, vector *v) {
    for (size_t i = 0; i < a->rows; ++i) {
        *(double *)matrix_get(a, i, i) = 
            *(double *)matrix_get(a, i, i) - *(double *)vector_get(v, i);
    }
}

int main(void) {
    matrix a;
    vector x_0, x;
    double tmp, epsilon;
    size_t n;

    printf("matrix size?\n");
    scanf("%zu", &n);

    matrix_init(&a, n, sizeof(double));
    vector_init(&x_0, n, sizeof(double));
    vector_init(&x, n, sizeof(double));
    
    for (size_t i = 0; i < n; ++i) {
        tmp = 1.0;
        vector_push(&x, &tmp);
    }

    printf("matrix?\n");
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            scanf("%lf", &tmp);
            matrix_push(&a, (void *)&tmp);
        }
    }

    printf("eigen values?\n");
    for (size_t i = 0; i < n; ++i) {
        scanf("%lf", &tmp);
        vector_push(&x_0, &tmp);
    }

    printf("epsilon?\n");
    scanf("%lf", &epsilon);

    matrix_init_(&a, &x_0);
   
    do {
        vector_normalize(&a, &x_0);
        lup(&a, &x_0, &x);
        //vector_normalize(&a, &x_0);
        vector_assign(&x_0, &x);
    } while (vector_diff(&x, &x_0) > epsilon); 

    vector_print(&x);

    return 0;
}
