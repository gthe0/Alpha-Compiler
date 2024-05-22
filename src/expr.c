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
#include <utils.h>
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

/*
* We do not need to create a new symbol due to the partial evaluation and variable reusage.
* For example, if we have a big boolean/propositional expression such as,
* a or b or c and not d etc. a,b,c and d will be turned into boolean expressions,
* pairs of ifeq and jumps will be emited with their args being a,b,c or d and only
* at the very end the results will be assigned to a temp variable _tx. 
* Thus, we do not want to create temp variables for a,b,c or d at all!
*/
expr* make_bool_expr(unsigned scope,
					unsigned yylineno)
{

	expr* bool_e = newexpr(boolexpr_e);

	bool_e->true_list = curr_quad_label();
	bool_e->false_list = next_quad_label();

	return bool_e;
}

/*
* Whenever an expression is encountered that can be evaluated as a boolean,
* we need to emit some instructions and store somewhere their quads for shortcircuiting.
* We do that by making a new boolexpr_e that acts as an accecptance flag in short_circuit_eval.
*/
expr* boolean_create(expr* e, 
					unsigned scope,
					unsigned yylineno)
{
	assert(e);

	/* If the type is boolexpr_e, then we do not need to
	* do anything because the needed instructions needed 
	* and its truth lists were already created.
	*/
	if(e->type == boolexpr_e)
		return e;

	/*
	* Create a new boolean expression with its:
	*	# truth_list == quad of if_eq_i
	*	# false_list == quaf of jump
	*/
	expr* bool_e = make_bool_expr(scope,yylineno);

	emit(if_eq_i, e, new_bool_expr(1), NULL, yylineno, 0);
	emit(jump_i, NULL, NULL, NULL, yylineno, 0);

	return bool_e ;
}

/*
* Here we patch the true lists and flase lists.
* We also create a new temporary symbol to use
* as a result storage for when we emit the quads that we need.
*/
void short_circuit_eval(expr* e, 
						unsigned scope,
						unsigned yylineno)
{
	if(!e || e->type != boolexpr_e)
		return ;

	/*
	* Useless check but I do not want to remove it.
	* Let BTP take care of it...
	*/
	if(!e->sym) e->sym = newtemp(scope,yylineno);

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
			return add_quotes(e->strConst); 
		case nil_e:
			return strdup("nil");
		default:
			return NULL;
	}

	return NULL;
}