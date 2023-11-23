#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

#include <eigen3/Eigen/Dense>


#include "main.hpp"

typedef enum cases {first, second} case_t;
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
    double result = 0.0;
    for (size_t i = 0; i < size(); ++i) {
        result += pow(data[i] - other.data[i], 2);
    }
    return sqrt(result);
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

Matrix Matrix::transpose() const {
    Matrix result(rows(), columns());
    for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < columns(); ++j) {
            result[i][j] = data[j][i];
        }
    }
    return result;
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

/* Here I will destroy race condition
Matrix MPI_aux(Matrix M) {
    Matrix B(M);
    double mi = 1.0/M.norm_inf();
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
*/


void MPI(Matrix& M, Vector& b, Vector& solution, double epsilon) {
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

    if ( B.norm_inf() >= 1.0 && B.norm_1() >= 1.0 ) {
        if ( mpi_case == first ) {
            Matrix M_trs(M);
            Vector cache(b.size());

            M_trs = M.transpose();
            cache = b;

            M = M_trs * M;
            b = M_trs * cache;

            mpi_case = second;
            MPI(M, b, solution, epsilon);
            return;
        
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
            x_next = B * x_cur + c;
            cache = M * x_next;
            
            while (cache.norm_diff(b) > epsilon) {
                x_cur = x_next;
                x_next = B * x_cur + c;
                cache = M * x_next;
            }

            solution = x_next;
            return;
        }
    }

    Vector c(b.size());
    c = b;
    for (size_t i = 0; i < c.size(); ++i) {
        c[i] = c[i] * mi;
    }
    

    double stop_mult = B.norm_inf() / (1.0 - B.norm_inf());
    Vector x_cur(c.size());
    Vector x_next(c.size());


    x_next = c;
    do {
        x_cur = x_next;
        x_next = B * x_cur + c;
    } while ((stop_mult * x_next.norm_diff(x_cur)) > epsilon);

    solution = x_next;
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

void parse_test(std::string filename) {
    std::ifstream ifile(filename, std::ios::binary);

    if ( !ifile.is_open() ) {
        throw std::invalid_argument("No such file");
    }

    std::cout.precision(8); 
    while (1) {
        double epsilon;
        size_t matrix_size;

        ifile >> epsilon >> matrix_size;

        if (ifile.eof()) break;

        Matrix M(matrix_size, matrix_size);
        Vector b(matrix_size);
        Vector solution(matrix_size);

        for (size_t i = 0; i < matrix_size; ++i) {
            for (size_t j = 0; j < matrix_size; ++j) {
                ifile >> M[i][j];
            }
        }

        for (size_t i = 0; i < matrix_size; ++i) {
            ifile >> b[i];
        }

        M.print();
        b.print();

        eigen_solve(M, b, solution);
        std::cout << "Eigen:" << '\n';
        solution.print();

        std::cout << '\n';

        MPI(M, b, solution, epsilon); 
        std::cout << "MPI:" << '\n';
        solution.print();
        
        std::cout << "\n\n";
    }

    if (ifile.bad()) {
        throw std::runtime_error("Could not read from this file");
    }

    ifile.close();
}


int main(void) {

    while(1) {
        try {
            std::string filename;
            std::cout << "Give me filename: ";
            std::cin >> filename;
            parse_test(filename);
            break;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }
    
    return 0;
}
