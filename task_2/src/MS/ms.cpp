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

int MS(const Matrix& M, const Vector& c, Vector& solution, const double epsilon) {
    
    Matrix A(M);
    Vector b(c);

    int k = 1;
    int flag = 0;

    if ( A.diag_domi() == -1 ) {
        Matrix A_trs(A.rows(), A.columns());
        A_trs = A.transpose();
        A = A_trs * A;
        b = A_trs * b;
        flag = 1;
    }

    Matrix B(A.rows(), A.columns());
    Vector l(b.size());
    for (size_t i = 0; i < A.rows(); ++i) {
        for (size_t j = 0; j < A.columns(); ++j) {
            if (i != j) {
                B[i][j] = -A[i][j] / A[i][i];
            }
        }
        l[i] = b[i] / A[i][i];
    }
    Vector x_next(l.size());
    Vector x_cur(l.size());

    if (flag == 0) {
        double stop_mult = B.norm_inf() / (1 - B.norm_inf());
        do {
            x_cur = x_next;
            for (size_t i = 0; i < B.rows(); ++i) {
                double sum = 0.0;
                for (size_t j = 0; j < B.columns(); ++j) {
                    sum += B[i][j] * x_next[j];
                }
                sum += l[i];
                x_next[i] = sum;
            }
            k++;
        } while ( stop_mult * x_next.norm_diff(x_cur) > epsilon );
        solution = x_next;
    } else if (flag == 1) {
        do {
            x_cur = x_next;
            for (size_t i = 0; i < B.rows(); ++i) {
                double sum = 0.0;
                for (size_t j = 0; j < B.columns(); ++j) {
                    sum += B[i][j] * x_next[j];
                }
                sum += l[i];
                x_next[i] = sum;
            }
            k++;
        } while ( (A * x_next).norm_diff(b) > epsilon );
        solution = x_next;
    }
    return k;
}
