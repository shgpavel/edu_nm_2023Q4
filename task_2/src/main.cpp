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
#include <string>
#include <fstream>

#include "main.hpp"

#include "MPI/mpi.hpp"
#include "MS/ms.hpp"
#include "LUP/lup.hpp"
#include "QR/qr.hpp"
#include "EigenWrap/eigen_wrap.hpp"
#include "Funcs/funcs.hpp"


Vector::Vector(size_t size) : data(size) {}

Vector::Vector(const Vector& other)
    : data(other.data) {}

Vector::~Vector() {}


size_t Vector::size() const {
    return data.size();
}

Vector& Vector::operator=(const Vector& other) {
    if (this == &other) {
        return *this;
    }

    data = other.data;
    return *this;
}

Vector& Vector::operator+(const Vector& other) {
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = data[i] + other.data[i];
    }
    return *this;
}

Vector& Vector::operator-(const Vector& other) {
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = data[i] - other.data[i];
    }
    return *this;
}

double& Vector::operator[](size_t index) {
    return data[index];
}

const double& Vector::operator[](size_t index) const {
    return data[index];
}

double Vector::norm_2(size_t start = 0) const {
    double result = 0.0;
    for (size_t i = start; i < size(); ++i) {
        result += data[i] * data[i];
    }
    return square_root_pd(result, 1e-14);
}

double Vector::norm_diff(const Vector& other) const {
    double result = 0.0;
    for (size_t i = 0; i < size(); ++i) {
        result += (data[i] - other.data[i]) * (data[i] - other.data[i]);
    }
    return square_root_pd(result, 1e-14);
}

void Vector::print() const {
    for (size_t i = 0; i < size(); ++i) {
        std::cout << data[i] << ' ';
    }
    std::cout << '\n';
}


Matrix::Matrix(size_t rows, size_t columns)
    : data(rows, std::vector<double>(columns)) {}

Matrix::Matrix(const Matrix& other)
    : data(other.data) {}

Matrix::~Matrix() {}

const std::vector<double>& Matrix::operator[](size_t index) const {
    return data[index];
}

std::vector<double>& Matrix::operator[](size_t index) {
    return data[index];
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;
    }

    data = other.data;
    return *this;
}

size_t Matrix::rows() const {
    return data.size();
}

size_t Matrix::columns() const {
    if (data.empty()) {
        return 0;
    }
    return data[0].size();
}

Vector Matrix::operator*(const Vector& vector) const {
    Vector result(rows());
    for (size_t i = 0; i < rows(); ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < columns(); ++j) {
            sum += data[i][j] * vector[j];
        }
        result[i] = sum;
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    Matrix result(rows(), other.columns());
    for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < other.columns(); ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < columns(); ++k) {
                sum += data[i][k] * other.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }
    return result;
}

Matrix Matrix::operator*(const double a) const {
    Matrix result(rows(), columns());
    for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < columns(); ++j) {
            result[i][j] = data[i][j] * a;
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix& other) const {
    Matrix result(rows(), other.columns());
    for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < other.columns(); ++j) {
            result[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::transpose() const {
    Matrix result(rows(), columns());
    for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < columns(); ++j) {
            result[i][j] = data[j][i];
        }
    }
    return result;
}

int Matrix::diag_domi() const {
    double sum = 0.0;
    for (size_t i = 0; i < rows(); ++i) {
        double a_diag = std::abs(data[i][i]);
        for (size_t j = 0; j < columns(); ++j) {
            if (i != j) {
                sum += std::abs(data[i][j]);
            }
        }
        if ( sum > a_diag ) {
            return -1;
        }
        sum = 0.0;
    }
    return 1;
}


double Matrix::norm_inf() const {
    double norm_inf = 0.0;
    for (size_t i = 0; i < rows(); ++i) {
        double row_sum = 0.0;

        for (size_t j = 0; j < columns(); ++j) {
            row_sum += std::abs(data[i][j]);
        }

        if (row_sum > norm_inf) {
            norm_inf = row_sum;
        }
    }
    return norm_inf;
}


