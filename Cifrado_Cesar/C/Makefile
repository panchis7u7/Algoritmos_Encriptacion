CC=gcc
CFLAGS=-Wall -Wextra -Werror

OBJS=cesar.o

all: cesar

cesar: $(OBJS)
	gcc -o $@ $^

clean:
	rm -f $(OBJS)