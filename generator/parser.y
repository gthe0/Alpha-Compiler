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
	#include <yacc_lib.h>
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
	static unsigned int loop_counter = 0;

	/* Flex variables */
	extern FILE* 	yyin;
	extern char*	yytext;
	extern int 		yylineno;
	
	int yylex(void);
	int yyerror(const char* s);
%}

%define parse.error verbose

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
%left AND
%left OR
%left '+' '-'
%left '*' '/' '%'
%nonassoc UNARY_MINUS
%right NOT INC_OP DEC_OP
%left '.' DOUBLE_DOT
%left '[' ']'
%left '(' ')'

%nonassoc EQ_OP  NE_OP
%nonassoc '<' '>' GE_OP  LE_OP

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
	| BREAK ';' {
		if(loop_counter == 0)
			LOG_ERROR(PARSER,ERROR,"BREAK outside a loop\n");
	}
	| CONTINUE ';'{
		if(loop_counter == 0)
			LOG_ERROR(PARSER,ERROR,"CONTINUE outside a loop\n");
	}
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
	: ID			{
		
	}
	| LOC ID		{
		if(lvalue_local(oSymTable,$2,scope) == 0)
		{
			Tables_insert(oSymTable,oScopeTable,LOCAL,$2,scope,yylineno);
		}
	}
	| DOUBLE_COL ID	{

	}
	| member
	;

member
	: call '.' ID
	| lvalue '.' ID
	| call '[' expr ']'
	| lvalue '[' expr ']'
	;

call: call '(' ')'
	| call '(' elist ')'
	| lvalue callsuffix
	| '(' funcdef ')' '(' elist ')'
	| '(' funcdef ')' '('  ')'
	;

callsuffix
	: normcall
	| methodcall 
	;

normcall
	: '(' elist ')'
	| '(' ')'
	;

methodcall
	: DOUBLE_DOT ID '(' elist ')' 
	| DOUBLE_DOT ID '('  ')' 
	; // equivalent to lvalue.id(lvalue, elist)

elist
	: expr 
	| elist ',' expr
	;

objectdef
	:	'[' object_list ']'
	;

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
	: '{' {scope++;} stmt_list '}'{
		
		ScopeTable_hide(oScopeTable,scope);
		scope--;}
	;

funcpref
	:	FUNC id_option 
	;

funcdef
	: funcpref	'('	{scope++;}
	  idlist	')'	{scope--;}
	  block			{
						ScopePop(oScopeStack);
					}
	;

id_option
	: /* empty production, making id_option optional */
	| ID{
		if((lvalue_Function(oSymTable,$1,scope, oScopeStack))==0)
		{
			Tables_insert(oSymTable,oScopeTable,USERFUNC,$1,scope,yylineno);
			oScopeStack = ScopePush(oScopeStack,scope+1);
		}
	}
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

loop_Inc:	{loop_counter++;}
loop_End:	{loop_counter--;}

whilestmt
	: WHILE '(' expr ')' loop_Inc  stmt loop_End 
	;

forstmt
	: FOR '(' elist ';' expr ';' elist ')' loop_Inc  stmt loop_End
	| FOR '(' elist ';' expr ';' ')' loop_Inc stmt loop_End
	;

returnstmt
	: RET expr ';'	{if(ScopeIsEmpty(oScopeStack)) LOG_ERROR(PARSER,ERROR, "Return outside a Function\n");}
	| RET ';'		{if(ScopeIsEmpty(oScopeStack)) LOG_ERROR(PARSER,ERROR, "Return outside a Function\n");}
	;
%%
/* Same as lex */

int yyerror(const char* s)
{
	LOG_ERROR(PARSER,ERROR,"%s, line %d, token %s\n",s ,yylineno, yytext);
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

	/* Call the Parser */
	yyparse();

	Tables_print(oSymTable,oScopeTable,ost,0);

	/* Close streams and clean up */
	Tables_free(oSymTable,oScopeTable);
	ScopeFree(oScopeStack);

	fclose(ost);
	fclose(yyin);
	
	return 0;
}