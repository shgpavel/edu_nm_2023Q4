#ifndef COMMON_H
#define COMMON_H


#define vector_val(v, i) *(double *)vector_get(v, i)
#define matrix_val(m, i, j) *(double *)matrix_get(m, i, j)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define rtol 1e-6
#define epsilon_c 1e-8
#define delta_c 1e-8

#define BOUND_A -10.0
#define BOUND_B 10.0

#define INITIAL rng(BOUND_B, BOUND_A)

#endif
