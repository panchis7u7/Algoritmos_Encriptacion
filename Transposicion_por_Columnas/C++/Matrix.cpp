#include "Matrix.hpp"
#include <string.h>
#include <math.h>

namespace mat {

	template <class T>
	Matrix<T>::Matrix(std::string message){
		float len = ceil(sqrt(message.length()));
		int padding =(pow(len, 2) - message.length());
		for(size_t i = 0; i < padding; ++i){
			message += "@";
		}

		this->filas = len;
		this->columnas = len;	
		this->data = Matrix<T>::alloc(len, len);

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
        this->freeData();
    }

	template <class T>
	void Matrix<T>::freeData(){
		for (size_t i = 0; i < this->filas; i++)
	    {
	    	delete[] this->data[i];
	    }
	    delete[] this->data;
	}

	template <class T>
	std::string Matrix<T>::getMessage(){
		std::string message = "";
		for (size_t i = 0; i < this->filas; i++)
	    {
	    	for (size_t j = 0; j < this->columnas; j++)
	    	{
	    		message += this->data[i][j];
	    	}
	    }
		return message;
	}

	template <class T>
	T** Matrix<T>::alloc(unsigned int filas, unsigned int columnas){
	    T** data = new T* [filas];
	    for (size_t i = 0; i < filas; i++)
	    {
	    	data[i] = new T[columnas];
	    }
		return data;
	}

	template <class T>
	void Matrix<T>::transpuesta(Matrix<T>& matrix){
		T** data = Matrix<T>::alloc(matrix.columnas, matrix.filas);

		for (size_t i = 0; i < matrix.filas; i++)
		{
			for (size_t j = 0; j < matrix.columnas; j++)
			{
				data[j][i] = matrix.data[i][j];
			}
		}
		matrix.freeData();
		matrix.data = data;
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