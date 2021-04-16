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
    void Matrix<T>::print() {
	    for (size_t i = 0; i < filas; i++)
	    {
	    	std::cout << "|";
	    	for (size_t j = 0; j < columnas; j++)
	    	{
	    		std::cout << "  " << this->data[i][j] << "  ";
	    	}
	    	std::cout << "|";
	    	std::cout << std::endl;
	    }
	    std::cout << std::endl;
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

}