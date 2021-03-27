#include "SHA512.hpp"
#include <iostream>
#include <sstream>

//Algoritmo de SHA512, main in c++.
extern "C" void start(char* msg);

int main(int argc, char *argv[]){
	(void)argc;
	SHA512 sha512;
	start(argv[1]);
	//std::stringstream ss;
	//ss << argv[1];
	//std::cout << sha512.hash(ss.str()) << std::endl;;

	return 0;
}
