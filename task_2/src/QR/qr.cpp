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

void QR(const Matrix& M, const Vector& c, Vector& solution) {
    Matrix A(M);
    Vector b(c);
    
    Matrix Q(A.rows(), A.columns());
    Matrix Cache(A.rows(), A.columns());
    
    Vector y(b.size());
    Vector cache(b.size());
    
    for (size_t i = 0; i < A.columns() - 1; ++i) {
        for (size_t j = i; j < A.rows(); ++j) {
            y[j] = A[j][i];
        }
        
        y[i] = (y[i] - y.norm_2(i));
        
        double y_norm_2 = y.norm_2(i);
        for (size_t j = i; j < A.rows(); ++j) {
            y[j] = (y[j] / y_norm_2);
        }

        for (size_t j = i; j < A.rows(); ++j) {
            for (size_t k = i; k < A.columns(); ++k) {
                Q[j][k] = -2.0 * y[j] * y[k];
            }
            Q[j][j] = Q[j][j] + 1;
        }

        for (size_t j = i; j < A.rows(); ++j) {
            for (size_t k = i; k < A.columns(); ++k) {
                double sum = 0.0;
                for (size_t l = i; l < A.rows(); ++l) {
                    sum = sum + (Q[j][l] * A[l][k]);
                }
                Cache[j][k] = sum;
            }
        }


        for (size_t j = i; j < A.rows(); ++j) {
            for (size_t k = i; k < A.columns(); ++k) {
                A[j][k] = Cache[j][k];
            }
        }

        for (size_t j = i; j < A.rows(); ++j) {
            double sum = 0.0;
            for (size_t k = i; k < A.columns(); ++k) {
                sum = sum + (Q[j][k] * b[k]);
            }
            cache[j] = sum;
        }
        
        for (size_t j = i; j < A.rows(); ++j) {
            b[j] = cache[j];
        }
    }

    for (size_t i = A.rows(); i > 0; ) {
        --i;
        solution[i] = b[i];
        for (size_t j = A.rows(); j > i + 1; ) {
            --j;
            solution[i] = solution[i] - (A[i][j] * solution[j]);
        }
        solution[i] = solution[i] / A[i][i];
    }

}
