TARGET=main

CC=gcc
PTHREAD=-pthread
CCFLAGS=-Wall -Wextra -Werror $(PTHREAD)
GTKFLAGS=`pkg-config --cflags gtk4`
GTKLIB=`pkg-config --libs gtk4`

#Linker.
LD=gcc
LDFLAGS= $(PTHREAD) ${GTKFLAGS} -export-dynamic

OBJS=main.o

all: $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS) ${GTKLIB}

main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKFLAGS) -o main.o ${GTKLIB}

clean:
	rm -f *.o $(TARGET)