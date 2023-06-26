CFLAGS=-O3 -Wall -Wextra
LIBS=

erd: main.c
	$(CC) $(CFLAGS) -o erd main.c $(LIBS)

clean:
	-rm erd
