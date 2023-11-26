class Vector {
public:
    Vector(size_t size);
    ~Vector();

    size_t size () const;
    
    double& operator[](size_t index);
    const double& operator[](size_t index) const;

    Vector& operator=(const Vector& other);
    Vector& operator-(const Vector& other);
    Vector& operator+(const Vector& other);
   
    void print() const;
private:
    std::vector<double> data;
};


class Matrix {
public:
    Matrix(size_t rows, size_t columns);
    Matrix(const Matrix& other);
    
    ~Matrix();
    
    size_t rows() const;
    size_t columns() const;


    std::vector<double>& operator[](size_t index);
    const std::vector<double>& operator[](size_t index) const;
    
    Matrix& operator=(const Matrix& other);
    

    void print() const;
private:
    std::vector<std::vector<double>> data;
};
