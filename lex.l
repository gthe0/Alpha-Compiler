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
    #include <stdlib.h>
    #include <string.h>
	#include "parser.h"
	#include "log.h"

	#if defined(WIN32) || defined(_WIN32_WCE)
	#define YY_NO_UNISTD_H
	static int isatty(int i) {return 0;}
	#endif

	/* Customize the default function name 'yylex' to 'alpha_yylex' */
	#define YY_DECL int alpha_yylex(void * ylval)	
	#define SCANNER_EOF YY_END_OF_BUFFER_CHAR
	#define SZ 0x40

	/* Function definitions*/
	static int multi_line_comment(int i);
	static int valid_string(char **s);
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

digits  	[0-9]
letters 	[A-Za-z]

id	    	{letters}({letters}|{digits}|[_])*
integer		{digits}+
float   	{digits}+"."{digits}+

STRING		\"
LINE_COMMENT	"//".*
BLOCK_COMMENT	"/*"

/* RULES SECTION
*   
*   The Rules section contains a series of rules in the form:
*	- pattern action
*	- pattern 
*/

%%
"if"				{return IF;}
"else"				{return ELSE;}
"while"				{return WHILE;}
"for"				{return FOR;}
"func"				{return FUNC;}
"return"			{return RETURN;}
"break"				{return BREAK;}
"continue"			{return CONTINUE;}
"and"				{return AND;}
"not"				{return NOT;}
"or"				{return OR;}
"local"				{return LOCAL;}
"true"				{return TRUE;}
"false"				{return FALSE;}
"nil"				{return NIL;}

"["					{return BRACKETS_O;}
"]"			 		{return BRACKETS_C;}
"{"					{return BRACES_O;}
"}"					{return BRACES_C;}
"("					{return PAREN_O;}
")"					{return PAREN_C;}
","					{return COMMA;}
";"					{return SEMI_COLON;}
":"					{return COLON;}
"::"				{return DOUBLE_COL;}
"."					{return DOT;}
".."				{return DOUBLE_DOT;}

"="             	{return ASSIGN;}
"+"             	{return PLUS;}
"-"             	{return MINUS;}
"*"             	{return MUL;}
"/"             	{return DIV;}
"%"             	{return MOD;}
">"					{return GREATER;}
"<"					{return LESS;}
 
"=="				{return EQ_OP;}
"!="				{return NE_OP;}
"++"				{return INC_OP;}
"--"				{return DEC_OP;}
">="				{return GE_OP;}
"<="				{return LE_OP;}

[ \t\n\r\v]+	{/*Ignore White Spaces*/}

{integer}			{yylval.intVal = atoi(yytext); return INT;}
{float}				{yylval.floatVal = atof(yytext); return FLOAT;}
{id}				{yylval.stringVal = strdup(yytext);	return ID;} 

{LINE_COMMENT}  { /*Ignore Line Comments*/ }

{STRING}		{
					int start = yylineno;
					char* s = NULL;
					if(valid_string(&s)== EXIT_FAILURE)
					{
						free(s);
						LOG_ERROR(LEXER, ERROR,"Unterminated STRING in line %d\n",start);
						return EXIT_FAILURE;
					}
					//AlphaToken_insert((AlphaToken_T) ylval, start, yylineno, s, "STRING", "char *", "STRING");
					free(s);
				}

{BLOCK_COMMENT}		{
						int start = yylineno;
						if(multi_line_comment(0) == EXIT_FAILURE)
						{
							LOG_ERROR(LEXER, ERROR,"Unterminated COMMENT in line %d\n",start);
							return EXIT_FAILURE;
						}
			}




%%

/* CODE SECTION
*	Here we write the code/functions that will be 
*	copied in the bottom of the generated file
*/


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
				case '\\': str[i] = '\\'; break;
				default:
					str[i] = '\\';
					LOG_ERROR(LEXER, WARNING, "Invalid escape sequence \\%c in line %d\n", c, yylineno);
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
static int multi_line_comment(int iteration)
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
					if(iteration == 0)
						printf("BLOCK_COMMENT\n");
					else	
						printf("NESTED_COMMENT\n");
					
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
					multi_line_comment(iteration+1);
			else
				unput(c);
	}
	return EXIT_FAILURE;
}


// /*
// * main
// */
// int main(int argc, char** argv) 
// {
// 	FILE* ost;
// 	int c;

// 	if(argc == 1)
// 	{
// 		LOG_ERROR(LEXER, USAGE,"%s <INPUT_FILE>\n", argv[0]);
// 		return EXIT_FAILURE;
// 	}

// 	/* Open the input file */
// 	if(!(yyin = fopen(argv[1],"r")))
// 	{
// 		LOG_ERROR(LEXER, ERROR, "Cannot read Input file %s\n", argv[1]);
// 		return EXIT_FAILURE;
// 	}
	
// 	/* If the output file is not provided use ost */
// 	if(argc > 2)
// 	{
// 		if(!(ost = fopen(argv[2],"w")))
// 		{
// 			LOG_ERROR(LEXER, ERROR, "Cannot write to specified file %s\n", argv[1]);
// 			return EXIT_FAILURE;
// 		}
// 	}
// 	else ost = stdout;

// 	AlphaToken_T head = AlphaToken_new();

// 	/* If there are no errors print list */
// 	if((c = alpha_yylex((void*) head)) == EXIT_SUCCESS)
// 	{
// 		al(head,ost);
// 	}

// 	/* Close streams and clean up */
// 	fclose(ost);
// 	fclose(yyin);

// 	AlphaToken_free(head);
// 	yylex_destroy();
	
// 	return c == EXIT_FAILURE 
// 			? 	EXIT_FAILURE
// 			:	EXIT_SUCCESS;
// }
