/*
Copyright 2023 Pavel Shago

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdio.h>
#include <math.h>

double factorial(int n) {
    double result = n;
    for (int i = 1; i < n; ++i) {
        result *= i;
    }
    return result;
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
    double result = 0.0, tmp = 0.0;
    int ctr = 0;
    do {
        tmp = (power(-1, ctr) / factorial(2 * ctr + 1)) * power(x, 2 * ctr + 1);
        result += tmp;
        ++ctr;
    } while (fabs(tmp) >= epsilon); 

    return result;
}

double sinhus(double x, double epsilon) {
    double result = 0.0, tmp = 0.0;
    int ctr = 0;
    do {
        tmp = power(x, 2 * ctr + 1) / factorial(2 * ctr + 1);
        result += tmp;
        ++ctr;
    } while (fabs(tmp)/3 >= epsilon);

    return result;
}

double square_rootPD(double num, double epsilon) {

    double guess = num;
    double prevguess = 0.0;

    while (fabs(guess - prevguess) > epsilon) {
        prevguess = guess;
        guess = 0.5 * (guess + num / guess);
    }

    return guess;
}
