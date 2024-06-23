#ifndef COMMON_H
#define COMMON_H


#define BOUND_A -1.0
#define BOUND_B 1.0
#define addition 1e-4

#define vector_val(v, i) (*(double *)vector_get(v, i))
#define matrix_val(m, i, j) (*(double *)matrix_get(m, i, j))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#endif
