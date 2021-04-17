#pragma once

#include <iostream>
#include <vector>

namespace mat {

    template <class T>
    class Matrix {
    public:
        Matrix(unsigned int filas, unsigned int columnas);
        Matrix(std::string message);
        virtual ~Matrix();
        Matrix(const Matrix &mat) {this->data = mat.data;};
        Matrix<T> cifrar(const char* mensaje);
        void aleatorizar();
        friend std::ostream& operator<< <> (std::ostream& out, const Matrix<T>& mat);
        friend std::ostream& operator<< <> (std::ostream& out, const Matrix<T>* mat);
    private:
        T** data;
        unsigned int filas;
        unsigned int columnas;
        T** alloc(unsigned int filas, unsigned int columnas);
    protected:
    };

    typedef Matrix<char> mc2d;
}