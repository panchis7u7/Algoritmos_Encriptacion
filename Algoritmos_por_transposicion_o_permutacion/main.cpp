#include "Matrix.hpp"
#include "Matrix.cpp"
#include <unistd.h>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

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

    std::cout << "-------------------------------------------------------------------------------------" << KCYN << std::endl;
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
                        mat::Matrix<char>::transpose(*matriz);
                        std::cout << matriz << std::endl;
                        std::cout << "El mensaje encriptado es: " << std::endl;
                        std::cout << matriz->getMessage() << std::endl;
                        break;
                    default:
                        std::cout << "Cifrado por filas usando como llave a: " << value << "." << std::endl;
                        matriz = new mat::Matrix<char>(mensaje, value, mat::opcodes::crypt);
                        mat::Matrix<char>::transpose(*matriz);
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
                        std::cout << matriz << std::endl;
                        mat::Matrix<char>::transpose(*matriz);
                        std::cout << "El mensaje encriptado es: " << std::endl;
                        std::cout << matriz->getMessage() << std::endl;
                        break;
                    default:
                        std::cout << "Cifrado por columnas usando como llave a: " << value << "." << std::endl;
                        matriz = new mat::Matrix<char>(mensaje, value, mat::opcodes::crypt);
                        std::cout << matriz << std::endl;
                        mat::Matrix<char>::transpose(*matriz);
                        std::cout << "El mensaje encriptado es: " << std::endl;
                        std::cout << matriz->getMessage() << std::endl;
                        break;
                    }
                }
                break;
            case 'd':
                subopts = optarg;
                while (*subopts != '\0')
                {
                    switch (getsubopt(&subopts, decrypt_opts, &value))
                    {
                    case NO_KEY_OPTION:
                        printf("Descifrado de mensaje:\n");
                        matriz = new mat::Matrix<char>(mensaje);
                        std::cout << matriz << std::endl;
                        std::cout << "El mensaje desencriptado es: " << std::endl;
                        mat::Matrix<char>::transpose(*matriz);
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
                        std::cout << "Descifrado usando como llave a: " << value << "." << std::endl;
                        matriz = new mat::Matrix<char>(mensaje, value, mat::opcodes::decrypt);
                        std::cout << matriz << std::endl;
                        std::cout << "El mensaje desencriptado es: " << std::endl;
                        res = matriz->getMessage(value);
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

    std::cout << "-------------------------------------------------------------------------------------" << KCYN << std::endl;
    //Ho&epmseo&Vslaa!leedarl@asnentu@&tg&eed@caoet&a@os&laar@m?d&&&t@
    return 0;
}