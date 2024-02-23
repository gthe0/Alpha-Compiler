# Define directory paths
SRC         := ./src
BIN         := ./bin
BUILD_DIR   := ./build
SCRIPTS     := ./scripts
INCLUDE     := ./include
GENERATOR	:= ./generator

# Collect all C source files in the directory
SOURCE_FILES := $(wildcard $(SRC)/*.c)
TMP_SOURCE_FILES := $(SOURCE_FILES:$(SRC)/%=%) # Remove the src/ prefix
OBJ_FILES := $(addprefix $(BUILD_DIR)/,$(TMP_SOURCE_FILES:.c=.o)) # Append build/ and .o

###########################################################################################
#	LEXICAL GENERATOR
###########################################################################################

# Compile object files
object: ${SRC}/scanner.c $(OBJ_FILES)

# Compile the scanner
compile: ${SRC}/scanner.c $(BIN)
	gcc -g -I $(INCLUDE) $(SOURCE_FILES) -o $(BIN)/scanner.exe

# Lexical analysis target
 ${SRC}/scanner.c: $(GENERATOR)/lex.l
	flex $(GENERATOR)/lex.l
	mv scanner.c $@
	$(eval SOURCE_FILES +=  $@)


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
	@rm -rf $(BUILD_DIR) $(BIN) $(SRC)/scanner.c output.txt

# Ensure that the build and bin directories exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN):
	mkdir -p $(BIN)
	