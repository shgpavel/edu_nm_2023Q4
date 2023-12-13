#include <stdio.h>

#include "../matrix/matrix.h"

int main(void) {

    vector b;
    vector_init(&b, 3, sizeof(size_t));
    for (size_t i = 0; i < 3; ++i) {
        vector_set_grow(&b, i, &i);
    }
    vector_print_st(&b);
    vector_swap(&b, 1, 2);
    vector_print_st(&b);

    vector_free(&b);

    return 0;
}
