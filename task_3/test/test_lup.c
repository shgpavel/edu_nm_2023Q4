#include <stdio.h>

#include "../lup/lup.h"
#include "../matrix/matrix.h"
#include "../vector/vector.h"

int main(void) {
    matrix a;
    vector b;
    vector solution;
    double tmp;
    
    matrix_init(&a, 3, sizeof(double));
    vector_init(&b, 3, sizeof(double));

    printf("Give me matrix\n");
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            scanf("%lf", &tmp);
            matrix_push(&a, (void *)&tmp);
        }
    }

    printf("Give me vector\n");
    for (size_t i = 0; i < 3; ++i) {
        scanf("%lf", &tmp);
        vector_push(&b, &tmp);
    }
    vector_init_copy(&solution, &b);

    lup(&a, &b, &solution);
    vector_print(&solution);

    return 0;
}
