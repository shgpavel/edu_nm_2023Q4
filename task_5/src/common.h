#ifndef COMMON_H
#define COMMON_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define unwrap_pair(ptr) (*(pair *)(ptr))
#define unwrap_double(ptr) (*(double *)(ptr))

#define pair_get(p, i) (*(pair *)vector_get(p, i))
#define vector_val(v, i) (*(double *)vector_get(v, i))
#define matrix_val(m, i, j) (*(double *)matrix_get(m, i, j))

#define divtol 1e-14
#define UNUSED(expr) \
  do {               \
    (void)(expr);    \
  } while (0)

#endif
