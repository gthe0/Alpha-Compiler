/* 
*  Authors: csd4881  csd4988  csd5038
*			
* 	parser.y
*
*  YACC File to write the context-free grammar
*  and generate the syntactical analyzer.
*/

/* Same as lex. Anything written inside 
*  will be copied on top of the generated c file
*/
%{
	#if defined(WIN32) || defined(_WIN32_WCE)
	#define YY_NO_UNISTD_H
	static int isatty(int i) {return 0;}
	#endif

	#include <log.h>
%}

%union
{
    int int_val;
    char* string;
    float float_value;
}

%token <string> 		ID STRING
%token <int_val> 		INT
%token <float_value> 	FLOAT

%token IF  ELSE  WHILE  FOR  FUNC  RET  BREAK  CONTINUE  
%token AND  NOT  OR  LOCAL  TRUE  FALSE  NIL 
%token EQ_OP  NE_OP  INC_OP  DEC_OP  GE_OP  LE_OP
%token DOUBLE_COL  DOUBLE_DOT

%start parser
%%


%%

yyerror(char* s)
{
	LOG_ERROR(PARSER,ERROR,"%s, line %d",s ,yylineno);
}

