#ifndef COMMON_H
#define COMMON_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define unwrap_double(ptr) (*(double *) (ptr))

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define UNUSED(expr) do { (void)(expr); } while (0)

#define unwrap_pair(ptr) (*(pair *) (ptr))

#endif
