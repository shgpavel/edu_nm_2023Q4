#ifndef FUNC_C_
#define FUNC_C_

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define UNUSED(expr) do { (void)(expr); } while (0)

inline double func(double x) {
    return (tan(M_PI_2 - x) - x);
}

inline double func_d(double x) {
    return (-1.0/(sin(x) * sin(x)) - 1);
}

inline double func_1_x(double x, double y, double lambda) {
    UNUSED(y);
    return (lambda * sin(x + 1));
}

inline double func_1_y(double x, double y, double lambda) {
    UNUSED(x);
    UNUSED(y);
    UNUSED(lambda);
    return 2.0;
}

inline double func_2_x(double x, double y, double lambda) {
    UNUSED(x);
    UNUSED(y);
    UNUSED(lambda);
    return 1.0;
}

inline double func_2_y(double x, double y, double lambda) {
    UNUSED(x);
    return (cos(y) * lambda);
}

inline double func_1_neg(double x, double y, double lambda) {
    return (-2 * y + (lambda * cos(x + 1)));
}

inline double func_2_neg(double x, double y, double lambda) {
    return (-x - (lambda * sin(y)) - 0.4);
}

#endif
