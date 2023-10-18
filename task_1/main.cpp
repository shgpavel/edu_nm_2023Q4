#include <iostream>
#include <cmath>

#include "funcs.hpp"

#define epsilon_sigma 1e-14

void z(double x, double epsilon_u,
        double epsilon_phi, double epsilon_psi, 
        FILE *csvout) {
    
    double u = sinus(power(x, 2) + 0.4, epsilon_u);
    double phi = square_rootPD(1 + power(x, 2), epsilon_phi) / (1 - x);
    double psi = sinhus(phi, epsilon_psi);
    
    double u_ = sin(pow(x, 2) + 0.4);
    double phi_ = sqrt(pow(x, 2) + 1) / (1 - x);
    double psi_ = sinh(phi_);

    fprintf(csvout, "%lf, %lf, %e, %lf, %e, %lf, %e, %lf, %e, %lf, "
            "%e, %lf, %e, %lf, %e, %lf, %lf\n",
            x, u, epsilon_u, u_, std::abs(u_ - u), phi, epsilon_phi, phi_,
            std::abs(phi_ - phi), psi, epsilon_psi, psi_, std::abs(psi_ - psi),
            psi/u, 1e-6, psi_/u_, std::abs(psi_/u_ - psi/u));
}

int main(void) {
    double lower, upper, step;
    double epsilon_u, epsilon_phi, epsilon_psi;
    FILE *csvout = fopen("output.csv", "w");

    printf( "Provide the input range as two floats:\n" );
    scanf( "%lf%lf", &lower, &upper );

    printf( "Provide step info:\n" );
    scanf( "%lf", &step );

    printf( "Provide epsilons for u func, phi, psi:\n" );

    scanf( "%lf%lf%lf", &epsilon_u, &epsilon_phi, &epsilon_psi );
    
    fprintf(csvout, "x, u(x), delta_u, u_(x), delta_u_, phi(x), delta_phi, phi_(x), "
                    "delta_phi_, psi(x), delta_psi, psi_(x), delta_psi_, z(x), "
                    "delta_z, z_(x), delta_z_\n");    
    
    for (double i = lower; i < upper + epsilon_sigma; i += step) {
        z(i, epsilon_u, epsilon_phi, epsilon_psi, csvout);
    }

    fclose(csvout);
    return 0;
}

