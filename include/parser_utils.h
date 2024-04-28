/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* parser_utils.h		            						*/
/* 														    */
/* A library with utility functions used in parser.y		*/
/*----------------------------------------------------------*/

#ifndef __YACC_LIB_H__
#define __YACC_LIB_H__

#include <tables.h>
#include <stmt.h>
#include <quad.h>
#include <IntegerStack.h>


/**
* @brief Check if the Arguments are valid
* 
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The current Scope 
* @param stack The Scope Stack
*
* @return EXIT_FAILURE or EXIT_SUCCESS 
*/
int Valid_args(char* name, unsigned int line,
				 unsigned int FromScope, ScopeStack_T stack);

int eval_lvalue(SymEntry_T entry,char* operation, int yylineno);

/**
* @brief Manages break and continue rules
* 
* @param name The name of the token
* @param loop_counter A counter to see if we are inside a loop. 
* @param yylineno The line that we found the token
*
* @return stmt pointer 
*/
stmt_T Manage_loop_token(char *name,
						 int loop_counter,
						 unsigned int yylineno);

/**
* @brief Manages the Local L-value
* 
* @param name The name of the token
* @param yylineno The line that we found the token
* @param scope The scope of the Entry 
*
* @return Return the entry
*/
SymEntry_T Manage_lv_local (char *name,
						 		unsigned  yylineno,
								int scope);

/**
* @brief Manages the ID L-value
* 
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The scope of the Entry 
* @param stack The scope stack of the Entry 
*
* @return Return the entry or a new one
*/
SymEntry_T Manage_lv_ID(char *name, unsigned int line,
 							unsigned int FromScope,ScopeStack_T stack);

/**
* @brief Check if the global token exists
* 
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The current Scope 
* @param stack The Scope Stack
*
* @return The token if found or else return NULL if not found 
*/
SymEntry_T Manage_lv_global(char *name,unsigned int line);

/**
* @brief This Functions checks if the Function Definition is valid
* 
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The current Scope 
* @param stack The Scope Stack
*
* @return The function symbol 
*/
SymEntry_T Manage_func_pref(char *name, unsigned int line,
					 unsigned int FromScope, ScopeStack_T stack);

/**
* @brief This Functions checks Manages the anonymous function definition
* 
* @param scope The current Scope 
* @param yylineno The line that we found the token
*
* @return The function symbol 
*/
char* Manage_func_name_anonymous(int scope,unsigned yylineno);


/**
* @brief This Functions checks Manages the anonymous function definition
*
* @param arg1 The first argument of the expression
* @param arg2 The second argument of the expression
* @param op	The opcdode of the instruction
* @param context The context in which it was used. 
* @param scope The current Scope 
* @param yylineno The line that we found the token
*
* @return The function symbol 
*/
expr* Manage_arithmetic_expr(expr* arg1, expr* arg2,
							 iopcode op, char* context,
							 unsigned scope, unsigned yylineno);


/**
* @brief This function manages the return statement
* 
* @param stack The Scope Stack
* @param yylineno The line that we found the token
* @param e	The expression that the function returns
*
* @return the stmt pointer 
*/
stmt_T Manage_ret_stmt(ScopeStack_T stack, unsigned yylineno, expr* e);

/**
* @brief This function manages the return statement
* 
* @param stack The Scope Stack
* @param yylineno The line that we found the token
* @param e	The expression that the function returns
*
* @return the stmt pointer 
*/
expr* Manage_assignexpr(expr* lvalue, expr* rvalue,
						unsigned int scope,
						 unsigned int yylineno);
#endif /* parser utilities */