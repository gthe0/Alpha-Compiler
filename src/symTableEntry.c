/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* symTableEntry.c	            							*/
/* 														    */
/* A simple implementation of the symTableEntry module		*/
/*----------------------------------------------------------*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <symTableEntry.h>


/*
* Set Variable Value in the union
*/
Variable* setVariable(const char* name, 
						unsigned int scope,
						 unsigned int line)
{
	Variable* new = malloc(sizeof(Variable));

	/* If malloc fails return NULL */	
	if(!new)
	{
		fprintf(stderr,"%s: Malloc Failed in %s in line %d",
				strerror(errno),__FILE__ ,__LINE__);
		return (new);
	}

	new->name = strdup(name);
	new->line = line;
	new->scope = scope;

	return (new);
}

/*
* Set Function Value in the union
*/
Function* setFunction(const char* name, 
						unsigned int scope,
						 unsigned int line)
{
	Function* new = malloc(sizeof(Function));

	/* If malloc fails return NULL */	
	if(!new)
	{
		fprintf(stderr,"%s: Malloc Failed in %s in line %d",
				strerror(errno),__FILE__ ,__LINE__);
		return (new);
	}

	new->name = strdup(name);
	new->line = line;
	new->scope = scope;

	return (new);
}

/*
* Free Entry
*/
void SymEntry_free(SymEntry_T oSymEntry)
{
	free(oSymEntry->value.varVal->name);
	free(oSymEntry->value.funcVal->name);

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

	/* If malloc fails return NULL */	
	if(!oSymEntry)
	{
		fprintf(stderr,"%s: Malloc Failed in %s in line %d",
				strerror(errno),__FILE__ ,__LINE__);
		
		return (oSymEntry);
	}

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
