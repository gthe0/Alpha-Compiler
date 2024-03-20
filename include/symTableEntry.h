/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* symTable.h		            							*/
/* 														    */
/* Type definitions for the Symbol Table Entries 			*/
/*----------------------------------------------------------*/

#ifndef __SYMBOL_TABLE_ENTRY__
#define __SYMBOL_TABLE_ENTRY__ 

#include <stdbool.h>

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

#endif  /* Symbol Table Entry */