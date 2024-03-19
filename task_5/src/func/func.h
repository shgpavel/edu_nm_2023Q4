#ifndef FUNC_C
#define FUNC_C

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define UNUSED(expr) do { (void)(expr); } while (0)

inline double func(double x) {
    return (tan(M_PI_2 - x) - x);
}

#endif
