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

void LUP(const Matrix& M, const Vector& c, Vector& solution) {
    Matrix A(M);
    Vector b(c.size());

    std::vector<size_t> perm(A.rows());
    for (size_t i = 0; i < A.rows(); ++i) {
        perm[i] = i;
    }

    for (size_t i = 0; i < A.rows(); ++i) {
        double lead = 0;
        size_t swap = i;
        for (size_t j = i + 1; j < A.columns(); ++j) {
            if (std::abs(lead) < std::abs(A[j][i])) {
                lead = A[j][i];
                swap = j;
            }
        }
        
        if (swap != i) {
            std::swap(perm[i], perm[swap]);
            std::swap(A[i], A[swap]);
        }

        for (size_t j = i + 1; j < A.rows(); ++j) {
            A[j][i] /= lead;
            for (size_t k = i + 1; k < A.columns(); ++k) {
                A[j][k] -= A[j][i] * A[i][k];
            }
        }
    }

    for (size_t i = 0; i < b.size(); ++i) {
        b[i] = c[perm[i]];
    }


    for (size_t i = 0; i < A.rows(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            b[i] = b[i] - (A[i][j] * b[j]);
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
