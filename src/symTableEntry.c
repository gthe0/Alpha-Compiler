/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* symTableEntry.c	            							*/
/* 														    */
/* A simple implementation of the symTableEntry module		*/
/*----------------------------------------------------------*/

#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <symTableEntry.h>


/*
* Set Variable Value in the union
*/
static Variable* setVariable(const char* name, 
							unsigned int scope,
						 	unsigned int line)
{
	Variable* new = malloc(sizeof(Variable));

	/* If malloc fails abort */	
	assert(new);

	new->name = name;
	new->name = name;
	new->line = line;
	new->scope = scope;

	return (new);
}

/*
* Set Function Value in the union
*/
static Function* setFunction(const char* name, 
							unsigned int scope,
						 	unsigned int line)
{
	Function* new = malloc(sizeof(Function));

	/* If malloc fails abort */	
	assert(new);

	new->name = name;
	new->name = name;
	new->line = line;
	new->scope = scope;

	return (new);
}

/*
* Free Entry
*/
void SymEntry_free(SymEntry_T oSymEntry)
{

	free(oSymEntry->value.varVal);
	free(oSymEntry->value.funcVal);

	free(oSymEntry);

	return ;
}

/*
* Set Entry for the table
*/
SymEntry_T SymEntry_create(
	SymbolType type,
	const char* name, 
	unsigned int scope,
	unsigned int line)
{
	SymEntry_T oSymEntry = malloc(sizeof(SymEntry_T));

	/* If malloc fails abort */	
	assert(oSymEntry);

	oSymEntry->isActive = true;
	oSymEntry->type		= type;

	/* Check if it is a Function or not and set value */
	if (type > FORMAL)
	{
		oSymEntry->value.funcVal 	= setFunction(name, scope, line);
		oSymEntry->value.varVal		= NULL;
	}
	else
	{
		oSymEntry->value.varVal 	= setVariable(name, scope, line);
		oSymEntry->value.funcVal 	= NULL;
	}

	return (oSymEntry);
}

/* Getter of Name */
const char* getName(SymEntry_T oSymEntry)
{
	/* If oSymEntry is NULL or both enties in value are NULL then abort */	
	assert(oSymEntry);
	assert(oSymEntry->value.funcVal && oSymEntry->value.varVal);

	/* Return the name */
	return oSymEntry->type > FORMAL ? oSymEntry->value.funcVal->name : oSymEntry->value.varVal->name ;
}

/* Getter of Scope */
unsigned int getScope(SymEntry_T oSymEntry)
{
	/* If oSymEntry is NULL or both enties in value are NULL then abort */	
	assert(oSymEntry);
	assert(oSymEntry->value.funcVal && oSymEntry->value.varVal);

	/* Return the scope */
	return oSymEntry->type > FORMAL ? oSymEntry->value.funcVal->scope : oSymEntry->value.varVal->scope ;
}
