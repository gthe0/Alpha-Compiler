/* 
*  Authors: 
* 
*
*
*/

/* DEFINITION SECTION
*
* Anything written inside the brackets will be copied 
* in the generated .c file
*/

%{
	#include <stdio.h>
	#include "lib/alpha_token.h"

	#if defined(WIN32) || defined(_WIN32_WCE)
	#define YY_NO_UNISTD_H
	static int isatty(int i) {return 0;}
	#endif

	/* Customize the default function name 'yylex' to 'alpha_yylex' */
    #define YY_DECL int alpha_yylex(void * ylval)
%}

%option noyywrap
%option yylineno		    

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
FLOAT   {digits}+\.{digits}+

LINE_COMMENT "//".*
MULTI_LINE_COMMENT "/\*"

/* RULES SECTION
*   
*   The Rules section contains a series of rules in the form:
*	- pattern action
*	- pattern 
*/

%%
/* KEYWORDS */
"if"				{AlphaToken_insert(head, yylineno, yytext, "IF", "KEYWORD");}
"else"				{AlphaToken_insert(head, yylineno, yytext, "ELSE", "KEYWORD");}
"while"				{AlphaToken_insert(head, yylineno, yytext, "WHILE", "KEYWORD");}
"for"				{AlphaToken_insert(head, yylineno, yytext, "FOR", "KEYWORD");}
"function"			{AlphaToken_insert(head, yylineno, yytext, "FUNC", "KEYWORD");}
"return"			{AlphaToken_insert(head, yylineno, yytext, "RET", "KEYWORD");}
"break"				{AlphaToken_insert(head, yylineno, yytext, "BREAK", "KEYWORD");}
"continue"			{AlphaToken_insert(head, yylineno, yytext, "CONTINUE", "KEYWORD");}
"and"				{AlphaToken_insert(head, yylineno, yytext, "AND", "KEYWORD");}
"not"				{AlphaToken_insert(head, yylineno, yytext, "NOT", "KEYWORD");}
"or"				{AlphaToken_insert(head, yylineno, yytext, "OR", "KEYWORD");}
"local"				{AlphaToken_insert(head, yylineno, yytext, "LOCAL", "KEYWORD");}
"true"				{AlphaToken_insert(head, yylineno, yytext, "TRUE", "KEYWORD");}
"false"				{AlphaToken_insert(head, yylineno, yytext, "FALSE", "KEYWORD");}
"nil"				{AlphaToken_insert(head, yylineno, yytext, "NIL", "KEYWORD");}
/* PUNCTUATIONS */
"["					{AlphaToken_insert(head, yylineno, yytext, "BRACKETS_OPEN", "PUNCT");}
"]"					{AlphaToken_insert(head, yylineno, yytext, "BRACKETS_CLOSE", "PUNCT");}
"{"					{AlphaToken_insert(head, yylineno, yytext, "BRACES_OPEN", "PUNCT");}
"}"					{AlphaToken_insert(head, yylineno, yytext, "BRACES_CLOSE", "PUNCT");}
"("					{AlphaToken_insert(head, yylineno, yytext, "PARENT_OPEN", "PUNCT");}
")"					{AlphaToken_insert(head, yylineno, yytext, "PARENT_CLOSE", "PUNCT");}
","					{AlphaToken_insert(head, yylineno, yytext, "COMMA", "PUNCT");}
";"					{AlphaToken_insert(head, yylineno, yytext, "SEMI_COL", "PUNCT");}
":"					{AlphaToken_insert(head, yylineno, yytext, "COL", "PUNCT");}
"::"				{AlphaToken_insert(head, yylineno, yytext, "DOUBLE_COL", "PUNCT");}
"."					{AlphaToken_insert(head, yylineno, yytext, "DOT", "PUNCT");}
".."				{AlphaToken_insert(head, yylineno, yytext, "DOUBLE_DOT", "PUNCT");}
"="					{AlphaToken_insert(head, yylineno, yytext, "ASSIGN_OP", "PUNCT");}
"+"					{AlphaToken_insert(head, yylineno, yytext, "PLUS_OP", "PUNCT");}
"-"					{AlphaToken_insert(head, yylineno, yytext, "MINUS_OP", "PUNCT");}
"*"					{AlphaToken_insert(head, yylineno, yytext, "MULT_OP", "PUNCT");}
"/"					{AlphaToken_insert(head, yylineno, yytext, "DIV_OP", "PUNCT");}
"%"					{AlphaToken_insert(head, yylineno, yytext, "MOD_OP", "PUNCT");}
"=="				{AlphaToken_insert(head, yylineno, yytext, "EQ_OP", "PUNCT");}
"!="				{AlphaToken_insert(head, yylineno, yytext, "NE_OP", "PUNCT");}
"++"				{AlphaToken_insert(head, yylineno, yytext, "INC_OP", "PUNCT");}
"--"				{AlphaToken_insert(head, yylineno, yytext, "DEC_OP", "PUNCT");}
">"					{AlphaToken_insert(head, yylineno, yytext, "GT_OP", "PUNCT");}
"<"					{AlphaToken_insert(head, yylineno, yytext, "LT_OP", "PUNCT");}
">="				{AlphaToken_insert(head, yylineno, yytext, "GE_OP", "PUNCT");}
"<="				{AlphaToken_insert(head, yylineno, yytext, "LE_OP", "PUNCT");}

/* IDENTIFIERS/NUMBERS */
{INT}				{AlphaToken_insert(head, yylineno, yytext, "INT", "INTCONST");}
{FLOAT}				{AlphaToken_insert(head, yylineno, yytext, "FLOAT", "REALCONST");}
{ID}				{AlphaToken_insert(head, yylineno, yytext, "ID", "IDENTIFIER");}

/* COMMENTS */
{LINE_COMMENT}  		{AlphaToken_insert(head, yylineno, yytext,"LINE_COMMENT","COMMENT");}
{MULTI_LINE_COMMENT}  	{AlphaToken_insert(head, yylineno, yytext,"MULTI_LINE_COMMENT","COMMENT");}

/* STRINGS */


/* IGNORED */
{WS}				{/*Ignore White Spaces*/}
.					{/*Ignore Illegal Chars*/}

%%




int main(){return 0;}
