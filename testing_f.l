%{
    /* C code to be copied into the generated .c file */
    #include    <stdio.h>
    #include    <stdlib.h>
    #include    <string.h>

    #define YY_DECL int alpha_yylex(void * ylval) /* Customize the default function name 'yylex' to 'alpha_yylex' */

    typedef struct alpha_token_t {
    unsigned int     numline;
    unsigned int     numToken;
    char          *name;
    char          *type;
    char          *category;
    struct alpha_token_t *next;
    } alpha_token;

    alpha_token * L = NULL;
    static int counter = 0;


    void L_Insert(alpha_token **L, char *name, char *type, int numline, int numToken, char *category) 
    {
        alpha_token * temp = *L; //it gives the head of the list
        
        alpha_token *newToken = (alpha_token *)malloc(sizeof(alpha_token));
        if (newToken == NULL) {
            printf("Oops! Memory allocation failed\n");
        }  
        newToken->next = NULL;
        newToken->name = strdup(name); // Allocate memory for the string and copy name
        newToken->type = strdup(type); // Allocate memory for the string and copy type
        newToken->numline = yylineno;
        newToken->numToken = numToken;
        newToken->category = strdup(category);
       
        // if list is empty
        if (temp == NULL)
        {
            *L = newToken;
        }

        //in any other case (+ the last element)
        else
        {
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newToken;
        }
        printf("Line : %d, #%d, Token: %s, Type: %s, Size: %d, Category: %s\n", newToken->numline, ++counter, newToken->name, newToken->type, newToken->numToken, newToken->category);
    }

    void L_Print(alpha_token ** L)
    {
        alpha_token * current =  *L;

        if (current == NULL)
        {
            printf("List is empty\n");
        
        }

        while (current != NULL)
        {
            printf("Token: %s, Type: %s, Line: %d, Token: %d\n", current->name, current->type, current->numline, current->numToken);
            current = current->next;
        }
        printf("\n");
    }


    const char * getCategory(char *token) {
        alpha_token *current = L;

        while (current != NULL) {
            if (strcmp(current->name, token) == 0) {
                return current->category;
            }
            current = current->next;
        }

        return "UNKNOWN";
    }



void al(const char *filename) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error! Problem with opening the file\n");
        exit(1);
    }

    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Error! Problem with creating the output file\n");
        fclose(fptr);
        exit(1);
    }

    char line[1000];
    int token_counter = 0;
    int line_number = 0;
    while (fgets(line, sizeof(line), fptr) != NULL) {
        line_number++;
        // Remove the newline character from the end of the line
        line[strcspn(line, "\n")-1] = '\0';
        
        fprintf(output_file, "<%d> : \n", line_number);

        char *token = strtok(line, " \t\n");
        while (token != NULL) {
            token_counter++;
            fprintf(output_file, "#<%d> \n\"%s\" \n%s \n", token_counter, token, getCategory(token));
            token = strtok(NULL, " \t\n");
        }
        fprintf(output_file, "\n\n");
    }

    fclose(fptr);
    fclose(output_file);
}
%}


%option    yylineno
%option    noyywrap

CONOS "conos"
IF    "if"
ELSE    "else"
WHILE    "while"
FOR    "for"
FUNCTIONS    "functions"
RETURN    "return"
BREAK    "break"
CONTINUE    "continue"
AND    "and"
NOT    "not"
OR    "or"
LOCAL    "local"
TRUE    "true" 
FALSE    "false"
NIL    "nil"

ASSIGN    "="
PLUS    "+"
MINUS    "-"
MUL    "*"
DIV    "/"
MOD    "%"
EQUAL    "=="
NOTEQUAL    "!="
PLUSPLUS    "++"
MINUSMINUS    "--"
GREATER    ">"
LESS    "<"
GREATEREQUAL    ">="
LESSEQUAL    "<="

DELIMS    [ \t\n]+
LINE_COMMENT 	"//".*

ID    [a-zA-Z][a-zA-Z0-9_]*
INTNUM    [0-9]+
REALNUM   [0-9]+"."[0-9]+

ALERT    "\a"
BACKSPACE    "\b"
FORMFEED    "\f"
NEWLINE    "\n"
CARRIAGE_RETURN    "\r"
HORIZONTAL_TAB    "\t"
VERTICAL_TAB    "\v"
BACKSLASH    "\\"
APOSTRPHE    "\'"
DOUBLE_QUOTE    "\""
QUESTION_MARK    "\?"

CURLY_BRACE_OPEN         "\{"
CURLY_BRACE_CLOSE        "\}"
SQUARE_BRACKET_OPEN        "\["
SQUARE_BRACKET_CLOSE    "\]"
PARENTHESIS_OPEN        "\("
PARENTHESIS_CLOSE        "\)"

