/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* yacc_lib.h		            							*/
/* 														    */
/* A library with utility functions used in parser.y		*/
/*----------------------------------------------------------*/

#ifndef __YACC_LIB_H__
#define __YACC_LIB_H__

#include <symTable.h>
#include <ScopeStack.h>

int lvalue_local(SymTable_T oSymTable,char* name,int scope);
int lvalue_global(SymTable_T oSymTable,char* name);


/**
* @brief This Functions checks if the return statement is valid
* 
* @param stack The Scope Stack
* @param yylineno The line that we found the token
*
* @return EXIT_FAILURE or EXIT_SUCCESS 
*/
int Valid_return(ScopeStack_T stack, unsigned int yylineno);

/**
* @brief This Functions checks if the loop token is valid
* 
* @param name The name of the token
* @param loop_counter A counter to see if we are inside a loop. 
* @param yylineno The line that we found the token
*
* @return EXIT_FAILURE or EXIT_SUCCESS 
*/
int Valid_loop_token(char *name, int loop_counter, unsigned int yylineno);

/**
* @brief This Functions checks if the Function Definition is valid
* 
* @param oSymTable The oSymTable (Used to lookup for the Symbol)
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The current Scope 
* @param stack The Scope Stack
*
* @return EXIT_FAILURE or EXIT_SUCCESS 
*/
int lvalue_Function(SymTable_T oSymTable,char* name,
					unsigned int line, unsigned int FromScope,
					ScopeStack_T stack);


/**
* @brief This Functions generates a name for Functions with no name defined
* 
* @return a name with illegal characters so that it will be unique
*/
char* func_name_generator();

#endif /* Yacc lib */