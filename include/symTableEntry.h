/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* symTableEntry.h	            							*/
/* 														    */
/* Type definitions and module for the Symbol Table Entries	*/
/*----------------------------------------------------------*/

#ifndef __SYMBOL_TABLE_ENTRY__
#define __SYMBOL_TABLE_ENTRY__ 

#include <scopeSpace.h>

#include <stdio.h>
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
	ScopeSpace space;
	const char *name;
	unsigned int scope;
	unsigned int line;
	unsigned int offset;
};

/* Struct storing Function information*/
struct Function
{
	const char *name;
	unsigned int scope;
	unsigned int line;
	unsigned int retlist;
	unsigned int iaddress;
	unsigned int taddress;
	unsigned int total_locals;
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

/**
* @brief Getter of Symbol Line.
*
* @param oSymEntry Symbol Entry.
*
* @return The line of Insertion.
*/
unsigned int getLine(SymEntry_T oSymEntry);

/**
* @brief Getter of Variable's offset.
*
* @param oSymEntry Symbol Entry.
*
* @return The offset of the variable.
*/
unsigned int getOffset_val(SymEntry_T oSymEntry);


/**
* @brief Getter of Variable's Space.
*
* @param oSymEntry Symbol Entry.
*
* @return The Space of the variable.
*/
ScopeSpace getSpace_val(SymEntry_T oSymEntry);

/**
* @brief Getter of instruction address 
*
* @param oSymEntry Symbol Entry.
*
* @return function.address
*/
unsigned int get_i_address(SymEntry_T oSymEntry);


/**
* @brief Getter of instruction address 
*
* @param oSymEntry Symbol Entry.
*
* @return function.address
*/
unsigned int get_t_address(SymEntry_T oSymEntry);

/**
* @brief Setter of instruction addresses 
* 
* @param oSymEntry Symbol Entry.
* @param iaddress the Instruction address of the function
* 
*/
void set_i_address(SymEntry_T oSymEntry,unsigned int iaddress);

/**
* @brief Setter of instruction addresses 
* 
* @param oSymEntry Symbol Entry.
* @param taddress the Instruction address of the function
* 
*/
void set_t_address(SymEntry_T oSymEntry,unsigned int taddress);


/**
* @brief Getter of return list 
* 
* @param oSymEntry Symbol Entry.
*
* @return function.retlist
*/
unsigned int get_retlist(SymEntry_T oSymEntry);

/**
* @brief Setter of instruction addresses 
* 
* @param oSymEntry Symbol Entry.
* @param retlist The Instruction address of the function
* 
*/
void set_retlist(SymEntry_T oSymEntry,unsigned int retlist);


/**
* @brief Getter of return list 
* 
* @param oSymEntry Symbol Entry.
*
* @return function.total_locals
*/
unsigned int get_total_locals(SymEntry_T oSymEntry);


/**
* @brief Setter of total_locals 
* 
* @param oSymEntry Symbol Entry.
* @param total_locals The total of the function locals
* 
*/
void set_total_locals(SymEntry_T oSymEntry, unsigned int total_locals);


/**
* @brief Prints the Symbol Entry.
*
* @param oSymEntry Symbol Entry.
* @param ost The output stream used.
*/
void SymEntry_print(SymEntry_T oSymEntry, FILE* ost);


#endif  /* Symbol Table Entry */