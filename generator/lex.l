/* 
*  Authors: 
* 
*
*
*/

/* DEFINITION SECTION
*
* Anything written inside the brackets will be copied 
* at the top of the generated .c file
*/

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <alpha_token.h>
	#include <al.h>

	#if defined(WIN32) || defined(_WIN32_WCE)
	#define YY_NO_UNISTD_H
	static int isatty(int i) {return 0;}
	#endif

	/* Customize the default function name 'yylex' to 'alpha_yylex' */
	#define YY_DECL int alpha_yylex(void * ylval)
	#define SZ 0x80

	/* Function definitions*/
	static int multi_line_comment();
	static int string(char **s);
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

digits  [0-9]
delims  [ \t\n\r\v]
letters [A-Za-z]

WS	    {delims}+
ID	    {letters}({letters}|{digits}|[_])*
INT	    {digits}+
FLOAT   {digits}"."{digits}+

STRING	\"
LINE_COMMENT "//".*
MULTI_LINE_COMMENT "/*"

/* RULES SECTION
*   
*   The Rules section contains a series of rules in the form:
*	- pattern action
*	- pattern 
*/

%%
"if"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "IF", "KEYWORD");}
"else"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "ELSE", "KEYWORD");}
"while"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "WHILE", "KEYWORD");}
"for"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "FOR", "KEYWORD");}
"function"			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "FUNC", "KEYWORD");}
"return"			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "RET", "KEYWORD");}
"break"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "BREAK", "KEYWORD");}
"continue"			{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "CONTINUE", "KEYWORD");}
"and"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "AND", "KEYWORD");}
"not"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "NOT", "KEYWORD");}
"or"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "OR", "KEYWORD");}
"local"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "LOCAL", "KEYWORD");}
"true"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "TRUE", "KEYWORD");}
"false"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "FALSE", "KEYWORD");}
"nil"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "NIL", "KEYWORD");}
"["					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "BRACKETS_OPEN", "PUNCT");}
"]"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "BRACKETS_CLOSE", "PUNCT");}
"{"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "BRACES_OPEN", "PUNCT");}
"}"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "BRACES_CLOSE", "PUNCT");}
"("					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "PARENT_OPEN", "PUNCT");}
")"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "PARENT_CLOSE", "PUNCT");}
","					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "COMMA", "PUNCT");}
";"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "SEMI_COL", "PUNCT");}
":"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "COL", "PUNCT");}
"::"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "DOUBLE_COL", "PUNCT");}
"."					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "DOT", "PUNCT");}
".."				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "DOUBLE_DOT", "PUNCT");}
"="					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "ASSIGN_OP", "PUNCT");}
"+"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "PLUS_OP", "PUNCT");}
"-"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "MINUS_OP", "PUNCT");}
"*"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "MULT_OP", "PUNCT");}
"/"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "DIV_OP", "PUNCT");}
"%"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "MOD_OP", "PUNCT");}
"=="				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "EQ_OP", "PUNCT");}
"!="				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "NE_OP", "PUNCT");}
"++"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "INC_OP", "PUNCT");}
"--"				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "DEC_OP", "PUNCT");}
">"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "GT_OP", "PUNCT");}
"<"					{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "LT_OP", "PUNCT");}
">="				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "GE_OP", "PUNCT");}
"<="				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "LE_OP", "PUNCT");}

{INT}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "INT", "INTCONST");}
{FLOAT}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "FLOAT", "REALCONST");}
{ID}				{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext, "ID", "IDENTIFIER");}

{LINE_COMMENT}  		{AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext,"LINE_COMMENT","COMMENT");}

{STRING}				{
							char* s = NULL;
							int start = yylineno;
							if(string(&s)== EXIT_FAILURE)
							{
								free(s);
								fprintf(stderr,"LINE %d: ERROR, Unterminated STRING\n",start);
								return 0;
							}
							AlphaToken_insert((AlphaToken_T) ylval, yylineno, s, "STRING", "STRING");	
						}

{MULTI_LINE_COMMENT}  	{
							int start = yylineno;
							if(multi_line_comment() == EXIT_FAILURE)
							{
								fprintf(stderr,"LINE %d: ERROR, Unterminated COMMENT\n",start);
								return 0;
							}
							AlphaToken_insert((AlphaToken_T) ylval, yylineno, yytext,"MULTI_LINE_COMMENT","COMMENT");
						}


{WS}				{/*Ignore White Spaces*/}
.					{return 0;}

%%

/*
* A Function to check if the strings are correct.
* If there are any invalid escape sequences it prints some warnings.
*
*	Returns:	--EXIT_FAILURE (1) if the string does not end
*				--EXIT_SUCCESS (0) if the string ends properly
*/
static int string(char **s)
{
	char *str; 
	int c = 0, i = 0, resize = 1 ;
	
	/* Allocate memory for string*/
	str = malloc(SZ*resize*sizeof(char));
	
	while((c = input()) != EOF )
	{
		/* Checks if the string ends with " */
		if(c == '\"')
		{
			str[i] = '\0';
			break;
		}

		/* Checks if the escape sequences are correct */
		if(c == '\\')
		{
			c = input();
			
			if(c == 't')
			{
				str[i] = '\t';
			}			
			else if(c == 'n')
			{
				str[i] = '\n';
			}
			else if(c == '\"')
			{
				str[i] = '\"';
			}
			else
			{
				str[i] = '\\';
				fprintf(stderr,"LINE %d: WARNING, invalid escape sequence \\%c\n",yylineno,c);
				unput(c);
			}
		}
		else
		{
			str[i] = c;
		}

		i++;
		
		/* Allocate more memory if needed */
		if(i >= SZ*resize)
		{
			resize++;
			str = realloc(str,SZ*resize);
		}
	}

	*s = realloc(str,i*sizeof(char)+1);
	return c == EOF ? EXIT_FAILURE : EXIT_SUCCESS ;
} 

/*
* A Function to check if the multiline comment ends
*	Returns:	--EXIT_FAILURE (1) if the comment does not end
*				--EXIT_SUCCESS (0) if the comment ends properly
*/
static int multi_line_comment()
{
	int c; 
	while ((c = input()) != EOF)
	{
	/* 
	*  If * is followed by a char other than /
	*  return it to the input stream and continue parsing
	*/
		if(c == '*')
			if((c = input()) == '/')
				return EXIT_SUCCESS;
			else
				unput(c);
	}
	
	return EXIT_FAILURE;
}	

/*
* main given in a lecture XD
*/
int main(int argc, char** argv) 
{
	AlphaToken_T head = AlphaToken_new();
	
	if(argc > 1)
		if(!(yyin = fopen(argv[1], "r"))){
			fprintf(stderr, "Cannot read file: %s\n", argv[1]);
			return 1;
		}
	else
		yyin = stdin;

	alpha_yylex((void*) head);
	
	al(head);
	AlphaToken_free(head);
	
	return 0;
}