CFLAGS=
LIBS=

erd: main.c
	$(CC) $(CFLAGS) -o erd main.c $(LIBS)

clean:
	rm erd
