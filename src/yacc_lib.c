/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* yacc_lib.c		            							*/
/* 														    */
/* Implementation of yacc_lib.h								*/
/*----------------------------------------------------------*/

#include <yacc_lib.h>
#include <log.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NO_OF_LIBFUNCTS 12

/* Library Functions. We Insert them in the initialization of the Tables*/
static char* LIB_FUNCTIONS[NO_OF_LIBFUNCTS] =
{
	"print",
	"input",
	"objectmemberkeys",
	"objecttotalmembers",
	"objectcopy",
	"totalarguments",
	"argument",
	"typeof",
	"strtonum",
	"sqrt",
	"cos",
	"sin",
};

static int Lib_shadow_check(char *name)
{
	for (int i = 0; i < NO_OF_LIBFUNCTS; i++)
	{
		if (!strcmp(LIB_FUNCTIONS[NO_OF_LIBFUNCTS],name))
		{
			LOG_ERROR(PARSER,ERROR,"Shadowing Library Function, line %d, token %s",name);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int lvalue_local(SymTable_T oSymTable,char* name,int scope)
{
	SymEntry_T entry;

	assert(oSymTable);
	assert(name);

	if(Lib_shadow_check(name) == EXIT_FAILURE)
		return EXIT_FAILURE;

	/* If an active local instance is already inserted in 
	the Symbol table, return Exit Failure*/
	if(entry = SymTable_lookup_scope(oSymTable,name,scope))
	{
		LOG_ERROR(PARSER,ERROR,"%s local instance found\n",name);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int lvalue_global(SymTable_T oSymTable,char* name)
{
	assert(oSymTable);
	assert(name);

	if(Lib_shadow_check(name) == EXIT_FAILURE)
		return EXIT_FAILURE;

	/* If global value is already inserted in 
	the Symbol table, return Exit Failure*/
	if(SymTable_lookup_scope(oSymTable,name,0) != NULL)
	{
		LOG_ERROR(PARSER,ERROR,"%s global instance found\n",name);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int lvalue_Function(SymTable_T oSymTable,char* name,int FromScope, ScopeStack_T stack)
{
	assert(oSymTable);
	assert(stack);
	assert(name);

	if(Lib_shadow_check(name) == EXIT_FAILURE)
		return EXIT_FAILURE;

	if(ScopeIsEmpty(stack)){
		if(SymTable_lookup_scope(oSymTable,name,0) != NULL)
		{
			LOG_ERROR(PARSER,ERROR,"%s is already inserted in the table\n",name);
			return EXIT_FAILURE;
		}
	}
	else if(SymTable_lookup(oSymTable,name,FromScope,ScopeTop(stack)) != NULL)
	{
		LOG_ERROR(PARSER,ERROR,"%s is already inserted in the table\n",name);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int lvalue_ID(SymTable_T oSymTable,char* name,int FromScope, ScopeStack_T stack)
{
	assert(oSymTable);
	assert(stack);
	assert(name);

	if(Lib_shadow_check(name) == EXIT_FAILURE)
		return EXIT_FAILURE;

	if(ScopeIsEmpty(stack)){
		if(SymTable_lookup_scope(oSymTable,name,0) != NULL)
		{
			LOG_ERROR(PARSER,ERROR,"%s is already inserted in the table\n",name);
			return EXIT_FAILURE;
		}
	}
	else if(SymTable_lookup(oSymTable,name,FromScope,ScopeTop(stack)) != NULL)
	{
		LOG_ERROR(PARSER,ERROR,"%s is already inserted in the table\n",name);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}