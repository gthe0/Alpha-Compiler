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

	#include <stmt.h>
	#include <expr.h>
	#include <log.h>
	#include <tables.h>
	#include <name_gen.h>
	#include <symTable.h>
	#include <scopeTable.h>
	#include <scopeSpace.h>
	#include <IntegerStack.h>
	#include <parser_utils.h>
	#include <symTableEntry.h>

	#if defined(WIN32) || defined(_WIN32_WCE)
	#define YY_NO_UNISTD_H
	static int isatty(int i) {return 0;}
	#endif

	/* The various stacks that we will use */
	static ScopeStack_T  	oScopeStack = NULL;
	static OffsetStack_T  	offsetStack = NULL;

	/* If Compilation Error is
	 encountered, it will be set to 1 */
	unsigned int ERROR_COMP = 0;

	/*Counter used to see in how many loops are we in */
	static unsigned int loop_counter = 0;

	/* The current scope */
	unsigned int scope = 0;

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
	unsigned unsignedVal;
	int	intVal;
	float floatVal;
	char* string;
	expr* expression;
	SymEntry_T entry;
	stmt_T statement;
}

%token <string> 	ID STRING 
%token <intVal> 	INT
%token <floatVal> 	FLOAT

%type <entry> 		lvalue funcpref
%type <string> 		func_name
%type <statement> 	stmt
%type <expression>	const 
%type <unsignedVal> funcbody


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
	{
		reset_temp();
		$$ = NULL;
	}
	| ifstmt
	{
		reset_temp();
		$$ = NULL;
	}
	| whilestmt
	{
		reset_temp();
		$$ = NULL;
	}
	| forstmt
	{
		reset_temp();
		$$ = NULL;
	}
	| returnstmt
	{
		reset_temp();
		$$ = NULL;
	}
	| BREAK ';' 	
	{
		reset_temp();
		$$ = Manage_loop_stmt("break",loop_counter,yylineno);
	}
	| CONTINUE ';'	
	{
		reset_temp();
		$$ = Manage_loop_stmt("continue",loop_counter,yylineno);
	}
	| block
	{
		reset_temp();
		$$ = NULL;
	}
	| funcdef
	{
		reset_temp();
		$$ = NULL;
	}
	| ';'
	{
		reset_temp();
		$$ = NULL;
	}
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
	| INC_OP lvalue 	{eval_lvalue($2,"++",yylineno);}
	| lvalue INC_OP		{eval_lvalue($1,"++",yylineno);}
	| DEC_OP lvalue		{eval_lvalue($2,"--",yylineno);}
	| lvalue DEC_OP		{eval_lvalue($1,"--",yylineno);}
	| primary
	;

assginexpr
	: lvalue '=' expr 	{eval_lvalue($1,"assignment",yylineno);}
	| error '=' expr 	{LOG_ERROR(PARSER,NOTE,"Wrong lvalue in assignment, line %u\n",yylineno); yyerrok;} 
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
	{
		$$ = Manage_lv_ID($1,yylineno, scope ,oScopeStack);
	}
	| LOC ID
	{
		$$ =  Manage_lv_local($2,yylineno,scope);
	}
	| DOUBLE_COL ID	
	{
		$$ = Manage_lv_global($2,yylineno);
	}
	| member	
	{
		$$ = NULL;
	}
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
	| DOUBLE_DOT ID '(' error ')' {  yyerrok;} 
	| DOUBLE_DOT ID '('  ')' 
	; // equivalent to lvalue.id(lvalue, elist)

elist
	: expr 
	| elist ',' expr
	| elist ',' error	{ yyerrok;} 
	;

objectdef
	:	'[' object_list ']'
	|	'[' error ']'	{  yyerrok;} 
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
	: '{' expr  ':' expr '}'
	| '{' error ':' expr '}'		{ yyerrok;} 
	| '{' expr  ':' error '}'		{ yyerrok;} 
	| '{' expr error expr '}'		{ yyerrok;} 
	;

