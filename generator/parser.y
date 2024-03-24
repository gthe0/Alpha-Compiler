/* 
*  Authors: csd4881  csd4988  csd5038
*			
* 	parser.y
*
*  YACC File to write the context-free grammar
*  and generate the syntactical analyzer.
*/

/* 
* Same as lex. Anything written inside 
*  will be copied on top of the generated c file
*/
%{
	#include <stdio.h>
	#include <stdlib.h>

	#include <ScopeStack.h>
	#include <tables.h>
	#include <log.h>

	#if defined(WIN32) || defined(_WIN32_WCE)
	#define YY_NO_UNISTD_H
	static int isatty(int i) {return 0;}
	#endif

	/* The various tables that we will use */
	static ScopeStack_T  oScopeStack = NULL;
	static ScopeTable_T oScopeTable = NULL;
	static SymTable_T oSymTable = NULL;

	static unsigned int scope = 0;
	
	/* Flex variables */
	extern FILE* 	yyin;
	extern char*	yytext;
	extern int 		yylineno;
	
	int yylex(void);
	int yyerror(char* s);
%}

%union
{
	int	intVal;
	char* string;
	float floatVal;
}

%token <string> 	ID STRING
%token <intVal> 	INT
%token <floatVal> 	FLOAT

%token IF  ELSE  WHILE  FOR  FUNC  RET  BREAK  CONTINUE  
%token AND  NOT  OR
%token LOC  TRUE  FALSE  NIL 
%token EQ_OP  NE_OP  INC_OP  DEC_OP  GE_OP  LE_OP
%token DOUBLE_COL  DOUBLE_DOT
%token ',' ';' ':' '.' '=' '+' '-' '%' '*' '/'
%token '[' ']' '(' ')' '{' '}' '<' '>'

%right '='
%nonassoc '<' '>'
%left '+' '-' ':' '.' '*' '/' '%' '[' ']' '(' ')'

%right NOT INC_OP DEC_OP UNARY_MINUS
%nonassoc EQ_OP  NE_OP  GE_OP  LE_OP
%left DOUBLE_DOT DOUBLE_COL AND OR

%precedence ELSE

%start program
%%
program
	: stmt_list /* stmt* means zero or more stmt */
	;

stmt_list
	: /* empty production, making stmt_list optional */
	| stmt_list stmt 
	;
	
stmt: expr ';'
	| ifstmt
	| whilestmt
	| forstmt
	| returnstmt
	| BREAK ';'
	| CONTINUE ';'
	| block
	| funcdef
	| ';'
	;

expr: assginexpr
	| expr '+' expr
	| expr '-' expr
	| expr '*' expr
	| expr '/' expr
	| expr '%' expr
	| expr '>' expr
	| expr '<' expr
	| expr GE_OP expr
	| expr LE_OP expr
	| expr EQ_OP expr
	| expr NE_OP expr
	| expr AND expr
	| expr OR expr
	| term
	;

term: '(' expr ')'
	| '-' expr	%prec UNARY_MINUS
	| NOT expr
	| INC_OP lvalue
	| lvalue INC_OP
	| DEC_OP lvalue
	| lvalue DEC_OP
	| primary
	;

assginexpr
	: lvalue '=' expr 
	;

primary
	: lvalue
	| call
	| objectdef
	| '(' funcdef ')'
	| const
	;

lvalue
	: ID
	| LOC ID
	| DOUBLE_COL ID
	| member
	;

member
	: call '.' ID
	| lvalue '.' ID
	| call '[' expr ']'
	| lvalue '[' expr ']'
	;

call: call '(' elist ')'
	| lvalue callsuffix
	| '(' funcdef ')' '(' elist ')'
	;

callsuffix
	: normcall
	| methodcall 
	;

normcall
	: '(' elist ')' 
	;

methodcall
	: DOUBLE_DOT ID '(' elist ')' 
	; // equivalent to lvalue.id(lvalue, elist)

elist
	: expr 
	| elist ',' expr
	;

objectdef:  '[' object_list ']' ;

object_list
	: elist
	| indexed
	;

indexed
	:   
	| indexed ',' indexedelem
	| indexedelem
	;

indexedelem
	: '{' expr ':' expr '}'
	;

block
	: '{' stmt_list '}' 
	;

funcdef
	: FUNC id_option '(' idlist ')' block
	;

id_option
	: /* empty production, making id_option optional */
	| ID
	;

const
	: INT 
	| NIL 
	| TRUE 
	| FALSE 
	| FLOAT 
	| STRING 
	;

idlist
	: 
	| ID
	| idlist ',' ID
	;

ifstmt
	:  IF '(' expr ')' stmt
	|  IF '(' expr ')' stmt ELSE stmt
	;

whilestmt
	: WHILE '(' expr ')' stmt 
	;

forstmt
	: FOR '(' elist ';' expr ';' elist ')' stmt
	;

returnstmt
	: RET expr ';'
	| RET ';'
	;
%%
/* Same as lex */

int yyerror(char* s)
{
	LOG_ERROR(PARSER,ERROR,"%s, line %d, token %s",s ,yylineno, yytext);
	return EXIT_FAILURE;
}

/* main */
int main(int argc,char** argv)
{
	FILE* ost;

	if(argc == 1)
	{
		LOG_ERROR(PARSER, USAGE,"%s <INPUT_FILE>\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open the input file */
	if(!(yyin = fopen(argv[1],"r")))
	{
		LOG_ERROR(PARSER, ERROR, "Cannot read Input file %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	
	/* If the output file is not provided use ost */
	if(argc > 2)
	{
		if(!(ost = fopen(argv[2],"w")))
		{
			LOG_ERROR(PARSER, ERROR, "Cannot write to specified file %s\n", argv[1]);
			return EXIT_FAILURE;
		}
	}
	else ost = stdout;

	/* Initializes tables and stack */
	oScopeStack = ScopeStack_init();
	Tables_init(&oSymTable,&oScopeTable);
	
	yyparse();
	
	/* Close streams and clean up */
	Tables_free(oSymTable,oScopeTable);

	fclose(ost);
	fclose(yyin);
	
	return 0;
}