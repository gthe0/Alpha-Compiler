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
	#include <assert.h>
	#include <stdio.h>
	#include <stdlib.h>

	#include <call.h>
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
	call_T call_object;
}

%token <string> 	ID STRING 
%token <intVal> 	INT
%token <floatVal> 	FLOAT

%type <call_object> callsuffix normcall methodcall
%type <entry> 		funcpref funcdef 
%type <string> 		func_name
%type <statement> whilestmt	stmt_list stmt block loop_stmt forstmt returnstmt funcbody
%type <expression>	const primary expr lvalue member term assginexpr elist call
%type <unsignedVal> prebody funcstart

%destructor {free($$);} stmt

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
	: /* empty production, making stmt_list optional */
	| stmt_list /* stmt* means zero or more stmt */
	;

stmt_list
	: stmt { $$ = $1 ; }
	| stmt_list stmt {
		
		$$ = new_stmt();
		$$->retlist = mergelist($1->retlist,$2->retlist);
		$$->contlist = mergelist($1->contlist,$2->contlist);
		$$->breaklist = mergelist($1->breaklist,$2->breaklist);
		
	}
	;
	
stmt: expr ';' 
	{
		reset_temp();
		$$ = new_stmt();
	}
	| ifstmt
	{
		reset_temp();
		$$ = new_stmt();
	}
	| whilestmt
	{
		reset_temp();
		$$ = new_stmt();
		$$->retlist = $1->retlist;
	}
	| forstmt
	{
		reset_temp();
		$$ = new_stmt();
		$$->retlist = $1->retlist;
	}
	| returnstmt
	{
		reset_temp();
		$$ = $1 ;
	}
	| BREAK ';' 	
	{
		reset_temp();
		$$ = Manage_loop_token("break",loop_counter,yylineno);
	}
	| CONTINUE ';'	
	{
		reset_temp();
		$$ = Manage_loop_token("continue",loop_counter,yylineno);
	}
	| block
	{
		reset_temp();
		$$ = $1;
	}
	| funcdef
	{
		reset_temp();
		$$ = new_stmt();
	}
	| ';'
	{
		reset_temp();
		$$ = new_stmt();
	}
	;

expr: assginexpr				{$$ = $1 ;}
	| expr '+' expr				{$$ = Manage_arithmetic_expr($1,$3,add_i,"ADDITION",scope,yylineno);}
	| expr '-' expr				{$$ = Manage_arithmetic_expr($1,$3,sub_i,"SUBTRACTIOM",scope,yylineno);}
	| expr '*' expr				{$$ = Manage_arithmetic_expr($1,$3,mul_i,"MULTIPLICATIOn",scope,yylineno);}
	| expr '%' expr				{$$ = Manage_arithmetic_expr($1,$3,div_i,"DIVISION",scope,yylineno);}
	| expr '/' expr				{$$ = Manage_arithmetic_expr($1,$3,mod_i,"MODULO",scope,yylineno);}
	| expr '>' expr				{$$ = NULL ;}
	| expr '<' expr				{$$ = NULL ;}
	| expr GE_OP expr			{$$ = NULL ;}
	| expr LE_OP expr			{$$ = NULL ;}
	| expr EQ_OP expr			{$$ = NULL ;}
	| expr NE_OP expr			{$$ = NULL ;}
	| expr AND expr				{$$ = NULL ;}
	| expr OR expr				{$$ = NULL ;}
	| term						{$$ = $1 ;}
	; 

term: '(' expr ')'{$$ = NULL;}
	| '-' expr	%prec UNARY_MINUS{$$ = NULL;}
	| NOT expr			{$$ = NULL;}
	| INC_OP lvalue 	{if($2 != NULL)eval_lvalue($2->sym,"++",yylineno);$$= NULL;}
	| lvalue INC_OP		{if($1 != NULL)eval_lvalue($1->sym,"++",yylineno);$$= NULL;}
	| DEC_OP lvalue		{if($2 != NULL)eval_lvalue($2->sym,"--",yylineno);$$= NULL;}
	| lvalue DEC_OP		{if($1 != NULL)eval_lvalue($1->sym,"--",yylineno);$$= NULL;}
	| primary			{$$ = $1;}
	;

assginexpr
	: lvalue '=' expr 	{if($1 != NULL && $3 != NULL) $$ = Manage_assignexpr($1,$3,scope,yylineno);}
	| error '=' expr 	{LOG_ERROR(PARSER,NOTE,"Wrong lvalue in assignment, line %u\n",yylineno); yyerrok;} 
	;

primary
	: lvalue
	{ 
    	$$ = emit_iftableitem($1);
	}  
	| call 					{$$ = NULL ;}
	| objectdef				{$$ = NULL ;}
	| '(' funcdef ')'		{$$ = NULL ;}
	| const					{$$ = $1;	}
	;

lvalue
	: ID	
	{
		$$ = lvalue_expr(Manage_lv_ID($1,yylineno, scope ,oScopeStack));
	}
	| LOC ID
	{
		$$ =  lvalue_expr(Manage_lv_local($2,yylineno,scope));
	}
	| DOUBLE_COL ID	
	{
		$$ = lvalue_expr(Manage_lv_global($2,yylineno));
	}
	| member	
	{
		$$ = $1;
	}
	;

