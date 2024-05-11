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

    #include <call.h>
    #include <stmt.h>
    #include <expr.h>
    #include <log.h>
    #include <tables.h>
    #include <forpref.h>
    #include <name_gen.h>
    #include <symTable.h>
    #include <scopeTable.h>
    #include <scopeSpace.h>
    #include <indexed_pair.h>
    #include <IntegerStack.h>
    #include <parser_utils.h>
    #include <symTableEntry.h>

    #if defined(WIN32) || defined(_WIN32_WCE)
    #define YY_NO_UNISTD_H
    static int isatty(int i) {return 0;}
    #endif

	#define YYDEBUG 1 /* Make 1 to enable debugging */

    /* The various stacks that we will use */
    static ScopeStack_T  	oScopeStack = NULL;
    static OffsetStack_T  	offsetStack = NULL;
    static LoopStack_T  	oloopStack	= NULL;

    /* If Compilation Error is
     encountered, it will be set to 1 */
    unsigned int ERROR_COMP = 0;

    /*Counter used to see in how many loops are we in */
    static unsigned int loop_counter = 0;

    /* The current scope */
    unsigned int scope = 0;

    /* Flex variables */
    extern char*	yytext;
    extern int 		yylineno;
    
    int yylex(void);
    int yyerror(const char* s);
%}

%define parse.error verbose
/* expect 1 shift/reduce warning (dangling else) */
%expect 1

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
    IndexPair_T index_pair_o;
    PairList_T pair_list_o;
    forpref_T forprefix_o;
}

%token <string> 		ID STRING 
%token <intVal> 		INT
%token <floatVal> 		FLOAT

%type <call_object>		callsuffix normcall methodcall
%type <entry> 			funcpref funcdef 
%type <string> 			func_name
%type <statement> 		whilestmt stmt_list stmt block loop_stmt forstmt returnstmt funcbody ifstmt
%type <expression>		const primary expr lvalue member term assginexpr elist call object_list objectdef
%type <unsignedVal> 	LQ NQ MQ funcstart whilecond whilestart ifprefix elseprefix
%type <pair_list_o>  	indexed
%type <index_pair_o>  	indexedelem
%type <forprefix_o>		forprefix

%destructor {free($$);} stmt

%token IF  ELSE  WHILE  FOR  FUNC  RET  BREAK  CONTINUE  
%token AND  NOT  OR
%token LOC  TRUE  FALSE  NIL 
%token EQ_OP  NE_OP  INC_OP  DEC_OP  GE_OP  LE_OP
%token DOUBLE_COL  DOUBLE_DOT
%token ',' ';' ':' '.' '=' '+' '-' '%' '*' '/'
%token '[' ']' '(' ')' '{' '}' '<' '>'

%right '='
%left OR
%left AND
%nonassoc EQ_OP  NE_OP
%nonassoc '<' '>' GE_OP  LE_OP
%left '+' '-'
%left '*' '/' '%'
%nonassoc UNARY_MINUS
%right NOT INC_OP DEC_OP
%left '.' DOUBLE_DOT
%left '[' ']'
%left '(' ')'


%precedence ELSE

%start program
%%
program
    : /* empty production, making stmt_list optional */
    | stmt_list /* stmt* means zero or more stmt */
    ;

stmt_list
    : stmt 				{ $$ = $1 ; }
    | stmt_list stmt 	{ $$ = Merge_stmt($1,$2); }
    ;
    
