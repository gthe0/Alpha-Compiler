/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* yacc_lib.h		            							*/
/* 														    */
/* A library with utility functions used in parser.y		*/
/*----------------------------------------------------------*/

#ifndef __YACC_LIB_H__
#define __YACC_LIB_H__

#include <tables.h>
#include <ScopeStack.h>

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
int Valid_Function(SymTable_T oSymTable,char* name,
					unsigned int line, unsigned int FromScope,
					ScopeStack_T stack);

/**
* @brief Check if the locals are valid
* 
* @param oSymTable The oSymTable (Used to lookup for the Symbol)
* @param name The name of the token
* @param line The line that we found the token
* @param scope The current Scope 
*
* @return the token if found or else NULL 
*/
SymEntry_T Valid_local(SymTable_T oSymTable,char* name,
					unsigned int line, unsigned int scope);


/**
* @brief Check if the Arguments are valid
* 
* @param oSymTable The oSymTable (Used to lookup for the Symbol)
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The current Scope 
* @param stack The Scope Stack
*
* @return EXIT_FAILURE or EXIT_SUCCESS 
*/
int Valid_args(SymTable_T oSymTable,char* name,
					unsigned int line, unsigned int FromScope,
					ScopeStack_T stack);

/* Function to check if the globals exists */
SymEntry_T Valid_lvalue_ID(SymTable_T oSymTable,ScopeTable_T oScopeTable, char *name,
					unsigned int line, unsigned int FromScope,
					ScopeStack_T stack);
/**
* @brief Check if the global token exists
* 
* @param oSymTable The oSymTable (Used to lookup for the Symbol)
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The current Scope 
* @param stack The Scope Stack
*
* @return The token if found or else return NULL if not found 
*/
SymEntry_T find_global(SymTable_T oSymTable, char *name,
					unsigned int line);

int eval_lvalue(SymEntry_T entry,char* operation, int yylineno);

/**
* @brief This Functions generates a name for Functions with no name defined
* 
* @return a name with illegal characters so that it will be unique
*/
char* func_name_generator();

#endif /* Yacc lib */