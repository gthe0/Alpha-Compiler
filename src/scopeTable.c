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

/* 
* It frees a ScopeList
*/
static void ScopeList_free(ScopeList_T list)
{
	ScopeList_T temp;

	while (list)
	{
		temp = list;
		list = list->next;
		
		SymEntry_free(temp->oSymEntry);
		free(temp);
	}
	
	return ;
} 

/* 
* It returns a new Empty ScopeList
*/
static ScopeList_T ScopeList_alloc()
{
	ScopeList_T new = malloc(sizeof(ScopeList));

	/* If malloc fails abort */	
	assert(new);

	new->oSymEntry = NULL;
	new->next = NULL;

	return new; 
} 

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
		/* If malloc fails abort */	
		assert(oScopeTable->table);

		for (int i = oScopeTable->max_scope + 1 ;
				 i <= oScopeTable->max_scope + SIZE;
				 i++)
        	oScopeTable->table[i] = NULL;

		oScopeTable->max_scope += SIZE;
	}

	ScopeList_T sl = oScopeTable->table[scope];

	return EXIT_FAILURE;
}