double Matrix::norm_1() const {
    Matrix tmp((*this).transpose());
    return tmp.norm_inf();
}


void Matrix::print() const {
    for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < columns(); ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << '\n';
    } 
}


void parse_test(std::string filename) {
    std::ifstream ifile(filename, std::ios::binary);

    if ( !ifile.is_open() ) {
        throw std::invalid_argument("No such file");
    }

    for (unsigned test = 0; ; ++test) {
        double epsilon;
        size_t matrix_size;

        ifile >> epsilon >> matrix_size;

        if (ifile.eof()) break;

        Matrix M(matrix_size, matrix_size);
        Vector b(matrix_size);
        Vector solution(matrix_size);
        Vector solution_r(matrix_size);

        for (size_t i = 0; i < matrix_size; ++i) {
            for (size_t j = 0; j < matrix_size; ++j) {
                ifile >> M[i][j];
            }
        }

        for (size_t i = 0; i < matrix_size; ++i) {
            ifile >> b[i];
        }

        /*  Next block have nothing related to parse_test   */
        
        std::cout << "Test " << test << ":" <<'\n';
        
        eigen_solve(M, b, solution_r);
        std::cout << "Eigen: ";
        solution_r.print();

        MPI(M, b, solution, epsilon); 
        std::cout << "MPI: ";
        solution.print();

        MS(M, b, solution, epsilon); 
        std::cout << "MS: ";
        solution.print();
        
        LUP(M, b, solution); 
        std::cout << "LUP: ";
        solution.print();
        
        QR(M, b, solution); 
        std::cout << "QR: ";
        solution.print();

        std::cout << '\n';
    }

    if (ifile.bad()) {
        throw std::runtime_error("Could not read from this file");
    }

    ifile.close();
}

/*
void test_5() {
    // 15 times 1e-(x) x in 3-6
    const double epsilon_0 = 1.5e-5;
    const double epsilon_1 = 1e-6;
    for (size_t size = 4; size < 11; ++size) {
        Matrix A(size, size);
        Matrix Cache(A);
        Vector b(size);
        Vector solution(size);
        Vector solution_r(size);


        for (size_t i = 0; i < size; ++i) {
            
            for (size_t j = 0; j <= i; ++j) {
                Cache[i][j] = 1;
            }

            for (size_t j = i + 1; j < size; ++j) {
                Cache[i][j] = -1;
            }
        }

        Cache = Cache * epsilon_0;

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < i; ++j) {
                A[i][j] = 0;
            }

            A[i][i] = 1;

            for (size_t j = i + 1; j < size; ++j) {
                A[i][j] = -1;
            }
        }

        A = A + Cache;

        for (size_t i = 0; i < size - 1; ++i) {
            b[i] = -1;
        }
        b[size - 1] = 1;

        std::cout << "n: " << size << ":" <<'\n';
        
        eigen_solve(A, b, solution_r);
        std::cout << "Eigen: ";
        solution_r.print();

        int k = MPI(A, b, solution, epsilon_1); 
        std::cout << "MPI: ";
        solution.print();
    
        double delta = solution.norm_diff(solution_r);
        std::cout << "delta: " << delta << "  k: " << k << "\n\n";

        k = MS(A, b, solution, epsilon_1); 
        std::cout << "MS: ";
        solution.print();
        delta = solution.norm_diff(solution_r);
        std::cout << "delta: " << delta << "  k: " << k << "\n\n";

        LUP(A, b, solution); 
        std::cout << "LUP: ";
        solution.print();
        delta = solution.norm_diff(solution_r);
        std::cout << "delta: " << delta << '\n';

        
        QR(A, b, solution); 
        std::cout << "QR: ";
        solution.print();
        delta = solution.norm_diff(solution_r);
        std::cout << "delta: " << delta << '\n';

   }
}
*/

int main(void) {
    while(1) {
        try {
            std::string filename;
            std::cout << "Test file path?" << '\n';
            std::cin >> filename;
            parse_test(filename);
            break;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }

    /*
    test_5();
    */

    return 0;
}
