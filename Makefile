# Define directory paths
SRC         := ./src
BIN         := ./bin
BUILD_DIR   := ./build
SCRIPTS     := ./scripts
INCLUDE     := ./include
GENERATOR   := ./generator

# Collect all C source files in the directory
SOURCE_FILES = $(wildcard $(SRC)/*.c)
TMP_SOURCE_FILES := $(SOURCE_FILES:$(SRC)/%=%) # Remove the src/ prefix
OBJ_FILES := $(addprefix $(BUILD_DIR)/,$(TMP_SOURCE_FILES:.c=.o)) # Append build/ and .o

###########################################################################################
#	LEXICAL GENERATOR
###########################################################################################

all: clean generate compile

# Compile object files
object: $(OBJ_FILES)

# Compile the parser
compile: $(INCLUDE) $(SRC)/scanner.c $(SRC)/parser.c $(BIN)
	gcc -g -I $(INCLUDE) src/**.c -o $(BIN)/parser.out
	@rm parser.output
	
# Generate the scanner and parser
generate: flex bison

# Lexical analysis target
flex: $(GENERATOR)/lex.l
	flex --outfile=scanner.c $(GENERATOR)/lex.l
	mv scanner.c $(SRC)/scanner.c

# Syntax analysis target
bison: $(GENERATOR)/parser.y
	bison -v -t --defines -Wconflicts-sr --debug --output=parser.c $(GENERATOR)/parser.y
	mv parser.h $(INCLUDE)/parser.h
	mv parser.c $(SRC)/parser.c

# Compile each source file into its object file individually
$(BUILD_DIR)/%.o: $(SRC)/%.c | $(BUILD_DIR)
	gcc -I $(INCLUDE) -c $< -o $@

###########################################################################################
#	SCRIPTS
###########################################################################################

FLEX_RULES: $(SCRIPTS)/RuleGen.py scripts/misc/list
	python3 $(SCRIPTS)/RuleGen.py < $(SCRIPTS)/misc/list_path > output.txt

test2: $(SCRIPTS)/phase2_test.sh
	$(SCRIPTS)/phase2_test.sh

test3: $(SCRIPTS)/phase3_test.sh
	$(SCRIPTS)/phase3_test.sh

test: test2 test3

##########################################################################################
#	UTILITIES
###########################################################################################

# Clean up build artifacts
clean:
	@rm -rf $(BUILD_DIR) $(BIN) $(SRC)/scanner.c $(INCLUDE)/scanner.h output.txt *.abc\
		$(SRC)/parser.c $(INCLUDE)/parser.h parser.output .vscode quads.txt tcg_instructions.txt test_results

# Ensure that the build and bin directories exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN):
	mkdir -p $(BIN)
	
