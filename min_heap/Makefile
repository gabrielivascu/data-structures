APP = main
OBJ = main.o min-heap.o

CC = gcc
CFLAGS = -g -Wall -Wextra -Wno-unused
LDFLAGS =

build: $(APP)

$(APP): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ) $(APP)
