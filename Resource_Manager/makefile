TARGET=main

CC=gcc
PTHREAD=-pthread
CCFLAGS=-Wall -Wextra -Werror $(PTHREAD)
GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

#Linker.
LD=gcc
LDFLAGS= $(PTHREAD) ${GTKLIB} -export-dynamic

OBJS=main.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o

clean:
	rm -f *.o $(TARGET)