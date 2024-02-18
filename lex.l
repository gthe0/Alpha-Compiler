/* =========================================================== 
*
* 
*
* =========================================================== */

/* DEFINITION SECTION
*
* Anything written in these brackets will be copied into the 
* generated .c file
*/

%{
    #if defined(WIN32) || defined(_WIN32_WCE)
    #define YY_NO_UNISTD_H
    static int isatty(int i) {return 0;}
    #endif
%}

%option noyywrap
%option yylineno
%option prefix="alpha_yy"

/* Flex macros
*
*   Here are defined the classes/terminal symbols,
*   the various grammars and the Rules that the lexical analyzer
*   will follow
*/

digits  [0-9]
delims  [ \t\n\r\v]
letters [A-Za-z]

/*
* Leximes Grammars
*/

WS	{delims}+
ID	{letters}({letters} | {digits} | [_] )*
INT	{digits}+
FLOAT   {digits}+\.{digits}+

STRING	\"[^\n"]\"
LINE_COMMENT 	"//".*


/* RULES SECTION
*   
*   The Rules section contains a series of rules in the form:
*	- pattern action
*	- pattern 
*/

%%

%%

int main(){return 0;}
