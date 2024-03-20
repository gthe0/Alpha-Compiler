/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* symTable.h		            							*/
/* 														    */
/* A simple interface for a Symbol Table template	        */
/* used to store syntax analyzer token information			*/
/*----------------------------------------------------------*/

#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__ 

#include <symTableEntry.h>

typedef struct 	SymbolTableEntry SymEntry_t;

typedef struct 	SymTable SymTable, *SymTable_T;

/** 
* @brief Return a new empty SymTable_T
*/
SymTable_T SymTable_new(void);

/**
* @brief It frees the binded memory of the SymTalbe .
*
* @param oSymTable  Symbol Table to be freed.
*/
void SymTable_free(SymTable_T oSymTable);

/**
* @brief Inserts information to the Symbol Table.
* 
* @param oSymTable		Pointer to the SymTable 
* @param eSymTable		Entry of the SymTable to be inserted
*/
int SymTable_insert(SymTable_T oSymTable,
					SymEntry_t eSymTable);

#endif  /* symTable ADT*/