SEMICOLON    ";"
COMMA    ","
COLON    ":"
DOUBLE_COLON    "::"
DOT    "."
DOUBLE_DOT    ".."
COMMENT_SINGLE_LINE    "//.*"
COMMENT_MULTI_LINE_OPEN    "/\*"
COMMENT_MULTI_LINE_CLOSE    "\*/"

%%
{IF}    {L_Insert(&L, yytext, "IF", yylineno, yyleng, "KEYWORD");}
{ELSE}    {L_Insert(&L, yytext, "ELSE", yylineno, yyleng,"KEYWORD");}
{WHILE}    {L_Insert(&L, yytext, "WHILE", yylineno, yyleng, "KEYWORD");}
{FOR}    {L_Insert(&L, yytext, "FOR", yylineno, yyleng, "KEYWORD");}
{FUNCTIONS}    {L_Insert(&L, yytext, "FUNCTIONS", yylineno, yyleng, "KEYWORD");}
{RETURN}    {L_Insert(&L, yytext, "RETURN", yylineno, yyleng, "KEYWORD");}
{BREAK}    {L_Insert(&L, yytext, "BREAK", yylineno, yyleng, "KEYWORD");}
{CONTINUE}    {L_Insert(&L, yytext, "CONTINUE", yylineno, yyleng, "KEYWORD");}
{AND}    {L_Insert(&L, yytext, "AND", yylineno, yyleng, "KEYWORD");}
{NOT}    {L_Insert(&L, yytext, "NOT", yylineno, yyleng, "KEYWORD");}
{OR}    {L_Insert(&L, yytext, "OR", yylineno, yyleng, "KEYWORD");}
{LOCAL}    {L_Insert(&L, yytext, "LOCAL", yylineno, yyleng, "KEYWORD");}
{TRUE}    {L_Insert(&L, yytext, "TRUE", yylineno, yyleng, "KEYWORD");}
{FALSE}    {L_Insert(&L, yytext, "FALSE", yylineno, yyleng, "KEYWORD");}
{NIL}    {L_Insert(&L, yytext, "NIL", yylineno, yyleng, "KEYWORD");}

{ASSIGN}   {L_Insert(&L, yytext, "ASSIGN", yylineno, yyleng, "OPERATOR");}
{PLUS}    {L_Insert(&L, yytext, "PLUS", yylineno, yyleng, "OPERATOR");}
{MINUS}    {L_Insert(&L, yytext, "MINUS", yylineno, yyleng, "OPERATOR");}
{MUL}    {L_Insert(&L, yytext, "MUL", yylineno, yyleng, "OPERATOR");}
{DIV}    {L_Insert(&L, yytext, "DIV", yylineno, yyleng, "OPERATOR");}
{MOD}    {L_Insert(&L, yytext, "MOD", yylineno, yyleng, "OPERATOR");}
{EQUAL}    {L_Insert(&L, yytext, "EQUAL", yylineno, yyleng, "OPERATOR");}
{NOTEQUAL}    {L_Insert(&L, yytext, "NOTEQUAL", yylineno, yyleng, "OPERATOR");}
{PLUSPLUS}    {L_Insert(&L, yytext, "PLUSPLUS", yylineno, yyleng, "OPERATOR");}
{MINUSMINUS}    {L_Insert(&L, yytext, "MINUSMINUS", yylineno, yyleng, "OPERATOR");}
{GREATER}    {L_Insert(&L, yytext, "GREATER", yylineno, yyleng, "OPERATOR");}
{LESS}    {L_Insert(&L, yytext, "LESS", yylineno, yyleng, "OPERATOR");}
{GREATEREQUAL}    {L_Insert(&L, yytext, "GREATEREQUAL", yylineno, yyleng, "OPERATOR");}
{LESSEQUAL}    {L_Insert(&L, yytext, "LESSEQUAL", yylineno, yyleng, "OPERATOR");}
{SEMICOLON}    {L_Insert(&L, yytext, "SEMICOLON", yylineno, yyleng, "DELIMITER");}

{INTNUM}    {L_Insert(&L, yytext, "INTNUM", yylineno, yyleng, "INT NUMBER");}
{REALNUM}    {L_Insert(&L, yytext, "REALNUM", yylineno, yyleng, "REAL NUMBER");}
{ID}    {L_Insert(&L, yytext, "ID", yylineno, yyleng, "IDENTIFIER");}
{DELIMS}    {L_Insert(&L, yytext, "DELIMS", yylineno, yyleng, "DELIMITER");}

%%
int main(int argc, char** argv) 
{
	if(argc > 1){
		if(!(yyin = fopen(argv[1], "r"))){
			fprintf(stderr, "Cannot read file: %s\n", argv[1]);
			return 1;
		}
	}else{
		yyin = stdin;
	}
	alpha_yylex(NULL);
	//L_Print(&L);

    al(argv[1]);
	return 0;
}