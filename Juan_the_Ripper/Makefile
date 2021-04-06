CPP=g++
NASM=nasm
CPPFLAGS=-Wall -Wextra -Werror -pedantic-errors -std=c++2a -no-pie
NASMFLAGS=-f elf64 -g -F dwarf
PROG=juan

OBJS= hashMain.o SHA512.o main.o

default: $(PROG)

$(PROG): $(OBJS)
	$(CPP) $(CPPFLAGS) -g $(OBJS) -o $(PROG) -lssl -lcrypto

hashMain.o: hashMain.cpp
SHA512.o: SHA512.cpp SHA512.hpp
main.o: main.asm
	$(NASM) $(NASMFLAGS) main.asm -o main.o