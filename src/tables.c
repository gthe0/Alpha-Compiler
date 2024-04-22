/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* tables.h			            							*/
/* 														    */
/* Wrapper Functions/APIs for the scope and symbol tables	*/
/*----------------------------------------------------------*/


#include <tables.h>
#include <log.h>
#include <name_gen.h>
#include <symTable.h>
#include <scopeTable.h>

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
int Tables_insert_Entry(SymEntry_T entry)
{
	int a;
	
	if(a = (SymTable_insert(entry)|ScopeTable_insert(entry)))
	{
		LOG_ERROR(PARSER,ERROR,"Insert failed ! Token already exists\n");
		SymEntry_free(entry);
	}

	return a;
}

/* Insert Entries in both tables */
int Tables_insert(SymbolType type,
					const char* name,
					unsigned int scope,
					unsigned int yylineno)
{
	int a;
	SymEntry_T entry = SymEntry_create(type,name,scope,yylineno);
	
	if(a = (SymTable_insert(entry)|ScopeTable_insert(entry)))
	{
		LOG_ERROR(PARSER,ERROR,"Insert failed ! Token already exists\n");
		SymEntry_free(entry);
	}

	return a;
}


/* Free both tables */
void Tables_free(void)
{
	SymTable_free();
	ScopeTable_free(0);
}

/* Initialize both tables */
int Tables_init(void)
{
	/* If they are both initialized already*/
	if(SymTable_isInit() && ScopeTable_isInit())
	{
		LOG_ERROR(PARSER,NOTE,"Tables are already Initialized !\n");
		return EXIT_FAILURE;
	}

	SymTable_new();
	ScopeTable_new();

	for (int i = 0; i < NO_OF_LIBFUNCTS; i++)
	{
		Tables_insert(LIBFUNC,LIB_FUNCTIONS[i],0,0);
	}

	return EXIT_SUCCESS;
}

/* Choose to either use the Symbol Table print or the Scope Table one. */
void Tables_print(FILE* ost, bool option)
{
	if(option)
	{
		SymTable_print(ost);
	}
	else
	{
		ScopeTable_print(ost);
	}

	return ;
}

/* Uses a new temp variable */
SymEntry_T newtemp(int scope,
					unsigned yylineno)
{
	char *name = new_temp_name(); 
	SymEntry_T oSymEntry;

	oSymEntry = SymTable_lookup_scope(name,scope);
	
	if(!oSymEntry)
	{
		if(scope == 0)	oSymEntry = SymEntry_create(GLOBAL,name,scope,yylineno);
		else			oSymEntry = SymEntry_create(LOCAL,name,scope,yylineno);
	
		Tables_insert_Entry(oSymEntry);
	}

	return oSymEntry;
}