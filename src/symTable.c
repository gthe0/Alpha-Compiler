/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* symTable.c		            							*/
/* 														    */
/* A Linked List implementation of the symTable interface 	*/
/*----------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <symTable.h>

#define HASH_MULTIPLIER 65599
#define BUCKET_SIZE 509

/* Type of the SymTable */
typedef struct SymTableList{
	SymEntry_T entry;
	struct SymTableList* next;
}SymList;

/* Type of the SymTable */
struct SymTable{
	SymList* List[BUCKET_SIZE];
};


SymTable_T oSymTable = NULL;

/*---------------------------- UTILITIES --------------------------------*/

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
* @param name the name of the Entry.
* @param scope the scope of the Entry
*
* @return Entry if it does, NULL if it does not
*/
static SymEntry_T List_contains(SymList* List, const char* name, unsigned int scope )
{
	if(List == NULL || name == NULL) 
		return NULL;
	
	while (List != NULL)
	{
		if (List->entry->isActive
			&& !strcmp(getName(List->entry),name) 
			&& getScope(List->entry) == scope)
			{
				return List->entry;
			}

		List = List->next;
	}

	return NULL;
}

/*---------------------------------------------------------------------*/

/* It returns a new Empty SymTable or aborts if it fails*/
void SymTable_new(void)
{
	SymTable_T new = malloc(sizeof(SymTable));
	
	/* If malloc fails abort */	
	assert(new);

	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		new->List[i] = NULL;
	}
	
	oSymTable = new;

	return ;
}

/* It frees the SymTable */
void SymTable_free(void)
{
	if(oSymTable == NULL)
		return;

	SymList* temp = NULL,
		   * head = NULL ;
	
	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		head = oSymTable->List[i];

		while (head)
		{
			temp = head ;
			head = head->next;

			SymEntry_free(temp->entry);
			free(temp);
		}
	}
	
	free(oSymTable);
}

/* It Inserts an Entry in the SymTable */
int SymTable_insert(SymEntry_T oSymEntry)
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
	if(List_contains(head,name,getScope(oSymEntry)) != NULL)
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
SymEntry_T SymTable_lookup_scope(const char *name,  int scope)
{
	if(!name || !oSymTable || scope < 0)
	{
		return NULL;
	}

	int hash = SymTable_hash(name);
	SymList* head = oSymTable->List[hash];

	return List_contains(head,name,scope);
}

/* It searches for an entry in the SymTable between 2 scopes */
SymEntry_T SymTable_lookup(const char *name, 
					int FromScope,
					int ToScope)
{
	SymEntry_T Entry = NULL;

	if(oSymTable == NULL || name == NULL || ToScope > FromScope || ToScope < 0) 
		return NULL;
	

	while(FromScope >= ToScope && Entry == NULL)
	{
		Entry = SymTable_lookup_scope(name,FromScope--);
	}

	return Entry;
}

/* Prints the contents of Symbol Table in random order */
void SymTable_print(FILE* ost) 
{
	if(!oSymTable)
		return ;

   	SymList	* head = NULL ;

	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		head = oSymTable->List[i];

		while (head)
		{
			SymEntry_print(head->entry, ost);
			head = head->next;
		}
	}

}

/* Checks if it is initialized */
int SymTable_isInit(void)
{
	return oSymTable != NULL ? 1 : 0 ;
}