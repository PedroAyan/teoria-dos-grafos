CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = main
ifeq ($(OS),Windows_NT)
TARGET = main.exe
endif

all: $(TARGET)

$(TARGET): main.c bellman_ford.c bellman_ford.h
	$(CC) $(CFLAGS) main.c bellman_ford.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: run

clean:
	$(RM) $(TARGET) *.o

.PHONY: all run test clean
