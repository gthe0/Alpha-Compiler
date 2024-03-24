/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* scopeTable.h		            							*/
/* 														    */
/* A simple interface for Scope Table ADT			        */
/*----------------------------------------------------------*/

#ifndef __SCOPE_TABLE__
#define __SCOPE_TABLE__ 

#include <symTableEntry.h>

typedef struct scopeTable scopeTable, *ScopeTable_T;

/** 
* @brief Return a new empty ScopeTable_T
*
* @return Empty Symbol Table
*/
ScopeTable_T ScopeTable_new(void);

/**
* @brief It frees the binded memory of the ScopeTable.
*
* @param oScopeTable Symbol Table to be freed.
* @param free_entries Whether the entries will be freed.
*/
void ScopeTable_free(ScopeTable_T oScopeTable,int free_entries);

/**
* @brief Inserts information to the Scope Table.
* 
* @param oScopeTable	Pointer to the SymTable 
* @param oSymEntry	Entry of the ScopeTable to be inserted
*
* @return	Whether the insertion was a SUCCESS or a FAILURE
*/
int ScopeTable_insert(ScopeTable_T oScopeTable,
					SymEntry_T oSymEntry);

/**
* @brief Hide all Entries with this scope.
* 
* @param oScopeTable Pointer to the SymTable 
* @param scope The scope of the Entries that we want to hide
*
* @return	Whether the opperation was a SUCCESS or a FAILURE
*/
int ScopeTable_hide(ScopeTable_T oScopeTable,
					 unsigned int scope);

/**
* @brief Prints all nodes of the Symbol table in order of scope and insertion.
* 
* @param oSymTable	Pointer to the SymTable 
*
*/
void ScopeTable_print(ScopeTable_T oScopeTable); 

#endif /* scope table ADT*/