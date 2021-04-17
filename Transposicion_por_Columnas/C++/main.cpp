#include "Matrix.hpp"
#include "Matrix.cpp"
#include <unistd.h>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

enum {
  COL_OPTION = 0,
  ROW_OPTION = 1
};

char colOption[] = "--col";
char rowOption[] = "--row";

char* decrypt_opts[] = {
  colOption,
  rowOption
};

int main(int argc, char* argv[]){
    char* subopts, *value;
    int opt;
    std::string mensaje = "Ingrese su mensaje! con la opcion -t!";
    mat::Matrix<char>* matriz;
    std::string res;
    while((opt = getopt(argc, argv, ":t:cfd:")) != -1){
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
                subopts = optarg;
                while (*subopts != '\0')
                {
                    switch (getsubopt(&subopts, decrypt_opts, &value))
                    {
                    case COL_OPTION:
                        printf("decifrado.\n");
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
                        printf("decifrado.\n");
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
                    }
                }
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