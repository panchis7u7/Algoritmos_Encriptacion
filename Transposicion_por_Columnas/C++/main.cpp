#include "Matrix.hpp"
#include "Matrix.cpp"
#include <unistd.h>

int main(int argc, char* argv[]){
    int opt;
    std::string mensaje = "Ingrese su mensaje! con la opcion -t!";
    mat::Matrix<char>* matriz;
    while((opt = getopt(argc, argv, ":t:cfd")) != -1){
        switch(opt){
            case 't':
                mensaje = optarg;
                break;
            case 'f':
                printf("Cifrado por filas.\n");
                break;
            case 'c':
                printf("cifrado por columnas.\n");
                matriz = new mat::Matrix<char>(mensaje);
                std::cout << matriz << std::endl;
                std::cout << "El mensaje encriptado es: " << std::endl;
                mat::Matrix<char>::transpuesta(*matriz);
                std::cout << matriz->getMessage() << std::endl;
                break;
            case 'd':
                printf("decifrado.\n");
                matriz = new mat::Matrix<char>(mensaje);
                std::cout << matriz << std::endl;
                std::cout << "El mensaje desencriptado es: " << std::endl;
                mat::Matrix<char>::transpuesta(*matriz);
                std::cout << matriz->getMessage() << std::endl;
                break;
            case ':':
                printf("opcion no soportada.\n");
                break;
            case '?':
                printf("opcion desconocida.\n");
                break;
        }
    }

    //Ho&epmseo&Vslaa!leedarl@asnentu@&tg&eed@caoet&a@os&laar@m?d&&&t@

    //mat::Matrix<char> prueba("Hola Como Estas? Te Saludo Desde Marte!!!!, Mi Contrasena Es 123456");
    //mat::Matrix<char> mensajeEncriptado(mensaje);
    //std::cout << mensajeEncriptado.getMessage() << std::endl;

    //mat::Matrix<char>::transpuesta(mensajeEncriptado);
    //std::cout << mensajeEncriptado << std::endl;
    return 0;
}