/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* forpref.c		            							*/
/* 														    */
/* Implementation of the forpref ADT and its functions		*/
/*----------------------------------------------------------*/

#include <quad.h>
#include <forpref.h>

#include <stdlib.h>
#include <assert.h>

forpref_T Manage_forpref(unsigned test,
						expr* e,
						unsigned scope,
						unsigned yylineno)
{
	short_circuit_eval(e,scope,yylineno);

	forpref_T new_pref = malloc(sizeof(forpref_T));
	assert(new_pref);

	new_pref->test = test;
	new_pref->enter = curr_quad_label();

	emit(if_eq_i, e, new_bool_expr(1), NULL, yylineno, 0);

	return new_pref;
}
