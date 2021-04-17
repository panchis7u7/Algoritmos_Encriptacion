#pragma once

#include <iostream>
#include <vector>

namespace mat {

    template <class T>
    class Matrix {
    public:
        Matrix(std::string message);
        virtual ~Matrix();
        Matrix(const Matrix &mat) {this->data = mat.data;};
        Matrix<T>(const char* mensaje);
        void freeData();
        std::string getMessage();
        static void transpuesta(Matrix<T>& matrix);
        friend std::ostream& operator<< <> (std::ostream& out, const Matrix<T>& mat);
        friend std::ostream& operator<< <> (std::ostream& out, const Matrix<T>* mat);
    private:
        T** data;
        unsigned int filas;
        unsigned int columnas;
        static T** alloc(unsigned int filas, unsigned int columnas);
    protected:
    };

    //typedef Matrix<char> mc2d;
}