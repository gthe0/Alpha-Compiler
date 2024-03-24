/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* symTable.h		            							*/
/* 														    */
/* A Linked List implementation of the symTable interface 	*/
/*----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <symTable.h>

#define HASH_MULTIPLIER 65599
#define BUCKET_SIZE 509

/* Type of the SymTable */
typedef struct SymTableList{
	SymEntry_T entry;
	SymTable_T next;
}SymList;

/* Type of the SymTable */
struct SymTable{
	SymList** List;
};

/*---------------------------- UTILITY --------------------------------*/

/**
* @brief The hash function used 
* @param name The name of the SymEntry (Used to calculate the hash)
*
* @return The hash/index for the Table
*/
static unsigned int SymTable_hash(const char *name)
{
    unsigned int ui;
    unsigned int uiHash = 0U;

    for (ui = 0U; name[ui] != '\0'; ui++)
        uiHash = uiHash * HASH_MULTIPLIER + name[ui];

    return (uiHash % BUCKET_SIZE);
}

/**
* @brief A function that checks if a SymEntry is within a List 
* @param List the List that we will parse.
* @param oSymEntry the Entry that we want to see if it is there.
*
* @return EXIT_SUCCESS if it does, EXIT_FAILURE if it does not
*/
static int List_contains(SymList* List, SymEntry_T oSymEntry)
{
	if(List == NULL || oSymEntry == NULL) 
		return EXIT_FAILURE;
	
	/* Get scope and name */
	const char* name = getName(oSymEntry);
	unsigned int scope = getScope(oSymEntry);
	
}

/*---------------------------------------------------------------------*/

/* It returns a new Empty SymTable or aborts if it fails*/
SymTable_T SymTable_new(void)
{
	SymTable_T new = malloc(sizeof(SymTable));
	SymList** List = malloc(sizeof(SymList*));
	
	/* If malloc fails abort */	
	assert(new);
	assert(List);

	new->List = List;

	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		new->List[i] = NULL;
	}
	
	return new;
}

/* It frees the SymTable */
void SymTable_free(SymTable_T oSymTable)
{
}

/* It Inserts an Entry in the SymTable */
int SymTable_insert(SymTable_T oSymTable,
					SymEntry_T oSymEntry)
{
	if(!oSymEntry || !oSymTable)
	{
		return EXIT_FAILURE;
	}

	/* Calculate Hash */
	const char*	name = getName(oSymEntry);
	int 		hash = SymTable_hash(name);
	
	/* Get list head */
	SymList* head = oSymTable->List[hash];
	SymList* node = NULL;

	/* Check if there is an Active instance of the Entry */
	if(List_contains(head,oSymEntry) == EXIT_FAILURE)
		return EXIT_FAILURE;

	node = malloc(sizeof(SymList));

	/* If we run out of mem, abort */
	assert(node);

	node->entry = oSymEntry;
	node->next = head;

	oSymTable->List[hash] = node;

	return EXIT_SUCCESS;
}


/* It searches for a specific entry in the SymTable */
int SymTable_lookup(SymTable_T oSymTable,
					const char *pcKey)
{
	
	return EXIT_FAILURE;
}

int SymTable_lookup_scope(SymTable_T oSymTable,
					const char *pcKey, unsigned int scope)
{
	return EXIT_FAILURE;
}

/* It hides all Entries in the table */
void SymTable_hide(SymTable_T oSymTable, unsigned int scope) 
{
}
