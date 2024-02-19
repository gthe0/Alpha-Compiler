/* =========================================================== 
*  Authors: 
* 
*
* =========================================================== */

/* DEFINITION SECTION
*
* Anything written inside the brackets will be copied 
* in the generated .c file
*/

%{
    #include <stdio.h>

    #if defined(WIN32) || defined(_WIN32_WCE)
    #define YY_NO_UNISTD_H
    static int isatty(int i) {return 0;}
    #endif
%}

%option noyywrap 		    // Do not define yywrap function
%option yylineno		    // Store the line number of the parsed lexime
%option prefix="alpha_yy"	// Change the Prefix from yy to alpha_yy

/* Flex macros
*
*   Here are defined the classes/terminal symbols,
*   the various grammars and the Rules that the lexical analyzer
*   will follow
*/

digits  [0-9]
delims  [ \t\n\r\v]
letters [A-Za-z]

WS	{delims}+
ID	{letters}({letters} | {digits} | [_] )*
INT	{digits}+
FLOAT   {digits}+\.{digits}+

/* RULES SECTION
*   
*   The Rules section contains a series of rules in the form:
*	- pattern action
*	- pattern 
*/

%%
"if"                    {return (IF);}
"else"                  {return (ELSE);}
"while"                 {return (WHILE);}
"for"                   {return (FOR);}
"function"              {return (FUNC);}
"return"                {return (RET);}
"break"                 {return (BREAK);}
"continue"              {return (CONTINUE);}
"and"                   {return (AND);}
"not"                   {return (NOT);}
"or"                    {return (OR);}
"local"                 {return (LOCAL);}
"true"                  {return (TRUE);}
"false"                 {return (FALSE);}
"nil"                   {return (NIL);}
"["                     {return (BRACKETS_OPEN);}
"]"                     {return (BRACKETS_CLOSE);}
"{"                     {return (BRACES_OPEN);}
"}"                     {return (BRACES_CLOSE);}
"("                     {return (PARENT_OPEN);}
")"                     {return (PARENT_CLOSE);}
","                     {return (COMMA);}
";"                     {return (SEMI_COL);}
":"                     {return (COL);}
"::"                    {return (DOUBLE_COL);}
"."                     {return (DOT);}
".."                    {return (DOUBLE_DOT);}
"="                     {return (ASSIGN_OP);}
"+"                     {return (PLUS_OP);}
"-"                     {return (MINUS_OP);}
"*"                     {return (MULT_OP);}
"/"                     {return (DIV_OP);}
"%"                     {return (MOD_OP);}
"=="                    {return (EQ_OP);}
"!="                    {return (NE_OP);}
"++"                    {return (INC_OP);}
"--"                    {return (DEC_OP);}
">"                     {return (GT_OP);}
"<"                     {return (LT_OP);}
">="                    {return (GE_OP);}
"<="                    {return (LE_OP);}
{INT}                   {return (INT);}
{FLOAT}                 {return (FLOAT);}
{ID}                    {return (ID);}
{WS}                    {}
%%

int main(){return 0;}
