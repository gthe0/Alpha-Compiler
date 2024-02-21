# Define directory paths
SRC         := ./src
BIN         := ./bin
BUILD_DIR   := ./build
SCRIPTS     := ./scripts
INCLUDE     := ./include

# Collect all C source files in the directory
SOURCE_FILES := $(wildcard $(SRC)/*.c)
TMP_SOURCE_FILES := $(SOURCE_FILES:$(SRC)/%=%) # Remove the src/ prefix
OBJ_FILES := $(addprefix $(BUILD_DIR)/,$(TMP_SOURCE_FILES:.c=.o)) # Append build/ and .o


###########################################################################################
#	LEXICAL GENERATOR
###########################################################################################

# Lexical analysis target
lex: src/lex.l
	@flex src/lex.l
	@mv *.c ${SRC}

# Compile object files
obj: lex $(OBJ_FILES)

# Compile the scanner
compile: lex $(BIN)
	gcc -I $(INCLUDE) $(SOURCE_FILES) -o $(BIN)/scanner.exe

# Run the scanner
run: compile
	$(BIN)/scanner.exe

# Compile each source file into its object file individually
$(BUILD_DIR)/%.o: $(SRC)/%.c | $(BUILD_DIR)
	gcc -I $(INCLUDE) -c $< -o $@

###########################################################################################
#	SCRIPTS
###########################################################################################

RULEGEN: scripts/RuleGen.py scripts/misc/list
	python3 scripts/RuleGen.py < scripts/misc/list_path > output.txt


###########################################################################################
#	UTILITIES
###########################################################################################

# Clean up build artifacts
clean:
	@rm -rf $(BUILD_DIR) $(BIN) $(SRC)/lex.yy.c output.txt

# Ensure that the build and bin directories exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN):
	mkdir -p $(BIN)
