#include "SHA512.h"
#include <iostream>
#include <sstream>

//Algoritmo de SHA512, main in c++.

int main(int argc, char *argv[]){
	(void)argc;
	SHA512 sha512;
	std::stringstream ss;
	ss << argv[1];
	std::cout << sha512.hash(ss.str()) << std::endl;;

	return 0;
}
