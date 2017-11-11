CC = gcc
CFLAGS = -pthread -Wall -O2
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
EXECUTABLE = webserver

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $^ $(CFLAGS) -o $(EXECUTABLE)


%.o: %.c
	$(CC) -o $@ -c $<

clean:
	@rm -f *.o $(EXECUTABLE)
