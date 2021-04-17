#include "Matrix.hpp"
#include "Matrix.cpp"
#include <unistd.h>

int main(int argc, char* argv[]){
    int opt;
    while((opt = getopt(argc, argv, ":t:f:lrx:")) != -1){
        switch(opt){
            case 't':
                std::cout << optarg << std::endl;
                break;
            case 'f':
                printf("opcion f.\n");
                break;
            case 'l':
                printf("opcion l.\n");
                break;
            case 'r':
                printf("opcion r.\n");
                break;
            case ':':
                printf("opcion no soportada.\n");
                break;
            case '?':
                printf("opcion desconocida.\n");
                break;
        }
    }

    mat::Matrix<char> prueba("Hola Como Estas? Te Saludo Desde Marte!!!!, Mi Contrasena Es 123456");
    std::cout << prueba << std::endl;
    return 0;
}