stmt: expr ';' 
    {
		short_circuit_eval($1,scope,yylineno);
        reset_temp();
        $$ = new_stmt();
    }
    | ifstmt
    {
        reset_temp();
        $$ = $1;
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

expr: assginexpr								{$$ = $1 ;}
    | expr '+' expr								{$$ = Manage_arithmetic_expr($1,$3,add_i,"addition",scope,yylineno);}
    | expr '-' expr								{$$ = Manage_arithmetic_expr($1,$3,sub_i,"subtraction",scope,yylineno);}
    | expr '*' expr								{$$ = Manage_arithmetic_expr($1,$3,mul_i,"multiplication",scope,yylineno);}
    | expr '%' expr								{$$ = Manage_arithmetic_expr($1,$3,mod_i,"modulo",scope,yylineno);}
    | expr '/' expr								{$$ = Manage_arithmetic_expr($1,$3,div_i,"division",scope,yylineno);}
    | expr '>' expr								{$$ = Manage_rel_expr($1,$3,if_greater_i,">",scope,yylineno) ;}
    | expr '<' expr								{$$ = Manage_rel_expr($1,$3,if_less_i,"<",scope,yylineno) ;}
    | expr GE_OP expr							{$$ = Manage_rel_expr($1,$3,if_greatereq_i,">=",scope,yylineno) ;}
    | expr LE_OP expr							{$$ = Manage_rel_expr($1,$3,if_lesseq_i,"<=",scope,yylineno) ;}
    | expr EQ_OP								{short_circuit_eval($1,scope,yylineno);}
	 expr										{$$ = Manage_eq_expr($1,$4,if_eq_i,"==",scope,yylineno) ;}
    | expr NE_OP								{short_circuit_eval($1,scope,yylineno);}
     expr										{$$ = Manage_eq_expr($1,$4,if_noteq_i,"!=",scope,yylineno) ;}
    | expr AND 									{$1 = boolean_create($1,scope,yylineno);}  
	LQ expr										{$5 = boolean_create($5,scope,yylineno);
                                                 $$ = Manage_conjunctions($1,$5,and_i,$4,scope,yylineno) ;}
    | expr OR 									{$1 = boolean_create($1,scope,yylineno);}
	 LQ	expr 									{$5 = boolean_create($5,scope,yylineno);
                                                 $$ = Manage_conjunctions($1,$5,or_i,$4,scope,yylineno) ;}
    | term										{$$ = $1 ;}
    ; 		

term: '(' expr ')'								{$$ = $2;}
    | '-' expr	%prec UNARY_MINUS				{$$ = Manage_unary_minus($2,scope,yylineno);}
    | NOT expr									{$$ = Manage_not_expr($2,scope,yylineno);}
    | INC_OP lvalue 							{$$ = Manage_lv_arithmetic_left($2,add_i,"left ++",scope,yylineno);}
    | lvalue INC_OP								{$$ = Manage_lv_arithmetic_right($1,add_i,"right ++",scope,yylineno);}
    | DEC_OP lvalue								{$$ = Manage_lv_arithmetic_left($2,sub_i,"left --",scope,yylineno);}
    | lvalue DEC_OP								{$$ = Manage_lv_arithmetic_right($1,sub_i,"right --",scope,yylineno);}
    | primary									{$$ = $1;}
    ;		

assginexpr		
    : lvalue '=' expr 							{$$ = Manage_assignexpr($1,$3,scope,yylineno);}
    | error '=' expr 							{LOG_ERROR(PARSER,NOTE,"Wrong lvalue in assignment, line %u\n",yylineno); yyerrok;} 
    ;		

primary		
    : lvalue									{$$ = emit_iftableitem($1);}  
    | call 										{$$ = emit_iftableitem($1);}
    | objectdef									{$$ = $1 ;}
    | '(' funcdef ')'							{$$ = newexpr(programfunc_e); $$->sym = $2 ;}
    | const										{$$ = $1;}
    ;		

lvalue		
    : ID										{ $$ = lvalue_expr(Manage_lv_ID($1,yylineno, scope ,oScopeStack)); }
    | LOC ID 									{ $$ = lvalue_expr(Manage_lv_local($2,yylineno,scope)); }
    | DOUBLE_COL ID								{ $$ = lvalue_expr(Manage_lv_global($2,yylineno)); }
    | member 									{ $$ = $1;	}
    ;		

member		
    : call '.' ID								{$$ = member_item($1,$3) ;}
    | lvalue '.' ID								{$$ = member_item($1,$3) ;}
    | call '[' expr ']'							{$$ = Manage_member($1,$3) ;}
    | lvalue '[' expr ']'						{$$ = Manage_member($1,$3) ;}
    ;		

call: call '(' ')'								{$$ = make_call($1, NULL);}
    | call '(' elist ')'						{$$ = make_call($1, $3);}
    | lvalue callsuffix 						{$$ = Manage_call_lv_suffix($1,$2);}
    | '(' funcdef ')' '(' elist ')'				{
                                                    expr* func = newexpr(programfunc_e);
                                                    func->sym = $2;
                                                    $$ = make_call(func, $5);
                                                }
    | '(' funcdef ')' '('  ')'					{
                                                    expr* func = newexpr(programfunc_e);
                                                    func->sym = $2;
                                                    $$ = make_call(func, NULL);
                                                }
    ;		

callsuffix		
    : normcall									{$$ = $1;}
    | methodcall 								{$$ = $1;}
    ;		

normcall		
    : '(' elist ')' 							{$$ = new_call($2,0,NULL);}
    | '(' ')'									{$$ = new_call(NULL,0,NULL);}
    ;		

methodcall		
    : DOUBLE_DOT ID '(' error ')' 				{yyerrok;} 
    | DOUBLE_DOT ID '(' elist ')' 				{ $$ = new_call($4,1,$2);}
    | DOUBLE_DOT ID '('  ')' 					{ $$ = new_call(NULL,1,$2); }
    ; 		

elist											
    : expr 										{
													/* Most rules need the elist reversed,
													only object_list needs it in order of parsing...*/
													$1->next = NULL;
													$$ = $1;
													short_circuit_eval($1,scope,yylineno);
												} 
    | elist ',' expr							{
													/* ...so we insert new expression in the head of the list, 
													making it reversed from the start */
													$3->next = $1;
													$$ = $3;
													short_circuit_eval($3,scope,yylineno);
												}
    | elist ',' error							{ yyerrok;} 
    ;		

objectdef		
    :	'[' object_list ']'						{$$ = $2;}
    |	'[' error ']'							{ yyerrok;} 
    ;		

object_list		
    : elist										{$$ = Manage_obj_elist($1,scope,yylineno);}
    | indexed									{$$ = Manage_obj_indexed($1,scope,yylineno);}
    |											{$$ = Manage_obj_indexed(NULL,scope,yylineno);}
    ;		

indexed		
    : indexed ',' indexedelem					{$$ = PairList_insert($3,$1);}
    | indexedelem								{$$ = PairList_insert($1,NULL);}
    ;

indexedelem
    : '{' expr  ':' expr '}'					{$$ = new_indexed_pair($2,$4,scope,yylineno);}
    | '{' error ':' expr '}'					{ yyerrok;} 
    | '{' expr  ':' error '}'					{ yyerrok;} 
    | '{' expr error expr '}'					{ yyerrok;} 
    ;		

NQ: 											{ $$ = curr_scope_offset(); };
LQ: 											{ $$ = curr_quad_label(); };
MQ:												{ 
                                                    $$ = curr_quad_label(); 
                                                    emit(jump_i,NULL,NULL,NULL,yylineno,0);
                                                };

block
    :'{'	{ scope++; } stmt_list '}'			{
                                                    $$ = $3;
                                                    ScopeTable_hide(scope);
                                                    scope--;
                                                }
    
    |'{'	{ scope++; }			'}'			{
                                                    /* alocate memory for a new stmt */
                                                    $$ = new_stmt();
                                                    ScopeTable_hide(scope);
                                                    scope--;
                                                }
    ;

funcstart: 										{
                                                    $$ = curr_quad_label();
                                                    emit(jump_i, NULL, NULL, NULL, yylineno, 0);
													
													/*
													* Reset loop counter in function definitions
													* to prevent illegal breaks and continues
													*/
													IntStack_Push(&oloopStack,loop_counter);
													loop_counter = 0;
                                                }

func_name
    : 											{ $$ =  func_name_generator(); }
    | ID										{ $$ = $1; }
    ;

funcpref
    :	FUNC func_name 							{
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
    : '(' {scope++;} idlist ')'					{
                                                    resetfunctionlocaloffset();
                                                    enterscopespace(); 
                                                    scope--;
                                                }
    ;


funcbody
    : block 									{
                                                    $$ = $1;
                                                    exitscopespace();
                                                }
    ;

funcdef
    : funcstart funcpref funcargs NQ funcbody	{
                                                    exitscopespace();
                                                    set_total_locals($2,$4);

                                                    IntStack_Pop(oScopeStack);
                                                    restore_curr_scope_offset(IntStack_Pop(offsetStack));
                                                    $$ = $2;

                                                    patchlist($5->retlist,curr_quad_label());

                                                    emit(funcend_i, lvalue_expr($$) , NULL, NULL,yylineno,0);

                                                    /* JUMP OUT OF FUNCTION SCOPE */
                                                    patchlabel($1,curr_quad_label());
													loop_counter = IntStack_Pop(oloopStack);
                                                }
    ;

const
    : INT 										{ $$ = new_num_expr($1); }
    | FLOAT 									{ $$ = new_num_expr($1); }
    | NIL 										{ $$ = new_nil_expr(  ); }
    | TRUE 										{ $$ = new_bool_expr(1); }
    | FALSE 									{ $$ = new_bool_expr(0); }
    | STRING 									{ $$ = new_string_expr($1);}
    ;

idlist
    :
    | ID										{
                                                    if((Valid_args($1,yylineno,scope, oScopeStack))== EXIT_SUCCESS)
                                                        Tables_insert(FORMAL,$1,scope,yylineno);
                                                }
    | idlist ',' ID								{		
                                                    if((Valid_args($3,yylineno,scope, oScopeStack))== EXIT_SUCCESS)
                                                        Tables_insert(FORMAL,$3,scope,yylineno);
                                                }
    ;

ifprefix
    :  IF '(' expr ')' 							{$$ = Manage_cond($3,scope,yylineno);}
    |  IF '(' error ')'							{yyerrok;} 
    ;

elseprefix
    : ELSE 										{ 
                                                    $$ = curr_quad_label();
                                                     emit(jump_i,NULL,NULL,NULL,yylineno,0);
                                                }
    ;

ifstmt
    :  ifprefix stmt 							{patchlabel($1,curr_quad_label()); $$ = $2;}
    |  ifprefix  stmt elseprefix stmt			{$$ = Manage_if_else($1,$2,$3,$4, yylineno);}
    ;		

loop_Inc:										{loop_counter++;};
loop_End:										{loop_counter--;};
loop_stmt:	loop_Inc stmt loop_End 				{ $$ = $2; };

whilestart: WHILE 								{ $$ = curr_quad_label(); };
whilecond: 	'(' expr ')'						{ $$ = Manage_cond($2,scope,yylineno); }				
        |	'(' error ')'						{yyerrok;}
        ;

whilestmt: whilestart whilecond loop_stmt 		{ $$ = Manage_while_stmt($1,$2,$3, yylineno);}
    ;

forprefix: FOR '(' elist ';' LQ expr ';'		{$$ = Manage_forpref($5,$6,scope,yylineno);}
| FOR '(' elist ';'LQ error ';'					{  yyerrok;} 
;

forstmt
    : forprefix MQ elist ')' MQ loop_stmt MQ	{$$ = Manage_for_stmt($1, $2, $5, $6, $7);}
    | forprefix MQ ')' MQ loop_stmt	MQ 			{$$ = Manage_for_stmt($1, $2, $4, $5, $6);}
    ;

returnstmt
    : RET expr ';'	{$$ = Manage_ret_stmt(oScopeStack,scope,yylineno,$2);}
    | RET ';'		{$$ = Manage_ret_stmt(oScopeStack,scope,yylineno,NULL);}
    ;
%%
/* Same as lex */

/*
* Parser error function, evoked every time
* there is a parsing error
*/
int yyerror(const char* s)
{
    LOG_ERROR(PARSER,ERROR,"%s, line %d\n",s ,yylineno);
    return EXIT_FAILURE;
}

/* Initializes the stacks */
void parser_stacks_init(void)
{
	oScopeStack = IntStack_init();
	offsetStack = IntStack_init();
	oloopStack	= IntStack_init();
}

/* Frees the stacks */
void parser_stacks_free(void)
{
    IntStack_free(oScopeStack);
    IntStack_free(offsetStack);
    IntStack_free(oloopStack);
}