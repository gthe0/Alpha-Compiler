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
	assert(oSymEntry);

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
static unsigned int istempname(char* s)
{
	return *s == '_' ;
}

/* Checks if the expression is temporary */
unsigned int is_temp_expr(expr* e)
{
	assert(e->sym->type < USERFUNC);
	return e->sym && istempname(e->sym->value.varVal->name);
}
