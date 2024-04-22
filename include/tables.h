/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* tables.h			            							*/
/* 														    */
/* Wrapper Functions/APIs for the scope and symbol tables	*/
/*----------------------------------------------------------*/

#ifndef __TABLES_H__
#define __TABLES_H__

#include <symTableEntry.h>


/**
* @brief A wrapper function used to Create and Insert Entries in both tables
* 
* @param entry The entry to be inserted
*
* @return EXIT_SUCCESS or EXIT_FAILURE
*/
int Tables_insert_Entry(SymEntry_T entry);
/**
* @brief A wrapper function used to Create and Insert Entries in both tables
*
* @param type 	The type of the Entry
* @param name 	The name of the Entry
* @param scope 	The scope of the Entry
* @param line 	The line of the Entry
*
* @return EXIT_SUCCESS or EXIT_FAILURE
*/
int Tables_insert( SymbolType type,
					const char* name,
					unsigned int scope,
					unsigned int yylineno);

/**
* @brief A wrapper function used to free both tables
*/
void Tables_free(void);

/**
* @brief A wrapper function used to Initiate both tables
* 
* @return EXIT_SUCCESS or EXIT_FAILURE
*/
int Tables_init(void);

/**
* @brief Choose to either use the Symbol Table print or the Scope Table one. 
* 
* @param ost			The output stream 
* @param option			true for SymTable print, false for ScopeTable print
*
*/
void Tables_print(FILE* ost, bool option);


/**
* @brief Choose to either use the Symbol Table print or the Scope Table one. 
* 
* @param scope 	The scope of the Entry
* @param yylineno 	The line of the Entry
*
*/
SymEntry_T newtemp(int scope,
					unsigned yylineno);

#endif /* wrapper */