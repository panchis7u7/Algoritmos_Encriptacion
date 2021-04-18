#include "Matrix.hpp"
#include "Matrix.cpp"
#include <unistd.h>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start
static inline std::string& ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string& rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string& trim(std::string& s) {
    return ltrim(rtrim(s));
}

enum {
  NO_KEY_OPTION = 0
};

char noKeyOption[] = "--nokey";

char* decrypt_opts[] = {
  noKeyOption
};

int main(int argc, char* argv[]){
    char* subopts, *value;
    int opt;
    std::string mensaje = "Ingrese su mensaje! con la opcion -t!";
    std::string key = "";
    mat::Matrix<char>* matriz;
    std::string res;
    while((opt = getopt(argc, argv, ":t:c:f:d:")) != -1){
        switch(opt){
            case 't':
                mensaje = optarg;
                break;
            case 'f':
                subopts = optarg;
                while (*subopts != '\0')
                {
                    switch (getsubopt(&subopts, decrypt_opts, &value))
                    {
                    case NO_KEY_OPTION:
                        printf("Cifrado por filas.\n");
                        matriz = new mat::Matrix<char>(mensaje);
                        mat::Matrix<char>::transpuesta(*matriz);
                        std::cout << matriz << std::endl;
                        std::cout << "El mensaje encriptado es: " << std::endl;
                        std::cout << matriz->getMessage() << std::endl;
                        break;
                    default:
                        std::cout << "Cifrado por filas usando como llave a: " << value << "." << std::endl;
                        matriz = new mat::Matrix<char>(mensaje, value);
                        mat::Matrix<char>::transpuesta(*matriz);
                        std::cout << matriz << std::endl;
                        std::cout << "El mensaje encriptado es: " << std::endl;
                        std::cout << matriz->getMessage() << std::endl;
                        break;
                    }
                }
                break;
            case 'c':
                subopts = optarg;
                while (*subopts != '\0')
                {
                    switch (getsubopt(&subopts, decrypt_opts, &value))
                    {
                    case NO_KEY_OPTION:
                        printf("Cifrado por columnas.\n");
                        matriz = new mat::Matrix<char>(mensaje);
                        mat::Matrix<char>::transpuesta(*matriz);
                        std::cout << matriz << std::endl;
                        std::cout << "El mensaje encriptado es: " << std::endl;
                        std::cout << matriz->getMessage() << std::endl;
                        break;
                    default:
                        std::cout << "Cifrado por columnas usando como llave a: " << value << "." << std::endl;
                        matriz = new mat::Matrix<char>(mensaje, value);
                        std::cout << matriz << std::endl;
                        std::cout << "El mensaje encriptado es: " << std::endl;
                        mat::Matrix<char>::transpuesta(*matriz);
                        std::cout << matriz->getMessage() << std::endl;
                        break;
                    }
                }
                break;
            case 'd':/*
                subopts = optarg;
                while (*subopts != '\0')
                {
                    switch (getsubopt(&subopts, decrypt_opts, &value))
                    {
                    case COL_OPTION:
                        printf("Descifrado por columnas:\n");
                        matriz = new mat::Matrix<char>(mensaje);
                        std::cout << matriz << std::endl;
                        std::cout << "El mensaje desencriptado es: " << std::endl;
                        mat::Matrix<char>::transpuesta(*matriz);
                        res = matriz->getMessage();
                        for (size_t i = 0; i < res.length(); i++)
                        {
                            res[i] = ((!(res[i] == '&')* !(res[i] == '@') *(int)res[i]))
				        	+ ((res[i] == '&')*32)
                            + ((res[i] == '@')*32);
                        }
                        trim(res);
                        std::cout << res << std::endl;
                        break;
                    
                    case ROW_OPTION:
                        printf("Descifrado por filas:\n");
                        matriz = new mat::Matrix<char>(mensaje);
                        std::cout << matriz << std::endl;
                        std::cout << "El mensaje desencriptado es: " << std::endl;
                        mat::Matrix<char>::transpuesta(*matriz);
                        res = matriz->getMessage();
                        for (size_t i = 0; i < res.length(); i++)
                        {
                            res[i] = ((!(res[i] == '&')* !(res[i] == '@') *(int)res[i]))
				        	+ ((res[i] == '&')*32)
                            + ((res[i] == '@')*32);
                        }
                        trim(res);
                        std::cout << res << std::endl;
                        break;

                    default:
                        std::cout << "Ingrese una opcion valida. ya sea --col o --row." << std::endl;
                        break;
                    }
                }*/
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
    return 0;
}