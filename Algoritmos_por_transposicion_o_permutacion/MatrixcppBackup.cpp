#include "Matrix.hpp"
#include <string.h>
#include <math.h>
#include <random>

namespace mat {

	template<class T>
	Matrix<T>::Matrix(int rows, int columns) {
		this->rows = rows;
		this->columns = columns;
		this->data = alloc(rows, columns);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//Create matrix cypher without key.

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

	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	//Create matrix cypher with key.

	template <class T>
	Matrix<T>::Matrix(std::string message, std::string key){
		if(opcode == opcodes::crypt){
			//srand(time(0));
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

			//Order alphabetically using linear O(n) bubble sort.
			for(int i = 0; i < this->rows-1; i++){
				if((int)this->data[i][0] > (int)this->data[i+1][0]){
					std::swap(this->data[i], this->data[i+1]);
					i = 0;
				}
			}

			Matrix<T>::transpose(*this);
			return;

		} else if (opcode == opcodes::decrypt){
			unsigned int rows = key.length();
			unsigned int columns = (ceil((message.length()/rows)));

			this->rows = rows;
			this->columns = columns;	
			this->data = Matrix<T>::alloc(rows, columns);

			int index = 0;

			for (size_t i = 0; i < this->rows; i++)
	    	{
				for (size_t j = 0; j < this->columns; j++)
				{
					this->data[i][j] = message[index];
					index++;
				}
	    	}
			
			return;

		} else {
			/* -------------- Randomize the matrix method. ----------------

			std::mt19937 rng(device());
			Uniform random numbers.
			std::uniform_int_distribution<int> gen(0, this->rows-1);
			unsigned int r;

			//Fisher–Yates algorithm.
			for (int i = this->rows-1; i != 0; i--) {
				//r = gen(rng); 
    			std::swap(this->data[i], this->data[r]); 
    		} 

			------------------------------------------------------------*/
			return;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	//Matrix object destructor.

    template <class T>
    Matrix<T>::~Matrix(){
        this->freeData();
    }

	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	//Matrix 2D data release.

	template <class T>
	void Matrix<T>::freeData(){
		for (size_t i = 0; i < this->rows; i++)
	    {
	    	delete[] this->data[i];
	    }
	    delete[] this->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	//Obtain uncyphered text from cyphered matrix.

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

	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	//Obtain uncyphered text from key cyphered matrix.

	template <class T>
	std::string Matrix<T>::getMessage(std::string key){
		std::replace(key.begin(), key.end(), ' ', '&');
		T** orderedCols = Matrix<T>::alloc(this->rows, this->columns);
		int* arr = (int*)malloc(sizeof(int) * key.length());
		memset(arr, 0, (sizeof(int) * key.length()));

		for (size_t i = 0; i < key.length(); i++)
		{
			for (size_t j = 0; j < this->rows; j++)
			{
				if((this->data[j][0] == key[i]) && (arr[j] != 1)){
					arr[j] = 1;
					orderedCols[i] = this->data[j];
					break;
				}
			}
		}

		for(int i = 0; i < this->rows; i++)
			this->data[i] = orderedCols[i];

		Matrix<T>::transpose(*this);

		return this->getMessage();
	}

	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	//2D matrix data allocator.

	template <class T>
	T** Matrix<T>::alloc(unsigned int rows, unsigned int columns){
	    T** data = new T* [rows];
	    for (size_t i = 0; i < rows; i++)
	    {
	    	data[i] = new T[columns];
	    }
		return data;
	}

	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	//Matrix transpose operation.

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

	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	//Matrix output stream operator overload for references.

	template <class T>
	std::ostream& operator<<(std::ostream& out, const Matrix<T>& mat){
		for (size_t i = 0; i < mat.rows; i++)
	    {
	    	out << "\x1B[37m" << "|";
	    	for (size_t j = 0; j < mat.columns; j++)
	    	{
	    		out << "  " << "\x1B[32m" << mat.data[i][j] << "  ";
	    	}
	    	out << "\x1B[37m" << "|";
	    	out << std::endl;
	    }
	    out << std::endl;	
		return out;
	}

	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	//Matrix output stream operator overload for pointers.

	template <class T>
	std::ostream& operator<<(std::ostream& out, const Matrix<T>* mat){
		for (size_t i = 0; i < mat->rows; i++)
	    {
	    	out << "\x1B[37m" << "|";
	    	for (size_t j = 0; j < mat->columns; j++)
	    	{
	    		out << "  " << "\x1B[32m" << mat->data[i][j] << "  ";
	    	}
	    	out << "\x1B[37m" << "|";
	    	out << std::endl;
	    }
	    out << std::endl;	
		return out;
	}
}

	////////////////////////////////////////////////////////////////////////////////////////