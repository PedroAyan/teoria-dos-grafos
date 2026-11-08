CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2
TARGET = main
ifeq ($(OS),Windows_NT)
TARGET = main.exe
endif

all: $(TARGET)

$(TARGET): main.c hamiltoniano.c hamiltoniano.h tps.c tps.h
	$(CC) $(CFLAGS) main.c hamiltoniano.c tps.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: run

clean:
	$(RM) $(TARGET) *.o

.PHONY: all run test clean
