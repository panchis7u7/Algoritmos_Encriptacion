CPP=g++
CPPFLAGS=-Wall -Wextra -Werror -pedantic-errors -std=c++2a -no-pie
PROG=mat

OBJS= Matrix.o main.o

default: $(PROG)

$(PROG): $(OBJS)
	$(CPP) $(CPPFLAGS) -g $(OBJS) -o $(PROG)

main.o: main.cpp
Matrix.o: Matrix.cpp Matrix.hpp