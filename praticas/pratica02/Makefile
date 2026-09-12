CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = main
ifeq ($(OS),Windows_NT)
TARGET = main.exe
endif

SRC = busca_largura.c busca_profundidade.c main.c

all: $(TARGET)

$(TARGET): $(SRC) busca_largura.h busca_profundidade.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET) *.o

.PHONY: all run clean
