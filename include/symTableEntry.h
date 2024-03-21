/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* symTableEntry.h	            							*/
/* 														    */
/* Type definitions and module for the Symbol Table Entries	*/
/*----------------------------------------------------------*/

#ifndef __SYMBOL_TABLE_ENTRY__
#define __SYMBOL_TABLE_ENTRY__ 

#include <stdbool.h>

/* Module struct */
typedef struct 	SymbolTableEntry *SymbolTableEntry_T;

/* Type definitions */
typedef enum	SymbolType SymbolType;
typedef struct 	Variable Variable;
typedef struct 	Function Function;

/*
*  Enums for symbol types characteristics
*  (i.e. scope,argument e.t.c)
*/
enum SymbolType
{
	GLOBAL,
	LOCAL,
	FORMAL,
	USERFUNC,
	LIBFUNC
};

/* Struct storing Variable information*/
struct Variable
{
	const char *name;
	unsigned int scope;
	unsigned int line;
};

/* Struct storing Function information*/
struct Function
{
	const char *name;
	unsigned int scope;
	unsigned int line;
};

/* Struct used for the Symbol Table Entry*/
struct SymbolTableEntry
{
	bool isActive;
	union
	{
		Variable *varVal;
		Function *funcVal;
	}value;
	
	SymbolType type;
};

/**
* @brief It frees the binded memory of a Symbol Entry.
*
* @param oSymEntry Symbol Entry to be freed.
*/
void freeEntry(SymbolTableEntry_T oSymEntry);

/**
* @brief Sets the Entry's varVariable.
* 
* @param name	Name of the Variable.
* @param scope	Scope of the Variable.
* @param line	The line where the Variable was found.
*
* @return	A Variable type pointer or NULL.
*/
Variable* setVariable(const char* name, 
						unsigned int scope,
						 unsigned int line);

/**
* @brief Sets the Entry's varFunction.
* 
* @param name	Name of the Function.
* @param scope	Scope of the Function.
* @param line	The line where the Function was found.
*
* @return	A Function type pointer or NULL.
*/
Function* setFunction(const char* name, 
						unsigned int scope,
						 unsigned int line);

SymbolTableEntry_T setEntry(


);

#endif  /* Symbol Table Entry */