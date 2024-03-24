/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* tables.h			            							*/
/* 														    */
/* Wrapper Functions/APIs for the scope and symbol tables	*/
/*----------------------------------------------------------*/


#include <tables.h>
#include <log.h>

#include <stdlib.h>

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
int Tables_insert(	SymTable_T oSymTable,
					ScopeTable_T oScopeTable,
					SymbolType type,
					const char* name,
					unsigned int scope,
					unsigned int yylineno)
{
	SymEntry_T entry = SymEntry_create(type,name,scope,yylineno);

	return SymTable_insert(oSymTable,entry) &&
			ScopeTable_insert(oScopeTable,entry);
}

/* Free both tables */
void Tables_free(SymTable_T oSymTable,
				ScopeTable_T oScopeTable)
{
	SymTable_free(oSymTable);
	ScopeTable_free(oScopeTable);
}

/* Initialize both tables */
int Tables_init(SymTable_T* oSymTable,
				ScopeTable_T* oScopeTable)
{
	/* If they are both initialized already*/
	if(!oSymTable && !oScopeTable)
		return EXIT_FAILURE;

	*oSymTable = SymTable_new();
	*oScopeTable = ScopeTable_new();

	for (int i = 0; i < NO_OF_LIBFUNCTS; i++)
	{
		Tables_insert(*oSymTable,*oScopeTable,LIBFUNC,LIB_FUNCTIONS[i],0,0);
	}

	return EXIT_SUCCESS;
}