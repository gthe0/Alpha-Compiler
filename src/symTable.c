/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* symTable.h		            							*/
/* 														    */
/* A Hash Map implementation of the symTable interface 		*/
/*----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <symTable.h>

/* It returns a new Empty SymTable or NULL if it fails*/
SymTable_T SymTable_new(void)
{
	SymTable_T oSymTable = (SymTable_T)malloc(sizeof(SymTable));
	if (oSymTable == NULL)
	{
		return NULL;
	}

	oSymTable->entry = NULL;
	oSymTable->next = NULL;

	return oSymTable;
}

/* It frees the SymTable */
void SymTable_free(SymTable_T oSymTable)
{
	SymTable_T oNext;

	while (oSymTable != NULL)
	{
		oNext = oSymTable->next;
		free(oSymTable);
		oSymTable = oNext;
	}
}

/* It Inserts an Entry in the SymTable */
int SymTable_insert(SymTable_T oSymTable,
					SymEntry_T oSymEntry)
{
	//insert the oSymEntry in the oSymTable at the end of the list
	
	while (oSymTable->next != NULL)
	{
		oSymTable = oSymTable->next;
	}

	oSymTable->next = (SymTable_T)malloc(sizeof(SymTable));
	if (oSymTable->next == NULL)
	{
		return EXIT_FAILURE;
	}

	oSymTable->next->entry = &oSymEntry;
	oSymTable->next->next = NULL;
	return EXIT_SUCCESS;
}


/* It searches for a specific entry in the SymTable */
int SymTable_lookup(SymTable_T oSymTable,
					const char *pcKey)
{
	while (oSymTable != NULL)
	{
		if (oSymTable->entry->value.varVal->name == pcKey || oSymTable->entry->value.funcVal->name == pcKey)
		{
			return EXIT_SUCCESS;
		}
		oSymTable = oSymTable->next;
	}

	return EXIT_FAILURE;
}