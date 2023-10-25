#include <iostream>
#include <vector>

#include "main.hpp"

Matrix::Matrix(unsigned rows, unsigned columns)
    : data(rows, std::vector<double>(columns)) {}

Matrix::Matrix(const std::vector<std::vector<double>>& data)
    : data(data) {}

Matrix::~Matrix() {}

Matrix::Matrix(const Matrix& other)
    : data(other.data) {}

std::vector<double>& Matrix::operator[](unsigned index) {
    return data[index];
}

const std::vector<double>& Matrix::operator[](unsigned index) const {
    return data[index];
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;
    }

    data = other.data;
    return *this;
}


unsigned Matrix::rows() const {
    return data.size();
}

unsigned Matrix::columns() const {
    if (data.empty()) {
        return 0;
    }
    return data[0].size();
}

/*
Matrix Matrix::solve() const {
    return 0;
}
*/

double Matrix::norm_inf() const {
    double norm_inf = 0.0;
    for (size_t i = 0; i < this->rows(); ++i) {
        double row_sum = 0.0;

        for (size_t j = 0; j < this->columns(); ++j) {
            row_sum += std::abs(this[i][j]);
        }

        if (row_sum > norm_inf) {
            norm_inf = row_sum;
        }
    }

    return norm_inf;
}

int main(void) {
    
    int n, m;
    std::cout << "Give me dismensions of matix"<< std::endl;
    
    while (1) {
        try {
        std::cin >> n >> m;
        std::cout << std::endl;
        
        if (n < 1 || m < 1) {
            throw std::invalid_argument("Matrix size must be >= 1 1");
        } else {
            break;
        }
    
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    Matrix m10(n, m);
     

    std::cout << "Give me elements of matrix" << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cin >> m10[i][j];
        }
    }
    std::cout << std::endl;

/*
   try {
        
        if (m == 1) {
            throw std::invalid_argument("0");
        }

        Matrix m9 = m10.solve();
        std::cout << "m9 = solv m10:" << std::endl;
        for (unsigned i = 0; i < m9.rows(); i++) {
            for (unsigned j = 0; j < m9.columns(); j++) {
                std::cout << m9[i][j] << " ";
            }
            std::cout << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "> " << e.what() << std::endl;
    }
*/
    return 0;
}

