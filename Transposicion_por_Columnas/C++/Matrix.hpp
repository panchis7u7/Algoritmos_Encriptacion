#pragma once

#include <iostream>
#include <vector>

namespace mat {

    template <class T>
    class Matrix {
    public:
        Matrix(unsigned int filas, unsigned int columnas);
        virtual ~Matrix();
        Matrix(const Matrix &mat) {this->data = mat.data;};
        void print();
        void aleatorizar();
    private:
        T** data;
        unsigned int filas;
        unsigned int columnas;
    protected:
    };
    typedef Matrix<char*> mat2Ds;
}