CFLAGS=-O3 -Wall -Wextra -std=gnu99 -pedantic
LIBS=

erd: main.c
	$(CC) $(CFLAGS) -o erd main.c $(LIBS)

clean:
	-rm erd
