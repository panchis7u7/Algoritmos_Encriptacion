#include "Matrix.hpp"
#include "Matrix.cpp"
#include <unistd.h>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string>

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes.

mat::Matrix<char>* cypher(std::string message, std::string key, unsigned n, mat::strategy strategy, mat::type type);
mat::Matrix<char>* decypher(std::string message, std::string key, unsigned n, mat::strategy strategy, mat::type type);

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

int main(int argc, char* argv[]){
    int opt;

    std::cout << KCYN << "-------------------------------------------------------------------------------------\n" << std::endl;
    std::cout << KYEL << "\t\t\tAlgoritmos por transposicion o permutacion." << std::endl;
    std::cout << KRED << "\t\t\t    Carlos Sebastian Madrigal Rodriguez\n" << KWHT << std::endl;
    std::string message = "Ingrese su mensaje! con la opcion -t!";
    std::string key = "";
    int n = 0;
    std::uint8_t options = 0;
    mat::Matrix<char>* result;
    std::string res;

    while((opt = getopt(argc, argv, ":t:crn:dk:")) != -1){
        switch(opt){
            case 't':
                message = optarg;
                break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Row cypher with no key.

            case 'r':
                options |= ROWS;
            break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Column cypher with no key.

            case 'c':
                options |= COLUMNS;
            break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            case 'n':
                n = std::stoi(optarg);
                options |= NUM_ROW_COLS;
            break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Key cypher.

            case 'k':
                key = optarg;
                options |= KEY;
            break;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Decypher.

            case 'd':
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
        std::cout << "Transposicion por Columnas." << std::endl;
        if(options & DECYPHER){
            //std::cout << "Cols Num Cols Key Decipher" << std::endl;
            if(options == (NUM_COLS_KEY | DECYPHER)){
                //std::cout << "Cols Num Cols Key" << std::endl;
            } else if(options == (COLS_NUM_COLS | DECYPHER)){
                //std::cout << "Cols Num Cols" << std::endl;
            } else if(options == (KEY | DECYPHER)){
                //std::cout << "Cols Key" << std::endl;
            } else {
                //std::cout << "Cols" << std::endl;
            }
        } else {
            //std::cout << "Cols Num Cols Key Cipher" << std::endl;
            if(options == COLS_NUM_COLS){
                //std::cout << "Cols Num Cols" << std::endl;
                result = cypher(message, key, n, mat::strategy::coltrans, mat::type::nNormal);
                std::cout << result << std::endl;
            } else if(options == (KEY | COLUMNS)){
                //std::cout << "Cols Key" << std::endl;
                result = cypher(message, key, n, mat::strategy::coltrans, mat::type::key);
                std::cout << result << std::endl;
            } else {
                //std::cout << "Cols" << std::endl;
                result = cypher(message, key, n, mat::strategy::coltrans, mat::type::normal);
                std::cout << result << std::endl;
            }
        }
    }

    //Row transposition.
    if(options & ROWS){
        std::cout << "Rows" << std::endl;
        if(options & DECYPHER){
            std::cout << "Rows Num Rows Key Decipher" << std::endl;
            if(options == (ROWS_NUM_ROWS | DECYPHER)){
                //std::cout << "Cols Num Cols" << std::endl;
                result = cypher(message, key, n, mat::strategy::coltrans, mat::type::nNormal);
                std::cout << result << std::endl;
            } else if(options == (KEY | DECYPHER)){
                //std::cout << "Cols Key" << std::endl;
                result = cypher(message, key, n, mat::strategy::coltrans, mat::type::key);
                std::cout << result << std::endl;
            } else {
                //std::cout << "Cols" << std::endl;
                result = cypher(message, key, n, mat::strategy::coltrans, mat::type::normal);
                std::cout << result << std::endl;
            }
        } else {
            std::cout << "Cols Num Cols Key Cipher" << std::endl;
            if(options == ROWS_NUM_ROWS){
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

mat::Matrix<char>* cypher(std::string message, std::string key, unsigned n, mat::strategy strategy, mat::type type){
    mat::Matrix<char>* cypher;
    switch (strategy) {

    case mat::strategy::coltrans:
        switch (type) {
        case mat::type::normal:
            cypher = new mat::Matrix<char>(message);
            break;
        case mat::type::nNormal:
            cypher = new mat::Matrix<char>(message, n);
            break;
        case mat::type::key:
            cypher = new mat::Matrix<char>(message, key);
            break;
        default:
            std::cout << KRED << "Error con los parametros!" << std::endl;
            break;
        }
        break;

    case mat::strategy::rowtrans:

        break;
    default:
        break;
    }

    return cypher;
}

mat::Matrix<char>* decypher(std::string message, std::string key, unsigned n, mat::strategy strategy, mat::type type){
    mat::Matrix<char>* decypher;
    return decypher;
}