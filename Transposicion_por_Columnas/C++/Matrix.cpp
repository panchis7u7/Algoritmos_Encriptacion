#include "Matrix.hpp"
#include <string.h>
#include <math.h>

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
	Matrix<T>::Matrix(std::string message){
		float len = ceil(sqrt(message.length()));
		int padding =(pow(len, 2) - message.length());
		for(size_t i = 0; i < padding; ++i){
			message += "@";
		}
			
		this->data = alloc(len, len);

		int index = 0;
		for (size_t i = 0; i < this->filas; i++)
	    {
			for (size_t j = 0; j < this->columnas; j++)
			{
				this->data[i][j] = ((!(message[index] == ' ')*(int)message[index]))
					+ ((message[index] == ' ')*38);
				index++;
			}
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
	T** Matrix<T>::alloc(unsigned int filas, unsigned int columnas){
		this->filas = filas;
	    this->columnas = columnas;
	    this->data = new char* [this->filas];
	    for (size_t i = 0; i < this->filas; i++)
	    {
	    	this->data[i] = new char[this->columnas];
	    }
		return this->data;
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