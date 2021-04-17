CPP=g++
CPPFLAGS=-pedantic-errors -std=c++2a
PROG=matrix

OBJS= Matrix.o main.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(PROG)

Matrix.o: Matrix.cpp Matrix.hpp
main.o: main.cpp

clean:
	rm Matrix.o
	rm main.o
	rm matrix