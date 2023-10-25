class Matrix {
public:
    Matrix(unsigned rows, unsigned columns);
    Matrix(const std::vector<std::vector<double>>& data);

    ~Matrix();

    Matrix(const Matrix& other);

    std::vector<double>& operator[](unsigned index);
    const std::vector<double>& operator[](unsigned index) const;
    
    Matrix& operator=(const Matrix& other);

    unsigned norm_inf() const;

    unsigned rows() const;

    unsigned columns() const;

    Matrix solve() const;

private:
    std::vector<std::vector<double>> data;
};
