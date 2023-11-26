#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

#include <eigen3/Eigen/Dense>


#include "main.hpp"

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

void Matrix::print() const {
    for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < columns(); ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << '\n';
    } 
}

void LUP(Matrix& M, Vector& c, Vector& solution) {
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

    for (unsigned test = 0; ; ++test) {
        size_t matrix_size;

        ifile >> matrix_size;

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


        /*
        This block have nothing with parse_test
        im just too lazy to make an io
        */ 

        std::cout << "Test " << test << ":" <<'\n';
        M.print();
        std::cout << '\n';
        b.print();

        eigen_solve(M, b, solution);
        std::cout << "Eigen: ";
        solution.print();

        LUP(M, b, solution); 
        std::cout << "LUP: ";
        solution.print();
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
