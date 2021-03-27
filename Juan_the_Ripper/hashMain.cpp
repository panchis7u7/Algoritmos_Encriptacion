#include "SHA512.hpp"
#include <iostream>
#include <sstream>

//Algoritmo de SHA512, main in c++.
extern "C" void start(SHA512& sha512, char* msg);

int main(int argc, char *argv[]){
	(void)argc;
	SHA512 sha512;
	start(sha512, argv[1]);
	//std::stringstream ss;
	//ss << argv[1];
	//std::cout << sha512.hash(ss.str()) << std::endl;
	//std::cout << sha512.hash(std::string(argv[1])) << std::endl;
	//const char* msg = sha512.hash(argv[1]);
	//printf("%s\n", msg);
	return 0;
}
