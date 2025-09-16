# Compilador
CC = gcc

# Flags del compilador
CFLAGS = -I./include -std=c17 -Og

# Directorios
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

# Archivos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/my_program

# Reglas
all: $(OBJ_DIR) $(BIN_DIR) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET) 

.PHONY: all clean
