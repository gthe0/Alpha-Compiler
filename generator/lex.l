/* 
*  Authors: csd4881, csd4988, csd5038
*			
* 	lex.l
*
*	This is a file with rules to generate
*	a lexical analyzer's source code in c
*/

/* DEFINITION SECTION
*
* Anything written inside the brackets will be copied 
* at the top of the generated .c file
*/

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	
	#include <alpha_token.h>
	#include <al.h>

	#if defined(WIN32) || defined(_WIN32_WCE)
	#define YY_NO_UNISTD_H
	static int isatty(int i) {return 0;}
	#endif

	/* Customize the default function name 'yylex' to 'alpha_yylex' */
	#define YY_DECL int alpha_yylex(void * ylval)
	#define SCANNER_EOF YY_END_OF_BUFFER_CHAR
	#define SZ 0x40

	/* Enums for error levels*/
	enum ERROR_TYPE {
		ERROR,
		WARNING,
	};

	typedef enum ERROR_TYPE ERROR_TYPE;

	/* Function definitions*/
	static int multi_line_comment(AlphaToken_T ylval, int );
	static int valid_string(char **s);

	/* Function to print errors/warnings */
	static void LOG_ERROR(ERROR_TYPE error,const char* msg, ...);

%}

%option noyywrap
%option yylineno	
%option	outfile = "scanner.c" 

/* Flex macros
*
*   Here are defined the classes/terminal symbols,
*   the various grammars and the Rules that the lexical analyzer
*   will follow
*/

IF    			"if"
ELSE    		"else"
WHILE   		"while"
FOR     		"for"
FUNC    		"function"
RETURN  		"return"
BREAK   		"break"
CONTINUE    	"continue"
AND    			"and"
NOT    			"not"
OR    			"or"
LOCAL    		"local"
TRUE    		"true" 
FALSE   		"false"
NIL    			"nil"
BRACKETS_OPEN	"["					 
BRACKETS_CLOSE  "]"
BRACES_OPEN 	"{"					
BRACES_CLOSE	"}"
PARENT_OPEN 	"("
PARENT_CLOSE    ")"
COMMA			","
SEMI_COL 		";"
COL 			":"
DOUBLE_COL 		"::"
DOT				"."
DOUBLE_DOT 		".."
ASSIGN_OP 		"="
PLUS_OP 		"+"
MINUS_OP 		"-"
MULT_OP 		"*"
DIV_OP 			"/"
MOD_OP 			"%"
EQ_OP 			"=="
NE_OP 			"!="
INC_OP			"++"
DEC_OP 			"--"
GT_OP 			">"
LT_OP 			"<"
GE_OP			">="
LE_OP			"<="

digits  		[0-9]
delims          [ \t\n\r\v]
letters 		[A-Za-z]

WS	    		{delims}+
ID	    		{letters}({letters}|{digits}|[_])*
INT	    		{digits}+
FLOAT   		{digits}+"."{digits}+

STRING			\"
LINE_COMMENT	"//".*
BLOCK_COMMENT	"/*"


/* RULES SECTION
*   
*   The Rules section contains a series of rules in the form:
*	- pattern action
*	- pattern 
*/

%%
{IF}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "IF", "enumarated", "KEYWORD");}
{ELSE}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "ELSE", "enumarated", "KEYWORD");}
{WHILE}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "WHILE", "enumarated", "KEYWORD");}
{FOR}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "FOR", "enumarated", "KEYWORD");}
{FUNC}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "FUNC", "enumarated", "KEYWORD");}
{RETURN}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "RET", "enumarated", "KEYWORD");}
{BREAK}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "BREAK", "enumarated", "KEYWORD");}
{CONTINUE}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "CONTINUE", "enumarated", "KEYWORD");}
{AND}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "AND", "enumarated", "KEYWORD");}
{NOT}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "NOT", "enumarated", "KEYWORD");}
{OR}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "OR", "enumarated", "KEYWORD");}
{LOCAL}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "LOCAL", "enumarated", "KEYWORD");}
{TRUE}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "TRUE", "enumarated", "KEYWORD");}
{FALSE}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "FALSE", "enumarated", "KEYWORD");}
{NIL}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "NIL", "enumarated", "KEYWORD");}

