#ifndef COMMON_H
#define COMMON_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define unwrap_double(ptr) (*(double *) (ptr))
#define UNUSED(expr) do { (void)(expr); } while (0)
#define unwrap_pair(ptr) (*(pair *) (ptr))
#define rtol 1e-6
#define epsilon_c 1e-8
#define delta_c 1e-8

#define BOUND_A 1.0
#define BOUND_B 10.0


#endif
