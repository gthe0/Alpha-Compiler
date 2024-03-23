/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* symTableEntry.h	            							*/
/* 														    */
/* Type definitions and module for the Symbol Table Entries	*/
/*----------------------------------------------------------*/

#ifndef __SYMBOL_TABLE_ENTRY__
#define __SYMBOL_TABLE_ENTRY__ 

#include <stdbool.h>

/* Module struct */
typedef struct 	SymbolTableEntry SymEntry, *SymEntry_T;

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
void SymEntry_free(SymEntry_T oSymEntry);

/**
* @brief Sets the Entry of the SymTable.
* 
* @param type	type of the value.
* @param name	Name of the value.
* @param scope	Scope of the value.
* @param line	The line where the value was found.
*
* @return	A Symbol Table Entry pointer or NULL.
*/
SymEntry_T SymEntry_create(
	SymbolType type,
	const char* name, 
	unsigned int scope,
	unsigned int line
);

/**
* @brief Getter of Symbol name.
*
* @param oSymEntry Symbol Entry.
*/
const char* getName(SymEntry_T oSymEntry);

/**
* @brief Getter of Symbol scope.
*
* @param oSymEntry Symbol Entry.
*/
unsigned int getScope(SymEntry_T oSymEntry);

#endif  /* Symbol Table Entry */