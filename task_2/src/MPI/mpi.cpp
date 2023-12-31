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

#include <iostream>
#include <vector>

#include "../main.hpp"

/*
    If MPI_aux will be calculated in MPI
    a smth like race condition will occur
    with this type of file reading to pass matrices
*/


Matrix MPI_aux(const Matrix A) {
    Matrix B(A);
    double mi = 1.0/A.norm_inf();
    for (size_t i = 0; i < B.rows(); ++i) {
        for (size_t j = 0; j < B.columns(); ++j) {
            if (i == j) {
                B[i][j] = 1 - (mi * B[i][j]);
            } else {
                B[i][j] = -mi * B[i][j];
            }
        }
    }

    return B;
}


int MPI(const Matrix& M, const Vector& c_in, Vector& solution, const double epsilon) {
    Matrix A(M);
    Vector b(c_in);

    int k = 1;
    double mi = 1.0/A.norm_inf();
    Matrix B(A);
    B = MPI_aux(A);

    if ( B.norm_inf() >= 1.0 && B.norm_1() >= 1.0 ) {
        Matrix A_trs(A);
        Vector cache(b);

        A_trs = A.transpose();

        A = A_trs * A;
        b = A_trs * cache;

        mi = 1.0/A.norm_inf();
        B = MPI_aux(A);

        if ( B.norm_inf() >= 1.0 && B.norm_1() >= 1.0) {
            Vector c(b);
            for (size_t i = 0; i < c.size(); ++i) {
                c[i] = c[i] * mi;
            }
            
            Vector x_cur(c);
            Vector x_next(c.size());
            Vector cache(c.size());

            x_next = B * x_cur + c;
            cache = A * x_next;
            while (cache.norm_diff(b) > epsilon) {
                x_cur = x_next;
                x_next = B * x_cur + c;
                cache = A * x_next;
                ++k;
            }

            solution = x_next;
            return k - 1;
        }
    }

    Vector c(b);
    for (size_t i = 0; i < c.size(); ++i) {
        c[i] = c[i] * mi;
    }
    

    double stop_mult = B.norm_inf() / (1.0 - B.norm_inf());
    Vector x_cur(c.size());
    Vector x_next(c);

    do {
        x_cur = x_next;
        x_next = B * x_cur + c;
        ++k;
    } while ((stop_mult * x_next.norm_diff(x_cur)) > epsilon);

    solution = x_next;
    return k;
}
