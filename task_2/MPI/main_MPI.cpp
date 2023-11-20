#include <iostream>
#include <vector>
#include <cmath>

#include <eigen3/Eigen/Dense>


#include "main.hpp"

#define epsilon 1e-6
#define N 15


typedef enum cases {first, second, third, fourth, fifth} case_t;
case_t mpi_case = first;

Vector::Vector(size_t size) : data(size) {}

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

double Vector::norm_diff(const Vector& other) const {
    double result = 0;
    for (size_t i = 0; i < this->size(); ++i) {
        result += pow((*this)[i] - other[i], 2);
    }
    return sqrt(result);
}

void Vector::print() const {
    for (size_t i = 0; i < this->size(); ++i) {
        std::cout << (*this)[i] << '\n';
    }
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

/*
Matrix Matrix::operator=(std::initializer_list<double>& a) {
    try ()
    Matrix result(a.size(), a.size());
}
*/

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
    for (unsigned i = 0; i < rows(); ++i) {
        for (unsigned j = 0; j < other.columns(); ++j) {
            double sum = 0.0;
            for (unsigned k = 0; k < columns(); ++k) {
                sum += data[i][k] * other.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }
    return result;
}

Matrix Matrix::transpose() const {
    Matrix result(*this);
    for (size_t i = 0; i < this->rows(); ++i) {
        for (size_t j = 0; j < this->columns(); ++j) {
            result[i][j] = (*this)[j][i];
        }
    }
    return result;
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


double Matrix::norm_1() const {
    Matrix tmp((*this).transpose());
    return tmp.norm_inf();
}


void Matrix::print() const {
    for (size_t i = 0; i < this->rows(); ++i) {
        for (size_t j = 0; j < this->columns(); ++j) {
            std::cout << (*this)[i][j] << " ";
        }
        std::cout << '\n';
    } 
}

unsigned MPI(Matrix& M, Vector& b, Vector& solution) {
    unsigned k = 1;
    double mi = 1.0/M.norm_inf();
    
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
    

    if ( B.norm_inf() >= 1 && B.norm_1() >= 1 ) {
        if ( mpi_case == first ) {
            Matrix M_trs(M);
            Vector cache(b.size());

            M_trs = M.transpose();
            cache = b;

            M = M * M_trs;
            b = M_trs * cache;

            mpi_case = second;
            k = MPI(M, b, solution);
            return k;
        } else if ( mpi_case == second ) {
            Vector c(b.size());
            c = b;
            for (size_t i = 0; i < c.size(); ++i) {
                c[i] = c[i] * mi;
            }
            
            Vector x_cur(c.size());
            Vector x_next(c.size());
            Vector cache(c.size());
            
            x_cur = c;
            cache = B * x_cur;
            x_next = cache + c;
            cache = M * x_next;
            
            while (cache.norm_diff(b) > epsilon) {
                x_cur = x_next;
                cache = B * x_cur;
                x_next = cache + c;
                ++k;
                cache = M * x_next;
            }

            solution = x_next;
            return k;
 
        }
    }

    Vector c(b.size());
    c = b;
    for (size_t i = 0; i < c.size(); ++i) {
        c[i] = c[i] * mi;
    }
    

    Vector x_cur(c.size());
    Vector x_next(c.size());
    Vector cache(c.size());
    double stop_mult = B.norm_inf() / (1 - B.norm_inf());


    x_cur = c;
    do {
        cache = x_cur;
        x_cur = x_next;
        x_next = B * cache + c;
        ++k;
    } while ((stop_mult * x_next.norm_diff(x_cur)) > epsilon);

    solution = x_next;
    return k;
}


void eigen_solve(Matrix& A, Vector& b, Vector& solution) {
	Eigen::MatrixXd eA;
	Eigen::VectorXd eb, eres;
	eb.resize(b.size());
	eA.resize(A.rows(), A.columns());
	
    for (size_t i = 0; i < A.rows(); i++) {
	    for (size_t j = 0; j < A.columns(); j++) {
			eA(i, j) = A[i][j];
		}
	}

	for (size_t i = 0; i < b.size(); i++) {
		eb[i] = b[i];
	}

	eres = eA.colPivHouseholderQr().solve(eb);
	for (size_t i = 0; i < solution.size(); i++) {
		solution[i] = eres[i];
    }
}

void test(case_t num, Matrix& M, Vector& b) {
    switch (num) {
        case first:
            M = { 0, 2, 3, 1, 2, 4, 4, 5, 6 };
            b = { 13, 17, 32 };
    }
}

int main(void) {
    
    /*
    size_t n;
    std::cout << "Give me matrix dismension:"<< '\n';
    
    while (1) {
        try {
        std::cin >> n;
        std::cout << '\n';
        
        if (n < 1) {
            throw std::invalid_argument("Matrix size must be >= 1 1;");
        } else {
            break;
        }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << ";" << '\n';
        }
    }

    Matrix m10(n, n);
    std::cout << "Give me elements of A matrix:" << '\n';
    for (size_t i = 0; i < m10.rows(); ++i) {
        for (size_t j = 0; j < m10.columns(); ++j) {
            std::cin >> m10[i][j];
        }
    }
    std::cout << '\n';

    Vector v1(n);
    std::cout << "Give me coordinates of b vector:" << '\n';
    for (size_t i = 0; i < v1.size(); ++i) {
        std::cin >> v1[i];
    }
    std::cout << '\n';

     Data is set 
    
    Matrix m10(3, 3);
    Vector v1(3);
    Vector solution(v1.size());

    test(first, m10, v1);
    eigen_solve(m10, v1, solution);
    std::cout << "Eigen:" << '\n';
    solution.print();

    unsigned g = MPI(m10, v1, solution);
    std::cout << "MPI:" << '\n';
    solution.print();
    std::cout << "Number of iters: " << g << std::endl;
    */
    return 0;
}