{BRACKETS_OPEN}		{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "BRACKETS_OPEN", "enumarated", "PUNCT");}
{BRACKETS_CLOSE}	{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "BRACKETS_CLOSE", "enumarated", "PUNCT");}
{BRACES_OPEN}		{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "BRACES_OPEN", "enumarated", "PUNCT");}
{BRACES_CLOSE}		{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "BRACES_CLOSE", "enumarated", "PUNCT");}
{PARENT_OPEN}		{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "PARENT_OPEN", "enumarated", "PUNCT");}
{PARENT_CLOSE}		{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "PARENT_CLOSE", "enumarated", "PUNCT");}
{COMMA}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "COMMA", "enumarated", "PUNCT");}
{SEMI_COL}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "SEMI_COL", "enumarated", "PUNCT");}
{COL}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "COL", "enumarated", "PUNCT");}
{DOUBLE_COL}		{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "DOUBLE_COL", "enumarated", "PUNCT");}
{DOT}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "DOT", "enumarated", "PUNCT");}
{DOUBLE_DOT}		{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "DOUBLE_DOT", "enumarated", "PUNCT");}

{ASSIGN_OP}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "ASSIGN_OP", "enumarated", "OPERATOR");}
{PLUS_OP}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "PLUS_OP", "enumarated", "OPERATOR");}
{MINUS_OP}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "MINUS_OP", "enumarated", "OPERATOR");}
{MULT_OP}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "MULT_OP", "enumarated", "OPERATOR");}
{DIV_OP}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "DIV_OP", "enumarated", "OPERATOR");}
{MOD_OP}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "MOD_OP", "enumarated", "OPERATOR");}
{EQ_OP}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "EQ_OP", "enumarated", "OPERATOR");}
{NE_OP}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "NE_OP", "enumarated", "OPERATOR");}
{INC_OP}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "INC_OP", "enumarated", "OPERATOR");}
{DEC_OP}			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "DEC_OP", "enumarated", "OPERATOR");}
{GT_OP}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "GT_OP", "enumarated", "OPERATOR");}
{LT_OP}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "LT_OP", "enumarated", "OPERATOR");}
{GE_OP}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "GE_OP", "enumarated", "OPERATOR");}
{LE_OP}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "LE_OP", "enumarated", "OPERATOR");}

{INT}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "INT", "integer", "INTCONST");}
{FLOAT}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "FLOAT", "double", "REALCONST");}
{ID}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext, "ID", "char *", "IDENTIFIER");}

{LINE_COMMENT}  		{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yylineno, yytext,"LINE_COMMENT","enumarated", "COMMENT");}

{STRING}				{
							int start = yylineno;
							char* s = NULL;
							if(valid_string(&s)== EXIT_FAILURE)
							{
								free(s);
								LOG_ERROR(ERROR,"Unterminated STRING in line %d\n",start);
								return EXIT_FAILURE;
							}
							AlphaToken_insert((AlphaToken_T) ylval, start, yylineno, s, "STRING", "char *", "STRING");
							free(s);
						}

{BLOCK_COMMENT}  	{
							int start = yylineno;
							if(multi_line_comment(ylval,0) == EXIT_FAILURE)
							{
								LOG_ERROR(ERROR,"Unterminated COMMENT in line %d\n",start);
								return EXIT_FAILURE;
							}
						}


{WS}				{/*Ignore White Spaces*/}
.					{LOG_ERROR(WARNING,"Illegal character %s in line %d\n",yytext,yylineno);}

%%

/* CODE SECTION
*	Here we write the code/functions that will be 
*	copied in the bottom of the generated file
*/

