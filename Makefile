
# Define the various PATHS
SRC = ./src
BIN = ./bin
BUILD = ./build
TOKEN = ./src/token
SCRIPTS = ./scripts
INCLUDE = ./include

# Define Commands for compilation
CC  = gcc
LEX = flex

.PHONY:src

src:
	$(LEX) $(SRC)/lex.l
	$(CC) -c $(TOKEN)/alpha_token.c -I $(INCLUDE)
	$(CC) -o exe *.c *.o -I $(INCLUDE)
	mv **.o $(BUILD)
 
.PHONY:MAKEDIRS

MAKEDIRS:
	@mkdir $(BUILD) $(BIN)

.PHONY:clean

clean:
	@rm -rf $(BUILD) $(BIN)
	@MAKEDIRS