#include "Matrix.hpp"

namespace mat {
    
    template <class T>
    Matrix<T>::Matrix(unsigned int filas, unsigned int columnas){
        this->filas = filas;
	    this->columnas = columnas;
	    this->data = new T* [this->filas];
	    for (size_t i = 0; i < this->filas; i++)
	    {
	    	this->data[i] = new T[this->columnas];
	    }
    }

    template <class T>
    Matrix<T>::~Matrix(){
        for (size_t i = 0; i < this->filas; i++)
	    {
	    	delete[] data[i];
	    }
	    delete[] data;
    }

    template <class T>
    void Matrix<T>::aleatorizar() {
	    for (size_t i = 0; i < this->filas; i++)
	    {
	    	for (size_t j = 0; j < this->columnas; j++)
	    	{
	    		//Genera numero aleatorio entre -1 y 1
	    		this->data[i][j] = (-1) + static_cast <T> (rand()) / (static_cast <T> (RAND_MAX / (1 - (-1)))); 
	    	}
	    }
    }

	template <class T>
	std::ostream& operator<<(std::ostream& out, const Matrix<T>& mat){
		for (size_t i = 0; i < mat.filas; i++)
	    {
	    	out << "|";
	    	for (size_t j = 0; j < mat.columnas; j++)
	    	{
	    		out << "  " << mat.data[i][j] << "  ";
	    	}
	    	out << "|";
	    	out << std::endl;
	    }
	    out << std::endl;	
		return out;
	}

	template <class T>
	std::ostream& operator<<(std::ostream& out, const Matrix<T>* mat){
		for (size_t i = 0; i < mat->filas; i++)
	    {
	    	out << "|";
	    	for (size_t j = 0; j < mat->columnas; j++)
	    	{
	    		out << "  " << mat->data[i][j] << "  ";
	    	}
	    	out << "|";
	    	out << std::endl;
	    }
	    out << std::endl;	
		return out;
	}
}