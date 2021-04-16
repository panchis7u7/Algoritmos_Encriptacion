#include "Matrix.hpp"
#include "Matrix.cpp"

int main(int argc, char** argv){
    (void)argc;
    (void)argv;
    std::cout << "prueba." << std::endl;
    mat::Matrix<int>* a = new mat::Matrix<int>(4,4);
    a->aleatorizar();
    std::cout << a;

    mat::Matrix<int> b(2,2);
    b.aleatorizar();
    std::cout << b;

    mat::mc2d prueba(2,2);

    return 0;
}