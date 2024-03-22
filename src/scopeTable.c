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

#define INIT_SIZE 5

typedef struct binding ScopeBind;

/* Binding used for the table */
struct binding
{
	SymEntry_T oSymEntry;
	ScopeBind* next;
};

/*The scope Table */
struct scopeTable
{
	unsigned int max_scope;
	ScopeBind** table;
};

/* It returns a new Empty ScopeTable*/
ScopeTable_T ScopeTable_new(void)
{
	ScopeTable_T new = malloc(sizeof(scopeTable));
	assert(new);

	new->max_scope = INIT_SIZE - 1;
	
	new->table = malloc(sizeof(ScopeBind*)*INIT_SIZE);
	assert(new->table);

	for (int i = 0; i < INIT_SIZE; i++)
        new->table[i] = NULL;

	return new;
}

int ScopeTable_insert(ScopeTable_T oScopeTable,SymEntry_T oSymEntry)
{
	assert(oScopeTable);
	assert(oSymEntry);

	int scope = oSymEntry->type > FORMAL
			  ? oSymEntry->value.funcVal->scope
			  : oSymEntry->value.varVal->scope;


	
}