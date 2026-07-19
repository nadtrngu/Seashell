CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS =
SOURCES = main.c builtins.c # Add more source files as needed
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = seashell

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
