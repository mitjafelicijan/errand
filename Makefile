CFLAGS = $(EXTRA_FLAGS) -O3 -Wall -Wextra -std=gnu99 -pedantic
LIBS =

TARGET = erd
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET) callgrind.out.*

valgrind:
	valgrind -s --leak-check=full ./erd
