CC=g++
CPPFLAGS=-Wall -Wextra -Werror -pedantic-errors -std=c++2a

OBJS=hashVal.o SHA512.o

SOURCES:= $(shell find . -name '*.cpp')

HEADERS:= $(shell find . -name '*.h')

all: main

main: $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) -pthread -g -o $@ $^

SHA512.o: SHA512.cpp SHA512.h