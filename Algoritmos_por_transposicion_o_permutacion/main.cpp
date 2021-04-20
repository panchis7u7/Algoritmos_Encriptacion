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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Command line options.

#define KEY             0x01    //0b00001 - 1
#define COLUMNS         0x02    //0b00010 - 2
#define ROWS            0x04    //0b00100 - 4
#define NUM_ROW_COLS    0x08    //0b01000 - 8
#define DECYPHER        0x10    //0b10000 - 16
#define ROWS_NUM_ROWS   0x0C    //0b01100 - 12
#define COLS_NUM_COLS   0x0A    //0b01010 - 10
#define NUM_ROWS_KEY    0x0D    //0b01101 - 13
#define NUM_COLS_KEY    0x0B    //0b01011 - 11

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// trim from start.
static inline std::string& ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end.
static inline std::string& rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends.
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

    std::cout << KCYN << "-------------------------------------------------------------------------------------\n" << std::endl;
    std::cout << KYEL << "\t\t\tAlgoritmos por transposicion o permutacion." << std::endl;
    std::cout << KRED << "\t\t\t    Carlos Sebastian Madrigal Rodriguez\n" << KWHT << std::endl;
    std::string mensaje = "Ingrese su mensaje! con la opcion -t!";
    std::string key = "";
    int n = 0;
    std::uint8_t options = 0;
    mat::Matrix<char>* matriz;
    std::string res;

    while((opt = getopt(argc, argv, ":t:crn:dk:")) != -1){
        switch(opt){
            case 't':
                mensaje = optarg;
                break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Row cypher with no key.

            case 'r':
                subopts = optarg; 
                options |= ROWS;
            break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Column cypher with no key.

            case 'c':
                subopts = optarg;
                options |= COLUMNS;
            break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            case 'n':
                subopts = optarg;
                options |= NUM_ROW_COLS;

                getsubopt(&subopts, decrypt_opts, &value);
                if(*subopts != '\0'){
                    n = (int)*value;
                }
            break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Key cypher.

            case 'k':
                subopts = optarg;
                options |= KEY;

                getsubopt(&subopts, decrypt_opts, &value);
                if(*subopts != '\0'){
                    key = value;
                }
            break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Decypher.

            case 'd':
                subopts = optarg;
                options |= DECYPHER;
            break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            case ':':
                std::cout << KRED << "Opcion no soportada." << std::endl;
                break;
            case '?':
                std::cout << KRED << "Opcion Desconocida." << std::endl;
                break;
        }
    }

    //Check if the user inserted at least the minimum options.
    if(options == 0){
        std::cout << KRED << "Ingrese al menos una opcion, utiliza --help!" << std::endl;
        goto exit;
    }   

    //Check user options.

    //Column transposition.
    if(options & COLUMNS){
        std::cout << "Columns" << std::endl;
        if(options & DECYPHER){
            std::cout << "Cols Num Cols Key Decipher" << std::endl;
            if(options == (NUM_COLS_KEY | DECYPHER)){
                std::cout << "Cols Num Cols Key" << std::endl;
            } else if(options == (COLS_NUM_COLS | DECYPHER)){
                std::cout << "Cols Num Cols" << std::endl;
            } else if(options == (KEY | DECYPHER)){
                std::cout << "Cols Key" << std::endl;
            } else {
                std::cout << "Cols" << std::endl;
            }
        } else {
            std::cout << "Cols Num Cols Key Cipher" << std::endl;
            if(options == NUM_COLS_KEY){
                std::cout << "Cols Num Cols Key" << std::endl;
            } else if(options == COLS_NUM_COLS){
                std::cout << "Cols Num Cols" << std::endl;
            } else if(options == KEY){
                std::cout << "Cols Key" << std::endl;
            } else {
                std::cout << "Cols" << std::endl;
            }
        }
    }

    //Row transposition.
    if(options & ROWS){
        std::cout << "Columns" << std::endl;
        if(options & DECYPHER){
            std::cout << "Cols Num Cols Key Decipher" << std::endl;
            if(options == (NUM_ROWS_KEY | DECYPHER)){
                std::cout << "Cols Num Cols Key" << std::endl;
            } else if(options == (ROWS_NUM_ROWS | DECYPHER)){
                std::cout << "Cols Num Cols" << std::endl;
            } else if(options == (KEY | DECYPHER)){
                std::cout << "Cols Key" << std::endl;
            } else {
                std::cout << "Cols" << std::endl;
            }
        } else {
            std::cout << "Cols Num Cols Key Cipher" << std::endl;
            if(options == NUM_ROWS_KEY){
                std::cout << "Cols Num Cols Key" << std::endl;
            } else if(options == ROWS_NUM_ROWS){
                std::cout << "Cols Num Cols" << std::endl;
            } else if(options == KEY){
                std::cout << "Cols Key" << std::endl;
            } else {
                std::cout << "Cols" << std::endl;
            }
        }
    }
    
    

    exit:
    std::cout << KCYN << "\n-------------------------------------------------------------------------------------" << KCYN << std::endl;
    //Ho&epmseo&Vslaa!leedarl@asnentu@&tg&eed@caoet&a@os&laar@m?d&&&t@


    // ./matrix -t "Hola como estas"? -c 4 --nokey -> Matrix transposition by columns (4).
    // ./matrix -t "Hola como estas"? -r 4 --nokey -> Matrix transposition by rows (4).
    // ./matrix -t "Hola como estas"? -c 4 -k "Gatito" -> Matrix transposition by columns (4) with key.
    // ./matrix -t "Hola como estas"? -r 4 -k "Gatito" -> Matrix transposition by rows (4) with key.

    return 0;
}