/*
*	A function to print the errors in stderr
*/
static void LOG_ERROR(ERROR_TYPE error_t ,const char* msg,...)
{
	/* If we are on linux machines, use ansi escape sequences */
	#if ( __linux__)
		char* warning = "\e[1;35mWARNING: \e[0m";
		char* error  = "\e[1;31mERROR: \e[0m";
	#else
		char* warning = "WARNING: ";
		char* error	  = "ERROR: ";
	#endif
	
	fflush(stdout);

	if(error_t == WARNING)
		fprintf(stderr,"%s",warning);
	else if(error_t == ERROR)
		fprintf(stderr,"%s",error);

	/* Initialize arguments */
	va_list args;
    va_start(args, msg);

	vfprintf(stderr,msg,args);

	/* Free them */
	va_end(args);

	return ;
}

/*
* A Function to check if the strings are correct.
* If there are any invalid escape sequences it prints some warnings.
*
*	Returns:	--EXIT_FAILURE (1) if the string does not end
*				--EXIT_SUCCESS (0) if the string ends properly
*/
static int valid_string(char **s)
{
	char *str; 
	int c = 0, i = 0,size = SZ*sizeof(char);
	
	/* Allocate memory for string*/
	str = malloc(size);
	
	while((c = input()) > SCANNER_EOF )
	{
		if (c == '\"') 
		{
            str[i] = '\0';
            break;
        } 
		
		if (c == '\\') 
		{
            c = input();
            switch (c) 
			{
                case 't': str[i] = '\t'; break;
                case 'n': str[i] = '\n'; break;
                case '\"': str[i] = '\"'; break;
                case '\'': str[i] = '\''; break;
                case '\\': str[i] = '\\'; break;
                default:
                    str[i] = '\\';
                    LOG_ERROR(WARNING, "Invalid escape sequence \\%c in line %d\n", c, yylineno);
                    unput(c);
                    break;
            }
		}
		else
		{
			str[i] = c;
		}

		i++;
		
		/* Allocate more memory if needed */
		if(i >= size)
		{
			size += SZ;
			str = realloc(str,(size)*sizeof(char));
		}

	}

	/* Resize it to not take more space than needed */
	*s = realloc(str,i*sizeof(char)+1);

	return c == SCANNER_EOF ? EXIT_FAILURE : EXIT_SUCCESS ;
} 

/*
* A Function to check if the multiline comment and the nested comments end
*	Returns:	--EXIT_FAILURE (1) if the comment does not end
*				--EXIT_SUCCESS (0) if the comment ends properly
*/
static int multi_line_comment(AlphaToken_T ylval, int iteration)
{
	int c;
	int start = yylineno;
	while ((c = input()) > SCANNER_EOF)
	{
	/* 
	*  If * is followed by a char other than /
	*  return it to the input stream and continue parsing
	*/
		if(c == '*')
			if((c = input()) == '/')
				{
					AlphaToken_insert(ylval, start, yylineno, yytext
									, iteration == 0 ? "BLOCK_COMMENT":"NESTED_COMMENT"
									, "enumarated", "COMMENT");
					
					return EXIT_SUCCESS;
				}
			else
				unput(c);
		/* 
		*  Else / is followed by * start processing 
		*  nested comment
		*/
		else if(c == '/')
			if((c = input()) == '*')
					multi_line_comment(ylval,iteration+1);
			else
				unput(c);
	}
	return EXIT_FAILURE;
}

/*
* main
*/
int main(int argc, char** argv) 
{
	int c;
	AlphaToken_T head = AlphaToken_new();

	if(argc == 1)
	{
		fprintf(stderr,"Usage: %s INPUT_FILE\n", argv[0]);
		return EXIT_FAILURE;
	}

	if(!(yyin = fopen(argv[1],"r")))
	{
		LOG_ERROR(ERROR, "Cannot read Input file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	if((c = alpha_yylex((void*) head)) == EXIT_SUCCESS)
	{
		al(head);
	}
	
	AlphaToken_free(head);
	yylex_destroy();
	
	return c == EXIT_FAILURE 
			? 	EXIT_FAILURE
			:	EXIT_SUCCESS;
}
