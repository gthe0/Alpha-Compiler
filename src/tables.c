/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* tables.h			            							*/
/* 														    */
/* Wrapper Functions/APIs for the scope and symbol tables	*/
/*----------------------------------------------------------*/


#include <tables.h>
#include <log.h>

#include <stdio.h>
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

/* Insert Entries in both tables */
int Tables_insert(SymTable_T oSymTable,
					ScopeTable_T oScopeTable,
					SymbolType type,
					const char* name,
					unsigned int scope,
					unsigned int yylineno)
{
	int a;
	SymEntry_T entry = SymEntry_create(type,name,scope,yylineno);
	
	if(a = (SymTable_insert(oSymTable,entry)|ScopeTable_insert(oScopeTable,entry)))
	{
		LOG_ERROR(PARSER,ERROR,"Insert failed ! Token already exists\n");
		SymEntry_free(entry);
	}

	return a;
}


/* Free both tables */
void Tables_free(SymTable_T oSymTable,
				ScopeTable_T oScopeTable)
{
	SymTable_free(oSymTable);
	ScopeTable_free(oScopeTable,0);
}

/* Initialize both tables */
int Tables_init(SymTable_T* oSymTable,
				ScopeTable_T* oScopeTable)
{
	/* If they are both initialized already*/
	if(!oSymTable && !oScopeTable)
	{
		LOG_ERROR(PARSER,NOTE,"You Inserted Initialized tables !\n");
		return EXIT_FAILURE;
	}

	*oSymTable = SymTable_new();
	*oScopeTable = ScopeTable_new();

	for (int i = 0; i < NO_OF_LIBFUNCTS; i++)
	{
		Tables_insert(*oSymTable,*oScopeTable,LIBFUNC,LIB_FUNCTIONS[i],0,0);
	}

	return EXIT_SUCCESS;
}

/* Choose to either use the Symbol Table print or the Scope Table one. */
void Tables_print(SymTable_T oSymTable,ScopeTable_T oScopeTable
				, FILE* ost, bool option)
{
	if(option)
	{
		SymTable_print(oSymTable,ost);
	}
	else
	{
		ScopeTable_print(oScopeTable,ost);
	}

	return ;
}