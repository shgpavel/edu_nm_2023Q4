#include <iostream>

#include "funcs.hpp"

double z(double x, double epsilon_u, double epsilon_phi, double epsilon_psi) {
    double u = sin(pow(x, 2) + 0.4, epsilon_u);
    double phi = square_rootPD(1 + pow(x, 2), epsilon_phi) / (1 - x);
    double psi = sinh(phi, epsilon_psi);
    return psi/u;
}

int main(void) {
    double lower, upper, step;
    double epsilon_u, epsilon_phi, epsilon_psi;

    printf( "Provide the input range as two floats\n" );
    scanf( "%lf%lf", &lower, &upper );

    printf( "Provide step info\n" );
    scanf( "%lf", &step );

    printf( "Provide epsilons for u func, phi, psi\n" );

    scanf( "%lf%lf%lf", &epsilon_u, &epsilon_phi, &epsilon_psi );
    
    printf("Thanks, working on it\n");

    for (double i = lower; i <= upper; i += step) {
        printf( "%.14lf\n", z(i, epsilon_u, epsilon_phi, epsilon_psi) );
    }

    return 0;
}

