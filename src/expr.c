/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* expr.c			            							*/
/* 														    */
/* Implementation of the expression ADT						*/
/*----------------------------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <expr.h>
#include <stmt.h>
#include <quad.h>
#include <log.h>
#include <tables.h>

#define MAX_DOUBLE_LENGTH 25

/**
* @brief This function turns doubles into strings, removing trailing zeroes 
* 
* @param num The double to be turned to string 
* 
* @return The generated string 
*/
static char* double_to_string(double num)
{
	char string_num[MAX_DOUBLE_LENGTH];
	sprintf(string_num, "%lf", num);

	int is_decimal = 0;
	char* head = string_num;
	
	/* If it is a float remove trailing decimals */
	while(*head != '\0')
		if(*head++ == '.')
			is_decimal = 1;
	
	/* Go inside the string */
	head-- ;

	/* Do not overextend */
	while (is_decimal && *head == '0' && head != string_num)
		head-- ;

	if(is_decimal)
		if(*head == '.')
			*head = '\0';
		else if(*head != '0' && head != string_num)
			*++head = '\0';
	
	char *generated_string = malloc((strlen(string_num)) * sizeof(char) + 1);
	assert(generated_string);

	strcpy(generated_string, string_num);

	return generated_string;
}


/**
* @brief Gets strConst and adds ""
* 
* @param e The expression 
* 
* @return "strConst"
*/
static char* get_stringConst(expr* e)
{
	char* strConst = malloc(strlen(e->strConst)*sizeof(char)+3);
	sprintf(strConst,"\"%s\"",e->strConst);
	return strConst;
}


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
expr* new_bool_expr(unsigned char boolConst)
{
	expr* e = newexpr(constbool_e);
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
	return 	e && e->sym && 
			e->sym->type < USERFUNC &&
			istempname(e->sym->value.varVal->name);
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
	LOG_ERROR(PARSER,ERROR,"Illegal expr used in %s!\n",context);	

	return ;
}

/* Create a blank boolean expression */
expr* make_bool_expr(unsigned scope,
					unsigned yylineno)
{

	expr* bool_e = newexpr(boolexpr_e);
	bool_e->sym = newtemp(scope,yylineno);
	bool_e->true_list = curr_quad_label();
	bool_e->false_list = next_quad_label();

	return bool_e;
}

/* Emit the instructions and create a boolean expression */
expr* boolean_create(expr* e, 
					unsigned scope,
					unsigned yylineno)
{
	assert(e);

	if(e->type == boolexpr_e)
		return e;

	expr* bool_e = make_bool_expr(scope,yylineno);

	emit(if_eq_i, e, new_bool_expr(1), NULL, yylineno, 0);
	emit(jump_i, NULL, NULL, NULL, yylineno, 0);

	return bool_e ;
}

/* Short circuit logic evaluation */
void short_circuit_eval(expr* e, 
						unsigned scope,
						unsigned yylineno)
{
	if(!e || e->type != boolexpr_e)
		return ;

	patchlist(e->true_list, curr_quad_label());
	patchlist(e->false_list, curr_quad_label()+2);

    emit(assign_i, new_bool_expr(1), NULL, e, yylineno, 0);
    emit(jump_i, NULL, NULL, NULL, yylineno, next_quad_label() + 1);
    
    emit(assign_i, new_bool_expr(0), NULL, e, yylineno,0);

	return ;
}

/* Used to decode the expressions and return their strings */
const char* expr_decode(expr* e)
{
	if(e == NULL)
		return NULL;

	switch(e->type)
	{
		case var_e:
		case tableitem_e:
		case programfunc_e:
		case libraryfunc_e:
		case arithexpr_e:
		case boolexpr_e:
		case assignexpr_e:
		case newtable_e:
			return e->sym ? getName(e->sym) : NULL;
		case constnum_e:
			return double_to_string(e->numConst);
		case constbool_e:
			return e->boolConst ? strdup("true") : strdup("false");
		case conststring_e:
			return get_stringConst(e); 
		case nil_e:
			return "nil";
		default:
			return NULL;
	}

	return NULL;
}