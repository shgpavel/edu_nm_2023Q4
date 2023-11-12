#include <iostream>
#include <vector>

#include "main.hpp"

Vector::Vector(unsigned size) : data(size) {}

Vector::~Vector() {}


unsigned Vector::size() const {
    return data.size();
}

Vector& Vector::operator=(const Vector& other) {
    if (this == &other) {
        return *this;
    }

    data = other.data;
    return *this;
}


double& Vector::operator[](unsigned index) {
    return data[index];
}

const double& Vector::operator[](unsigned index) const {
    return data[index];
}

void Vector::print() const {
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << (*this)[i] << std::endl;
    }
}


Matrix::Matrix(unsigned rows, unsigned columns)
    : data(rows, std::vector<double>(columns)) {}

Matrix::Matrix(const Matrix& other)
    : data(other.data) {}

Matrix::~Matrix() {}

const std::vector<double>& Matrix::operator[](unsigned index) const {
    return data[index];
}

std::vector<double>& Matrix::operator[](unsigned index) {
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


double Matrix::norm_inf() const {
    double norm_inf = 0.0;
    for (size_t i = 0; i < this->rows(); ++i) {
        double row_sum = 0.0;

        for (size_t j = 0; j < this->columns(); ++j) {
            row_sum += (*this)[i][j];
        }

        if (row_sum > norm_inf) {
            norm_inf = row_sum;
        }
    }

    return norm_inf;
}


void Matrix::print() const {
    for (size_t i = 0; i < this->rows(); ++i) {
        for (size_t j = 0; j < this->columns(); ++j) {
            std::cout << (*this)[i][j] << " ";
        }
        std::cout << std::endl;
    } 
}

void MPI(Matrix& M, Vector& v1, Vector& solution, double eps) {
    Vector c(v1.size());
    c = v1;

    double mi = 1.0/M.norm_inf();
    for (size_t i = 0; i < c.size(); ++i) {
        c[i] = c[i] * mi;
    }
    
    Matrix B(M);
    for (size_t i = 0; i < B.rows(); ++i) {
        for (size_t j = 0; j < B.columns(); ++j) {
            if (i == j) {
                B[i][j] = 1 - (mi * B[i][j]);
            } else {
                B[i][j] = -mi * B[i][j];
            }
        }
    }

    if (B.norm_inf() < 1) {
         Vector x_cur(c.size());
         Vector x_next(c.size());
         x_cur = c;
         x_next = B * x_cur + c;
    }
}


int main(void) {
    
    int n, m;
    std::cout << "Give me dismensions of matrix"<< std::endl;
    
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
    for (size_t i = 0; i < m10.rows(); ++i) {
        for (size_t j = 0; j < m10.columns(); ++j) {
            std::cin >> m10[i][j];
        }
    }
    std::cout << std::endl; 

    std::cout << "Give me b vector dismensions" << std::endl;

    while (1) {
        try {
        std::cin >> n;
        std::cout << std::endl;
        
        if (n < 0) {
            throw std::invalid_argument("Vector dismensions must be >= 1");
        }
        
        if (n >= 0) {
            break;
        }

        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    Vector v1(n);

    std::cout << "Give me coordinates of 1st vector:" << std::endl;
    for (size_t i = 0; i < v1.size(); ++i) {
        std::cin >> v1[i];
    }
    std::cout << std::endl;
    /* Data is set */
    
    Vector solution(v1.size());
    MPI(m10, v1, solution);
    solution.print();

    return 0;
}