member
	: call '.' ID	{$$ = NULL ;}
	| lvalue '.' ID
	{
		$$ = member_item($1,$3);
	}
	| call '[' expr ']'	{$$ = NULL ;}
	| lvalue '[' expr ']'
	{
		$1 = emit_iftableitem($1);
		$$ = newexpr(tableitem_e);
		$$->sym = $1->sym;
		$$->index = $3;
	}
	;

call: call '(' ')'
	{
		$$ = make_call($1, NULL);
	}
	| call '(' elist ')'
	{
		$$ = make_call($1, $3);
	}
	| '(' funcdef ')' '(' elist ')'
	{
		expr* func = newexpr(programfunc_e);
		func->sym = $2;
		$$ = make_call(func, $5);
	}
	| '(' funcdef ')' '('  ')'	
	{
		expr* func = newexpr(programfunc_e);
		func->sym = $2;
		$$ = make_call(func, NULL);
	}
	| lvalue callsuffix
	;

callsuffix
	: normcall			{$$ = $1;}
	| methodcall 		{$$ = $1;}
	;

normcall
	: '(' elist ')' { $$ = new_call($2,0,NULL); }
	| '(' ')'		{$$ = new_call(NULL,0,NULL);}
	;

methodcall
	: DOUBLE_DOT ID '(' error ')' 	{  yyerrok;} 
	| DOUBLE_DOT ID '(' elist ')' 	{ $$ = new_call($4,1,$2);}
	| DOUBLE_DOT ID '('  ')' 		{ $$ = new_call(NULL,1,$2); }
	; 

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
	:'{' { scope++; } 
	stmt_list 
	'}'
	{
		$$ = $3;
		ScopeTable_hide(scope);
		scope--;
	}
	|'{' { scope++; } '}'
	{
		/* alocate memory for a new stmt */
		$$ = new_stmt();
		ScopeTable_hide(scope);
		scope--;
	}
	;

funcstart: {
	$$ = curr_quad_label();
	emit(jump_i, NULL, NULL, NULL, yylineno, 0);
}

func_name
	: 		{ $$ =  func_name_generator(); }
	| ID	{ $$ = $1; }
	;

funcpref
	:	FUNC func_name 
	{
		$$ = Manage_func_pref($2,yylineno,scope,oScopeStack);
		set_i_address($$,next_quad_label());
		/* JUMP OUT OF FUNCTION SCOPE */
		emit(funcstart_i, lvalue_expr($$) , NULL, NULL,yylineno,0);
		
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

prebody:
	{
		$$ = curr_scope_offset();
	}
	;

funcbody
	: block 
	{
		$$ = $1;
		exitscopespace();
	}
	;

funcdef
	: funcstart funcpref funcargs prebody funcbody			
	{
		exitscopespace();
		set_total_locals($2,$4);

		IntStack_Pop(oScopeStack);
		restore_curr_scope_offset(IntStack_Pop(offsetStack));
		$$ = $2;

		patchlist($5->retlist,curr_quad_label());
		
		emit(funcend_i, lvalue_expr($$) , NULL, NULL,yylineno,0);
		
		/* JUMP OUT OF FUNCTION SCOPE */
		patchlabel($1,curr_quad_label());
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
loop_stmt:	loop_Inc stmt loop_End
{
	$$ = $2;
};

whilestmt
	: WHILE '(' expr ')' loop_stmt 	{		
		$$ = $5;
		patchlist($5->breaklist,curr_quad_label());
		patchlist($5->contlist,curr_quad_label());}
	| WHILE '(' error ')' loop_stmt {  yyerrok;} 
	;

forstmt
	: FOR '(' elist ';' expr ';' elist ')' loop_stmt	{
		$$ = $9;
		patchlist($9->breaklist,curr_quad_label());
		patchlist($9->contlist,curr_quad_label());
	}
	| FOR '(' elist ';' expr ';' ')' loop_stmt			{
		$$ = $8;
		patchlist($8->breaklist,curr_quad_label());
		patchlist($8->contlist,curr_quad_label());
	}
	| FOR '(' elist ';' error ';' elist')' loop_stmt	{  yyerrok;} 
	| FOR '(' elist ';' error ';' ')' loop_stmt			{  yyerrok;} 
	;

returnstmt
	: RET expr ';'	{$$ = Manage_ret_stmt(oScopeStack,yylineno,$2);}
	| RET ';'		{$$ = Manage_ret_stmt(oScopeStack,yylineno,NULL);}
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
	offsetStack = IntStack_init();

	expand();
	Tables_init();
	/* Call the Parser */
	yyparse();

	printf("%-30.100s\n",ERROR_COMP ? "COMPILATION ERROR ENCOUNTERD" : "ALL FINE");

	Tables_print(ost,0);
	
	if(ERROR_COMP == 0)
		write_quads();

	/* Close streams and clean up */
	Tables_free();
	IntStack_free(oScopeStack);

	fclose(ost);
	fclose(yyin);
	
	return 0;
}