class Vector {
public:
    Vector(unsigned size);
    ~Vector();

    unsigned size () const;

    Vector& operator=(const Vector& other);

    double& operator[](unsigned index);
    const double& operator[](unsigned index) const;
    
    void print() const;

private:
    std::vector<double> data;
};


class Matrix {
public:
    Matrix(unsigned rows, unsigned columns);
    Matrix(const Matrix& other);
    
    ~Matrix();

    const std::vector<double>& operator[](unsigned index) const;

    std::vector<double>& operator[](unsigned index);
    
    Matrix& operator=(const Matrix& other);

    double norm_inf() const;

    unsigned rows() const;

    unsigned columns() const;

    void print() const;

private:
    std::vector<std::vector<double>> data;
};
