/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* expr.c			            							*/
/* 														    */
/* Implementation of the expression ADT						*/
/*----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

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