#include "Matrix.hpp"
#include <string.h>
#include <math.h>
#include <random>

namespace mat {

	template <class T>
	Matrix<T>::Matrix(std::string message){
		float len = ceil(sqrt(message.length()));
		int padding =(pow(len, 2) - message.length());
		for(size_t i = 0; i < padding; ++i){
			message += "@";
		}

		this->rows = len;
		this->columns = len;	
		this->data = Matrix<T>::alloc(len, len);

		int index = 0;
		for (size_t i = 0; i < this->rows; i++)
	    {
			for (size_t j = 0; j < this->columns; j++)
			{
				this->data[i][j] = ((!(message[index] == ' ')*(int)message[index]))
					+ ((message[index] == ' ')*38);
				index++;
			}
	    }
	}

	template <class T>
	Matrix<T>::Matrix(std::string message, std::string key){
		srand(time(0));
		std::replace(key.begin(), key.end(), ' ', '&');
		float columns = key.length();
		float rows = (ceil((message.length()/columns)) + 1);
		int padding = ((rows * columns) - (columns + message.length()));
		for(size_t i = 0; i < padding; ++i){
			message += "@";
		}

		this->rows = rows;
		this->columns = columns;	
		this->data = Matrix<T>::alloc(rows, columns);

		int index = 0;

		strncpy(this->data[0], key.c_str(), key.length());

		for (size_t i = 1; i < this->rows; i++)
	    {
			for (size_t j = 0; j < this->columns; j++)
			{
				this->data[i][j] = ((!(message[index] == ' ')*(int)message[index]))
					+ ((message[index] == ' ')*38);
				index++;
			}
	    }

		Matrix<T>::transpose(*this);
		std::cout << this << std::endl;

		//Randomize the matrix.
		std::random_device device;
		std::mt19937 rng(device());
		//Uniform random numbers.
		std::uniform_int_distribution<int> gen(0, this->rows-1);
		unsigned int r;

		//Fisher–Yates algorithm.
		for (int i = this->rows-1; i != 0; i--) {
			r = gen(rng); 
    		std::swap(this->data[i], this->data[r]); 
    	} 

		//std::cout << this <<std::endl;

		Matrix<T>::transpose(*this);
	}

    template <class T>
    Matrix<T>::~Matrix(){
        this->freeData();
    }

	template <class T>
	void Matrix<T>::freeData(){
		for (size_t i = 0; i < this->rows; i++)
	    {
	    	delete[] this->data[i];
	    }
	    delete[] this->data;
	}

	template <class T>
	std::string Matrix<T>::getMessage(){
		std::string message = "";
		for (size_t i = 0; i < this->rows; i++)
	    {
	    	for (size_t j = 0; j < this->columns; j++)
	    	{
	    		message += this->data[i][j];
	    	}
	    }
		return message;
	}

	template <class T>
	std::string Matrix<T>::getMessage(std::string key){
		std::string message = "";
		Matrix<T>::transpose(*this);
		for (size_t i = 0; i < key.length; i++)
		{
			for (size_t j = 0; j < this->filas; j++)
			{
				if(this->data[j][0] == key[i]){
					std::swap(this->data[j], this->data[r]); 
				}
			}
			
			
		}
		
		return message;
	}

	template <class T>
	T** Matrix<T>::alloc(unsigned int rows, unsigned int columns){
	    T** data = new T* [rows];
	    for (size_t i = 0; i < rows; i++)
	    {
	    	data[i] = new T[columns];
	    }
		return data;
	}

	template <class T>
	void Matrix<T>::transpose(Matrix<T>& matrix){
		T** data = Matrix<T>::alloc(matrix.columns, matrix.rows);

		for (size_t i = 0; i < matrix.rows; i++)
		{
			for (size_t j = 0; j < matrix.columns; j++)
			{
				data[j][i] = matrix.data[i][j];
			}
		}
		matrix.freeData();
		unsigned int rows = matrix.rows;
		matrix.rows = matrix.columns;
		matrix.columns = rows;
		matrix.data = data;
	}

	template <class T>
	std::ostream& operator<<(std::ostream& out, const Matrix<T>& mat){
		for (size_t i = 0; i < mat.rows; i++)
	    {
	    	out << "|";
	    	for (size_t j = 0; j < mat.columns; j++)
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
		for (size_t i = 0; i < mat->rows; i++)
	    {
	    	out << "|";
	    	for (size_t j = 0; j < mat->columns; j++)
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