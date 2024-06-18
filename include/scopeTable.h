/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* scopeTable.h		            							*/
/* 														    */
/* A simple interface for Scope Table module		        */
/*----------------------------------------------------------*/

#ifndef SCOPE_TABLE
#define SCOPE_TABLE 

#include <stdio.h>

#include <symTableEntry.h>

typedef struct scopeTable scopeTable, *ScopeTable_T;

/** 
* @brief Return a new empty ScopeTable_T
*
* @return Empty Symbol Table
*/
void ScopeTable_new(void);

/**
* @brief It frees the binded memory of the ScopeTable.
*
* @param free_entries Whether the entries will be freed.
*/
void ScopeTable_free(int free_entries);

/**
* @brief Inserts information to the Scope Table.
* 
* @param oSymEntry	Entry of the ScopeTable to be inserted
*
* @return	Whether the insertion was a SUCCESS or a FAILURE
*/
int ScopeTable_insert(SymEntry_T oSymEntry);

/**
* @brief Hide all Entries with this scope.
* 
* @param scope The scope of the Entries that we want to hide
*
* @return	Whether the opperation was a SUCCESS or a FAILURE
*/
int ScopeTable_hide(unsigned int scope);

/**
* @brief Prints all nodes of the Symbol table in order of scope and insertion.
* 
* @param ost The output stream used.
*
*/
void ScopeTable_print(FILE* ost);

/**
* @brief Checks if the ScopeTable is initialized or not.
* 
* @return	Whether 1 for yes, 0 for nope
*/
int ScopeTable_isInit(void);


#endif /* scope table module*/