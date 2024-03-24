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
	#include <parser.h>
	#include <log.h>

	#if defined(WIN32) || defined(_WIN32_WCE)
	#define YY_NO_UNISTD_H
	static int isatty(int i) {return 0;}
	#endif

	/* Customize the default function name 'yylex' to 'alpha_yylex' */
	#define SCANNER_EOF YY_END_OF_BUFFER_CHAR
	#define SZ 0x40

	/* Function definitions*/
	static int multi_line_comment(void);
	static int valid_string(void);
%}

%option noyywrap
%option yylineno	

/* Flex macros
*
*   Here are defined the classes/terminal symbols,
*   the various grammars and the Rules that the lexical analyzer
*   will follow
*/

digits  		[0-9]
delims         	[ \t\n\r\v]
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
"if"				{return (IF);}
"else"				{return (ELSE);}
"while"				{return (WHILE);}
"for"				{return (FOR);}
"function"			{return (FUNC);}
"return"			{return (RET);}
"break"				{return (BREAK);}
"continue"			{return (CONTINUE);}
"and"				{return (AND);}
"not"				{return (NOT);}
"or"				{return (OR);}
"local"				{return (LOC);}
"true"				{return (TRUE);}
"false"				{return (FALSE);}
"nil"				{return (NIL);}
 
"::"				{return (DOUBLE_COL);}
".."				{return (DOUBLE_DOT);}
"=="				{return (EQ_OP);}
"!="				{return (NE_OP);}
"++"				{return (INC_OP);}
"--"				{return (DEC_OP);}
">="				{return (GE_OP);}
"<="				{return (LE_OP);}

">"					{return ('>');}
"<"					{return ('<');}
"["					{return ('[');}
"]"					{return (']');}
"{"					{return ('{');}
"}"					{return ('}');}
"("					{return ('(');}
")"					{return (')');}
","					{return (',');}
";"					{return (';');}
":"					{return (':');}
"."					{return ('.');}
"="					{return ('=');}
"+"					{return ('+');}
"-"					{return ('-');}
"%"					{return ('%');}
"*"					{return ('*');}
"/"					{return ('/');}

{ID}				{yylval.string = strdup(yytext); return (ID);}
{INT}				{yylval.intVal	 = atoi(yytext); return (INT);}
{FLOAT}				{yylval.floatVal = atof(yytext); return (FLOAT);}

{LINE_COMMENT}  	{/* COMMENT */}

{BLOCK_COMMENT}		{
						int start = yylineno;
						if(multi_line_comment() == EXIT_FAILURE)
						{
							LOG_ERROR(LEXER,ERROR,"Unterminated COMMENT in line %d\n",start);
							exit(EXIT_FAILURE);
						}
					}

{STRING}			{
						int start = yylineno;
						if(valid_string()== EXIT_FAILURE)
						{
							LOG_ERROR(LEXER,ERROR,"Unterminated STRING in line %d\n",start);
							exit(EXIT_FAILURE);
						}

						return (STRING);
					}


{WS}				{/*Ignore White Spaces*/}
.					{
						LOG_ERROR(LEXER,ERROR,"Illegal character %s in line %d\n",yytext,yylineno);
						exit(EXIT_FAILURE);
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
static int valid_string(void)
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
	if(c == SCANNER_EOF)
		yylval.string = strdup("");
	else
		yylval.string = strdup(str);

	free(str);
	
	return c == SCANNER_EOF ? EXIT_FAILURE : EXIT_SUCCESS ;
} 

/*
* A Function to check if the multiline comment and the nested comments end
*	Returns:	--EXIT_FAILURE (1) if the comment does not end
*				--EXIT_SUCCESS (0) if the comment ends properly
*/
static int multi_line_comment(void)
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
					return EXIT_SUCCESS;
			else
				unput(c);
	/* 
	*  Else / is followed by * start processing 
	*  nested comment
	*/
		else if(c == '/')
			if((c = input()) == '*')
				multi_line_comment();
			else
				unput(c);
	}

	return EXIT_FAILURE;
}