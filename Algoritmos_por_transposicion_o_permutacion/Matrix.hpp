#pragma once

#include <iostream>
#include <vector>

namespace mat {

    enum strategy {
        rowtrans = 0,
        coltrans
    };

    enum type {
        nKey = 0,
        key, 
        normal
    };

    template <class T>
    class Matrix {
    public:
        Matrix(int rows, int columns);
        Matrix(std::string message);
        Matrix(std::string message, unsigned n);
        Matrix(std::string message, std::string key);
        Matrix(std::string message, std::string key, unsigned n);
        Matrix(const Matrix& mat) {this->data = mat.data;};
        Matrix(const Matrix* mat) {this->data = mat->data;};
        virtual ~Matrix();
        void freeData();
        std::string getMessage();
        std::string getMessage(std::string key);
        static void transpose(Matrix<T>& matrix);
        friend std::ostream& operator<< <> (std::ostream& out, const Matrix<T>& mat);
        friend std::ostream& operator<< <> (std::ostream& out, const Matrix<T>* mat);
    private:
        T** data;
        unsigned int rows;
        unsigned int columns;
        static T** alloc(unsigned int rows, unsigned int columns);
    protected:
    };

    //typedef Matrix<char> mc2d;
}