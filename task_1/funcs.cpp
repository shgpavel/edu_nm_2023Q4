#include <iostream>

double factorial(int n) {
    if (n <= 1) return 1.0;
    return n * factorial(n - 1);
}

double power(double base, int exponent) {
    double result = 1.0;
    if (exponent > 0) {
        for (int i = 0; i < exponent; i++) {
            result *= base;
        }
    } else if (exponent < 0) {
        for (int i = 0; i > exponent; i--) {
            result /= base;
        }
    }
    return result;
}

double sinus(double x, double epsilon) {
    double tmp = 0.1, result = 0.0;
    int ctr = 0;
    while (std::abs(result - tmp) >= epsilon) {
        tmp = result;
        result += (power(-1, ctr) / factorial(2 * ctr + 1)) * power(x, 2 * ctr + 1);
        ++ctr;
    }

    return result;
}

double sinhus(double x, double epsilon) {
    double tmp = 0.1, result = 0.0;
    int ctr = 0;
    while (std::abs(result - tmp)/3 >= epsilon) {
        tmp = result;
        result += power(x, 2 * ctr + 1) / factorial(2 * ctr + 1);
        ++ctr;
    }

    return result;
}

double square_rootPD(double num, double epsilon) {

    double guess = num;
    double prevguess = 0.0;

    while (std::abs(guess - prevguess) > epsilon) {
        prevguess = guess;
        guess = 0.5 * (guess + num / guess);
    }

    return guess;
}
