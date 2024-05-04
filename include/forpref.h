/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* forpref.h		            							*/
/* 														    */
/* An ADT used to store the for test/body entry point info	*/
/*----------------------------------------------------------*/

#ifndef __FORPREF__
#define __FORPREF__

#include <expr.h>

/* typedefs of the forpref struct */
typedef struct forprefix forprefix, *forpref_T;

/* The forprefix struct */
struct forprefix
{
	unsigned int test;
	unsigned int enter;
};


/**
* @brief Creates a new, filled forprefix and emits a needed jump.
*
* @param test The label/quad address of the testing/conditional code.
* @param e The expression to be emited
* @param yylineno The line at which the for statement was found/started 
*
* @return The newly allocated and filled forprefix struct
*/
forpref_T Manage_forpref(unsigned test,
						expr* e,
						unsigned scope,
						unsigned yylineno);

#endif /*call ADT*/