block
	:
	'{' 
	{
		scope++;
	} 
	stmt_list 
	'}'
	{
		ScopeTable_hide(scope);
		scope--;
	}
	;

funcpref
	:	FUNC func_name 
	{
		$$ = Manage_func_pref($2,yylineno,scope,oScopeStack);
		
		emit(funcstart_i, $$ ? lvalue_expr($$) : NULL , NULL, NULL,0,yylineno);
		
		IntStack_Push(&offsetStack, curr_scope_offset()); 
		IntStack_Push(&oScopeStack, scope+1); 
		
		enterscopespace(); 
		resetformalargoffset();
	}
	;

funcargs
	: '(' {scope++;} idlist ')'
	{
		resetfunctionlocaloffset();
		enterscopespace(); 
		scope--;
	}
	;

funcbody
	: block 
	{
		$$ = curr_scope_offset();
		exitscopespace();
	}
	;

funcdef
	: funcpref
	  funcargs
	  funcbody			
	{
		exitscopespace();
		IntStack_Pop(oScopeStack);
		restore_curr_scope_offset(IntStack_Pop(offsetStack));
	}
	;

func_name
	: 		
	{
		$$ =  func_name_generator();
	}
	| ID	
	{
		$$ = $1;
	}
	;

const
	: INT 		{ $$ = new_num_expr($1); }
	| FLOAT 	{ $$ = new_num_expr($1); }
	| NIL 		{ $$ = new_nil_expr(  ); }
	| TRUE 		{ $$ = new_bool_expr(1); }
	| FALSE 	{ $$ = new_bool_expr(0); }
	| STRING 	{ $$ = new_string_expr($1);}
	;

idlist
	: 
	| ID
	{
		if((Valid_args($1,yylineno,scope, oScopeStack))== EXIT_SUCCESS)
		{
			Tables_insert(FORMAL,$1,scope,yylineno);
		}
	}
	| idlist ',' ID	
	{		
		if((Valid_args($3,yylineno,scope, oScopeStack))== EXIT_SUCCESS)
		{
			Tables_insert(FORMAL,$3,scope,yylineno);
		}
	}
	;

ifstmt
	:  IF '(' expr ')' stmt
	|  IF '(' expr ')' stmt ELSE stmt
	|  IF '(' error ')'				 	{  yyerrok;} 
	;

loop_Inc:	{loop_counter++;}
loop_End:	{loop_counter--;}

whilestmt
	: WHILE '(' expr ')' loop_Inc  stmt loop_End 
	| WHILE '(' error ')' loop_Inc  stmt loop_End {  yyerrok;} 
	;

forstmt
	: FOR '(' elist ';' expr ';' elist ')' loop_Inc  stmt loop_End
	| FOR '(' elist ';' expr ';' ')' loop_Inc stmt loop_End
	| FOR '(' elist ';' error ';' elist')' loop_Inc stmt loop_End	{  yyerrok;} 
	| FOR '(' elist ';' error ';' ')' loop_Inc stmt loop_End		{  yyerrok;} 
	;

returnstmt
	: RET expr ';'	{Valid_return(oScopeStack,yylineno);}
	| RET ';'		{Valid_return(oScopeStack,yylineno);}
	;
%%
/* Same as lex */

int yyerror(const char* s)
{
	LOG_ERROR(PARSER,ERROR,"%s, line %d\n",s ,yylineno);
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
	oScopeStack = IntStack_init();
	Tables_init();
	expand();
	/* Call the Parser */
	yyparse();

	printf("%30.100s\n",ERROR_COMP ? "COMPILATION ERROR ENCOUNTERD" : "ALL FINE");

	Tables_print(ost,0);

	/* Close streams and clean up */
	Tables_free();
	IntStack_free(oScopeStack);

	fclose(ost);
	fclose(yyin);
	
	return 0;
}