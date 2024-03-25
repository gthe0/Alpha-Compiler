/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* yacc_lib.c		            							*/
/* 														    */
/* Implementation of yacc_lib.h								*/
/*----------------------------------------------------------*/

#include <yacc_lib.h>

#include <assert.h>
#include <stdlib.h>

lvalue_local(SymTable_T oSymTable,char* name,int scope)
{
	assert(oSymTable);
	assert(name);

	/* If local value is already inserted in 
	the Symbol table, return Exit Failure*/
	if(SymTable_lookup_scope(oSymTable,name,scope) != NULL)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

lvalue_global(SymTable_T oSymTable,char* name)
{
	assert(oSymTable);
	assert(name);

	/* If global value is already inserted in 
	the Symbol table, return Exit Failure*/
	if(SymTable_lookup_scope(oSymTable,name,0) != NULL)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

lvalue_Function(SymTable_T oSymTable,char* name)
{
	assert(oSymTable);
	assert(name);

	/* If global value is already inserted in 
	the Symbol table, return Exit Failure*/
	if(SymTable_lookup_scope(oSymTable,name,0) != NULL)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}