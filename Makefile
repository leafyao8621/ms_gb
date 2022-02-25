CC = /home/leaf/gbdk/bin/lcc
SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)
ASM = $(SRC:.c=.asm)
LST = $(SRC:.c=.lst)
SYM = $(SRC:.c=.sym)
OBJ = $(SRC:.c=.o)
IHX = ms.ihx
BIN = ms.gb

%.o: %.c
	$(CC) -c $< -o $@ -I/home/leaf/gbdk/include

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

.PHONY: clean run
clean:
	@rm $(ASM)
	@rm $(LST)
	@rm $(SYM)
	@rm $(OBJ)
	@rm $(IHX)
	@rm $(BIN)

run: $(BIN)
	@vba $(BIN)
