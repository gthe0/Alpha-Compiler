/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* symTable.h		            							*/
/* 														    */
/* A simple interface for a Symbol Table module		        */
/* used to store syntax analyzer token information			*/
/*----------------------------------------------------------*/

#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__ 

#include <stdio.h>

#include <symTableEntry.h>

typedef struct 	SymTable SymTable, *SymTable_T;

/** 
* @brief Creates a new empty SymTable_T
*/
void SymTable_new(void);

/**
* @brief It frees the binded memory of the SymTalbe .
*/
void SymTable_free(void);

/**
* @brief Inserts information to the Symbol Table.
* 
* @param oSymTable	Pointer to the SymTable 
* @param oSymEntry	Entry of the SymTable to be inserted
*
* @return	Whether the insertion was a SUCCESS or a FAILURE
*/
int SymTable_insert(SymEntry_T oSymEntry);


/**
 * @brief Searches for an entry in the Symbol Table between two scopes
 * 
 * @param name		name of Entry
 * @param FromScope	The Scope that we are currently at
 * @param ToScope	The Scope up to which we search
 * 
 * @return The Entry if it was found, otherwise NULL
*/
SymEntry_T SymTable_lookup(const char *name, 
					 int FromScope,
					 int ToScope);

/**
 * @brief Searches for a specific entry in the Symbol Table in a specific scope
 * 
 * @param name		Key to be searched
 * @param scope		Scope of the key to be searched
 * 
 * @return The Entry if it was found, otherwise NULL
*/
SymEntry_T SymTable_lookup_scope(const char *name, int scope);


/**
* @brief Checks if the SymTable is initialized or not.
* 
* @return	Whether 1 for yes, 0 for nope
*/
int SymTable_isInit(void);

/**
* @brief Prints all nodes of the Symbol table in random order.
* 
* @param oSymTable	Pointer to the SymTable 
* @param ost The output stream used.
*
*/
void SymTable_print(FILE* ost);

#endif  /* symTable module*/