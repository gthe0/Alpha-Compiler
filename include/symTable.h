/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* symTable.h		            							*/
/* 														    */
/* A simple interface for a Symbol Table ADT		        */
/* used to store syntax analyzer token information			*/
/*----------------------------------------------------------*/

#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__ 

#include <symTableEntry.h>

typedef struct 	SymTable SymTable, *SymTable_T;

/** 
* @brief Return a new empty SymTable_T
*
* @return Empty Symbol Table
*/
SymTable_T SymTable_new(void);

/**
* @brief It frees the binded memory of the SymTalbe .
*
* @param oSymTable Symbol Table to be freed.
*/
void SymTable_free(SymTable_T oSymTable);

/**
* @brief Inserts information to the Symbol Table.
* 
* @param oSymTable	Pointer to the SymTable 
* @param oSymEntry	Entry of the SymTable to be inserted
*
* @return	Whether the insertion was a SUCCESS or a FAILURE
*/
int SymTable_insert(SymTable_T oSymTable,
					SymEntry_T oSymEntry);


/**
 * @brief Searches for an entry in the Symbol Table between two scopes
 * 
 * @param oSymTable	Pointer to the SymTable
 * @param name		name of Entry
 * @param FromScope	The Scope that we are currently at
 * @param ToScope	The Scope up to which we search
 * 
 * @return The Entry if it was found, otherwise NULL
*/
SymEntry_T SymTable_lookup(SymTable_T oSymTable,
					const char *name, 
					unsigned int FromScope,
					unsigned int ToScope);

/**
 * @brief Searches for a specific entry in the Symbol Table in a specific scope
 * 
 * @param oSymTable	Pointer to the SymTable
 * @param name		Key to be searched
 * @param scope		Scope of the key to be searched
 * 
 * @return The Entry if it was found, otherwise NULL
*/
SymEntry_T SymTable_lookup_scope(SymTable_T oSymTable,
					const char *name, unsigned int scope);

#endif  /* symTable ADT*/