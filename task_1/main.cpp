#include <iostream>

#include "funcs.hpp"

double z(double x) {
    double u = sin(pow(x, 2) + 0.4, 30);
    double phi = square_rootPD(1 + pow(x, 2), 1e-6) / (1 - x);
    double psi = sinh(phi, 30);
    return psi/u;
}

int main(void) {
    double input;
    scanf("%lf", &input);

    printf( "%.14lf\n", z(input));
    return 0;
}

