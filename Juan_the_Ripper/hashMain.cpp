#include "SHA512.hpp"
#include <iostream>
#include <sstream>
#include <string.h>

//Algoritmo de SHA512, main in c++.
extern "C" void start(SHA512& sha512, char* msg, char* dictionary);

int main(int argc, char *argv[]){
	(void)argc;
	SHA512 sha512;
	char* dictionary;
	//if(std::string(argv[1]).find("--wordlist=") != std::string::npos)
	if(strstr(argv[1], "--wordlist=") != NULL){
		int len = strlen(argv[1]);
		char* copia = (char*)alloca(len);
		strncpy(copia, argv[1], len);
		char* token = strtok(copia, "=");
		token = strtok(NULL, "=");
		dictionary = (char*)malloc(sizeof(char)*(strlen(token)-2));
		strncpy(dictionary, token, (strlen(token)-2));
	}
	start(sha512, argv[2], dictionary);
	//std::stringstream ss;
	//ss << argv[1];
	//std::cout << sha512.hash(ss.str()) << std::endl;
	//std::cout << sha512.hash(std::string(argv[1])) << std::endl;
	//const char* msg = sha512.hash(argv[1]);
	//printf("%s\n", msg);
	return 0;
}
