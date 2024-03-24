/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* scopeTable.h		            							*/
/* 														    */
/* A list implementation for Scope Table ADT		        */
/*----------------------------------------------------------*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <scopeTable.h>

#define SIZE 5

typedef struct ScopeList ScopeList,*ScopeList_T;

/* Binding used for the table */
struct ScopeList
{
	SymEntry_T oSymEntry;
	ScopeList_T next;
};

/*The scope Table */
struct scopeTable
{
	unsigned int max_scope;
	ScopeList_T* table;
};

/*---------------------------- UTILITIES --------------------------------*/

/**
* @brief A function that checks if a SymEntry is within a List 
* @param List the List that we will parse.
* @param name the name of the Entry.
* @param scope the scope of the Entry
*
* @return Entry if it does, NULL if it does not
*/
static SymEntry_T List_contains(ScopeList_T List, const char* name, unsigned int scope )
{
	if(List == NULL || name == NULL) 
		return NULL;
	
	while (List != NULL)
	{
		if (List->oSymEntry->isActive
			&& !strcmp(getName(List->oSymEntry),name) 
			&& getScope(List->oSymEntry) == scope)
			return List->oSymEntry;

		List = List->next;
	}

	return NULL;
}

/**
* @brief A function to Print list from the end to the begining
* @param head the List that we will print.
*
*/
static void In_Order_Print(ScopeList_T head)
{
	if(head == NULL)
		return;
	
	In_Order_Print(head->next);
	SymEntry_print(head->oSymEntry);
}

/*---------------------------------------------------------------------*/

/* 
* It returns a new Empty ScopeTable
*/
ScopeTable_T ScopeTable_new(void)
{
	ScopeTable_T new = malloc(sizeof(scopeTable));

	/* If malloc fails abort */	
	assert(new);

	new->max_scope = SIZE - 1;
	new->table = malloc(sizeof(ScopeList_T)*SIZE);

	/* If malloc fails abort */	
	assert(new->table);

	for (int i = 0; i < SIZE; i++)
        new->table[i] = NULL;

	return new;
}

/* Free the scope table */
void ScopeTable_free(ScopeTable_T oScopeTable, int free_entries)
{
	if(oScopeTable == NULL)
		return;

	ScopeList_T temp = NULL,
		   		head = NULL ;
	
	for (int i = 0; i < oScopeTable->max_scope; i++)
	{
		head = oScopeTable->table[i];

		while (head)
		{
			temp = head ;
			head = head->next;

			if(free_entries)
				SymEntry_free(temp->oSymEntry);
			
			free(temp);
		}
	}
	
	free(oScopeTable->table);
	free(oScopeTable);
}

/* 
* It Inserts an Entry in the Scope Table
*/
int ScopeTable_insert(ScopeTable_T oScopeTable,SymEntry_T oSymEntry)
{
	int scope = getScope(oSymEntry);
	/* Resize if scope is bigger than no of buckets */
	if(oScopeTable->max_scope < scope )
	{
		oScopeTable->table = realloc(oScopeTable->table,
									oScopeTable->max_scope+SIZE);
		/* If realloc fails abort */	
		assert(oScopeTable->table);

		for (int i = oScopeTable->max_scope + 1 ;
				 i <= oScopeTable->max_scope + SIZE;
				 i++)
        	oScopeTable->table[i] = NULL;

		oScopeTable->max_scope += SIZE;
	}

	ScopeList_T head = oScopeTable->table[scope];

	/* Check if there is an Active instance of the Entry */
	if(List_contains(head,getName(oSymEntry),getScope(oSymEntry)) != NULL)
		return EXIT_FAILURE;

	ScopeList_T node = malloc(sizeof(ScopeList));

	/* If we run out of mem, abort */
	assert(node);

	node->oSymEntry = oSymEntry;
	node->next = head;

	oScopeTable->table[scope] = node;

	return EXIT_SUCCESS;
}

/* Hide all entries with the same Scope */
int ScopeTable_hide(ScopeTable_T oScopeTable, unsigned int scope)
{
	if(oScopeTable == NULL && scope > oScopeTable->max_scope)
		return EXIT_FAILURE;
	
	ScopeList_T head = oScopeTable->table[scope];

	while (head && head->oSymEntry->isActive)
	{
		head->oSymEntry->isActive = false;
		head = head->next;
	}

	return EXIT_SUCCESS;
}

/* Prints the contents of the Symbol Table per scope and order */
void ScopeTable_print(ScopeTable_T oScopeTable) 
{
	if(!oScopeTable)
		return ;
	
   	ScopeList_T head = NULL ;

	for (int i = 0; i < oScopeTable->max_scope; i++)
	{
		head = oScopeTable->table[i];
		In_Order_Print(head);
	}

}
