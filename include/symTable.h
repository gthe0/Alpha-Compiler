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

typedef struct 	SymbolTableEntry SymEntry_t;

typedef struct 	SymTable {
	SymEntry_t *entry;
	struct SymTable *next;
}SymTable, *SymTable_T;

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
					SymEntry_t oSymEntry);


/**
 * @brief Searches for a specific entry in the Symbol Table
 * 
 * @param oSymTable	Pointer to the SymTable
 * @param pcKey		Key to be searched
 * 
 * @return	Whether the key was found or not
*/

int SymTable_lookup(SymTable_T oSymTable,
					const char *pcKey);


#endif  /* symTable ADT*/