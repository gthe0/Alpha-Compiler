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
#include <symTable.h>
#include <scopeTable.h>

/**
* @brief A wrapper function used to Create and Insert Entries in both tables
* 
* @param oSymTable The Symbol table
* @param oScopeTable The Scope table
* @param type 	The type of the Entry
* @param name 	The name of the Entry
* @param scope 	The scope of the Entry
* @param line 	The line of the Entry
*
* @return EXIT_SUCCESS or EXIT_FAILURE
*/
int Tables_insert(	SymTable_T oSymTable,
					ScopeTable_T oScopeTable,
					SymbolType type,
					const char* name,
					unsigned int scope,
					unsigned int yylineno);

/**
* @brief A wrapper function used to free both tables
* 
* @param oSymTable The Symbol table
* @param oScopeTable The Scope table
*
*/
void Tables_free(SymTable_T oSymTable,
				ScopeTable_T oScopeTable);

/**
* @brief A wrapper function used to Initiate both tables
* 
* @param oSymTable The Symbol table
* @param oScopeTable The Scope table
*
* @return EXIT_SUCCESS or EXIT_FAILURE
*/
int Tables_init(SymTable_T* oSymTable,
				ScopeTable_T* oScopeTable);

#endif /* wrapper */