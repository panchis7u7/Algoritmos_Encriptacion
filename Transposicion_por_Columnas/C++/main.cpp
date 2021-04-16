#include "Matrix.hpp"
#include "Matrix.cpp"

int main(int argc, char** argv){
    mat::Matrix<int>* a = new mat::Matrix<int>(2,2);
    a->aleatorizar();
    a->print();

    return 0;
}