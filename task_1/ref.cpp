#include <iostream>
#include <cmath>


#define epsilon_sigma 1e-14


int main(void) {
    
    double lower, upper, step;

    printf( "Provide the input range as two floats:\n" );
    scanf( "%lf%lf", &lower, &upper );

    printf( "Provide step info:\n" );
    scanf( "%lf", &step );

    
    for (double i = lower; i < upper + epsilon_sigma; i += step) {
        printf("%.14lf\n", sinh(sqrt(pow(i, 2) + 1) / (1 - i)) / sin(pow(i, 2) + 0.4));
    }



    
    return 0;
}
