/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* expr.c			            							*/
/* 														    */
/* Implementation of the expression ADT						*/
/*----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <expr.h>
#include <stmt.h>
#include <quad.h>
#include <log.h>
#include <tables.h>

/* Creates a new expression */
expr *newexpr(expr_t t)
{
    expr *e = (expr *)malloc(sizeof(expr));
    memset(e, 0, sizeof(expr));

    e->type = t;
    return e;
}


/*Creates a new string expression*/
expr* new_string_expr(char* string)
{
	expr* e = newexpr(conststring_e);
	e->strConst = string;
	return e;
}


/*Creates a new nil expression*/
expr* new_nil_expr(void)
{
	return newexpr(nil_e);
}


/*Creates a new boolean expression*/
expr* new_bool_expr(bool boolConst)
{
	expr* e = newexpr(boolexpr_e);
	e->boolConst = boolConst;
	return e;
}


/*Creates a new number expression*/
expr* new_num_expr(double numConst)
{
	expr* e = newexpr(constnum_e);
	e->numConst = numConst;
	return e;
}


/* lvalue expression generator */
expr* lvalue_expr(SymEntry_T oSymEntry)
{
	if(!oSymEntry) return NULL ;

    expr* e = (expr*)malloc(sizeof(expr));
    memset(e, 0, sizeof(expr));

    e->next = NULL;
    e->sym = oSymEntry;

	if(oSymEntry->type < USERFUNC)
		e->type = var_e;
	else if(oSymEntry->type == USERFUNC)
		e->type = programfunc_e;
	else if(oSymEntry->type == LIBFUNC)
		e->type = libraryfunc_e;

	return e;
}

/* Checks whether the name is temporary or not */
static unsigned int istempname(const char* s)
{
	return *s == '_' ;
}

/* Checks if the expression is temporary */
unsigned int is_temp_expr(expr* e)
{
	assert(e->sym->type < USERFUNC);
	return e->sym && istempname(e->sym->value.varVal->name);
}

/* Checks whether arithmetic is valid or not */
void check_arith (expr* e, const char* context)
{

	if(				
		e->type == constbool_e		|| 
		e->type == conststring_e	||
		e->type == programfunc_e	||
		e->type == libraryfunc_e	||
		e->type == newtable_e		||
		e->type == boolexpr_e		||
		e->type == nil_e
	)
	LOG_ERROR(PARSER,ERROR,"Illegal expr used in %s!",context);	

	return ;
}

/* short_circuit boolean logic */
void short_circuit(expr* e,unsigned yylineno)
{
	assert(e);

	if(e->type != boolexpr_e)
		return ;

	patchlist(e->true_list,curr_quad_label());

	emit(assign_i,new_bool_expr(1),NULL,e,yylineno,0);
	emit(jump_i,NULL,NULL,NULL,yylineno,next_quad_label()+1);

	patchlist(e->false_list,curr_quad_label());

	emit(assign_i,new_bool_expr(0),NULL,e,yylineno,0);
}


/* Creates a boolean expression out of another */
expr* make_bool_expr(expr* e, 
					unsigned scope,
					unsigned yylineno)
{
	assert(e);

	if(e->type == boolexpr_e)
		return e;


	expr* bool_e = newexpr(boolexpr_e);
	bool_e->sym = newtemp(scope,yylineno);


	bool_e->true_list = curr_quad_label();
	emit(if_eq_i, e, new_bool_expr(1), NULL, yylineno, 0);
	
	
	bool_e->false_list = curr_quad_label();
	emit(jump_i, NULL, NULL, NULL, yylineno, 0);


	return bool_e ;
}