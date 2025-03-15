CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
TARGET = Ultimate_TicTacToe
SRC = main.c ai.c game.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
