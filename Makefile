# CC=clang
CFLAGS=-O0 -Wall -Wextra -ggdb
LIBS=

erd: main.c
	$(CC) $(CFLAGS) -o erd main.c $(LIBS)

clean:
	-rm erd
