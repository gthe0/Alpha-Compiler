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

	new->name = strdup(name);
	new->line = line;
	new->scope = scope;
	new->offset = currscopeoffset(); 
	new->space = currscopespace();
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
	if(oSymEntry->type > FORMAL )
	{
		free((char*)oSymEntry->value.funcVal->name);
		free(oSymEntry->value.funcVal);
	}
	else
	{
		free((char*)oSymEntry->value.varVal->name);
		free(oSymEntry->value.varVal);
	}

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
	SymEntry_T oSymEntry = malloc(sizeof(SymEntry));

	/* If malloc fails abort */	
	assert(oSymEntry);

	oSymEntry->isActive = true;
	oSymEntry->type		= type;

	/* Check if it is a Function or not and set value */
	if (type > FORMAL)
	{
		oSymEntry->value.funcVal 	= setFunction(name, scope, line);
	}
	else
	{
		oSymEntry->value.varVal 	= setVariable(name, scope, line);
	}

	return (oSymEntry);
}

/* Getter of Name */
const char* getName(SymEntry_T oSymEntry)
{
	/* If oSymEntry is NULL or both enties in value are NULL then abort */	
	assert(oSymEntry);

	/* Return the name */
	return oSymEntry->type > FORMAL ? oSymEntry->value.funcVal->name : oSymEntry->value.varVal->name ;
}

/* Getter of Scope */
unsigned int getScope(SymEntry_T oSymEntry)
{
	/* If oSymEntry is NULL or both enties in value are NULL then abort */	
	assert(oSymEntry);

	/* Return the scope */
	return oSymEntry->type > FORMAL ? oSymEntry->value.funcVal->scope : oSymEntry->value.varVal->scope ;
}

/* Getter of Line */
unsigned int getLine(SymEntry_T oSymEntry)
{
	/* If oSymEntry is NULL or both enties in value are NULL then abort */	
	assert(oSymEntry);

	/* Return the line */
	return oSymEntry->type > FORMAL ? oSymEntry->value.funcVal->line : oSymEntry->value.varVal->line ;
}

unsigned int getOffset_val(SymEntry_T oSymEntry)
{
	/* If oSymEntry is NULL or the type is not of a variable then abort */	
	assert(oSymEntry && oSymEntry->type < USERFUNC);

	/* Return the variable's offset */
	return oSymEntry->value.varVal->offset ;
}

ScopeSpace getSpace_val(SymEntry_T oSymEntry)
{
	/* If oSymEntry is NULL or the type is not of a variable then abort */	
	assert(oSymEntry && oSymEntry->type < USERFUNC);

	/* Return the variable's space */
	return oSymEntry->value.varVal->space;
}

/* Used to Print Entry Info */
void SymEntry_print(SymEntry_T oSymEntry, FILE* ost)
{
	if(!oSymEntry)
		return ;

	if (oSymEntry->type == GLOBAL) 
	{
		fprintf(ost," \"%s\" [global variable] (line %d) (scope %d)\n", oSymEntry->value.varVal->name, oSymEntry->value.varVal->line, oSymEntry->value.varVal->scope);
	} 
	else if (oSymEntry->type == LOCAL) 
	{
		fprintf(ost," \"%s\" [local variable] (line %d) (scope %d)\n", oSymEntry->value.varVal->name, oSymEntry->value.varVal->line, oSymEntry->value.varVal->scope);            } 
	else if (oSymEntry->type == FORMAL) 
	{
		fprintf(ost," \"%s\" [formal variable] (line %d) (scope %d)\n", oSymEntry->value.varVal->name, oSymEntry->value.varVal->line, oSymEntry->value.varVal->scope);
	}
	else if (oSymEntry->type == USERFUNC) 
	{
		fprintf(ost," \"%s\" [user function] (line %d) (scope %d)\n", oSymEntry->value.funcVal->name, oSymEntry->value.funcVal->line, oSymEntry->value.funcVal->scope);
	} 
	else if (oSymEntry->type == LIBFUNC) 
	{
		fprintf(ost," \"%s\" [library function] (line %d) (scope %d)\n", oSymEntry->value.funcVal->name, oSymEntry->value.funcVal->line, oSymEntry->value.funcVal->scope);
	}

	return ;
}