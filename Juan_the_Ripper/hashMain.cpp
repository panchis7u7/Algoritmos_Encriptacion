#include "SHA512.hpp"
#include <iostream>
#include <sstream>
#include <string.h>

//Algoritmo de SHA512, main in c++.
extern "C" void start(SHA512& sha512, char* msg, char* dictionary);

int getSize(char* msg){
	return strlen(msg);
} 

int main(int argc, char *argv[]){
	(void)argc;
	SHA512 sha512;
	char* dictionary;
	char* ptr = strstr(argv[1], "--wordlist=");
	if(ptr != NULL){
		int len = strlen(ptr+11);
		dictionary = (char*)malloc(sizeof(char)*len);
		strncpy(dictionary, ptr+11, len);
	}
	start(sha512, argv[2], dictionary);
	return 0;
}
