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

static unsigned unnamed_func_counter = 0;

/* Library Functions. We Insert them in the initialization of the Tables*/
static char *LIB_FUNCTIONS[NO_OF_LIBFUNCTS] =
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
		if (!strcmp(LIB_FUNCTIONS[NO_OF_LIBFUNCTS], name))
		{
			LOG_ERROR(PARSER, ERROR, "Shadowing Library Function, line %d, token %s", name);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int lvalue_local(SymTable_T oSymTable, char *name, int scope)
{
	SymEntry_T entry;

	assert(oSymTable);
	assert(name);

	if (Lib_shadow_check(name) == EXIT_FAILURE)
		return EXIT_FAILURE;

	/* If an active local instance is already inserted in
	the Symbol table, return Exit Failure*/
	if (entry = SymTable_lookup_scope(oSymTable, name, scope))
	{
		LOG_ERROR(PARSER, ERROR, "%s local instance found\n", name);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int lvalue_global(SymTable_T oSymTable, char *name)
{
	assert(oSymTable);
	assert(name);

	if (Lib_shadow_check(name) == EXIT_FAILURE)
		return EXIT_FAILURE;

	/* If global value is already inserted in
	the Symbol table, return Exit Failure*/
	if (SymTable_lookup_scope(oSymTable, name, 0) != NULL)
	{
		LOG_ERROR(PARSER, ERROR, "%s global instance found\n", name);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/* Function to check if the Function Definition is Valid */
int lvalue_Function(SymTable_T oSymTable, char *name,
					unsigned int line, unsigned int FromScope,
					ScopeStack_T stack)
{
	assert(oSymTable);
	assert(stack);
	assert(name);

	SymEntry_T entry = NULL;

	if (Lib_shadow_check(name) == EXIT_FAILURE)
	{
		LOG_ERROR(PARSER, ERROR, "Shadowing Library Function, line %d, token %s", line, name);
		LOG_ERROR(PARSER, NOTE, "%s is a library Function\n\n", name, getLine(entry));

		return EXIT_FAILURE;
	}

	if (entry = SymTable_lookup_scope(oSymTable, name, 0))
	{
		LOG_ERROR(PARSER, ERROR, "User Function %s is already inserted in the Table\n", name);
		LOG_ERROR(PARSER, NOTE, "%s was inserted in line %u\n\n", name, getLine(entry));

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* Check if the loop-only keys are valid */
int Valid_loop_token(char *name, int loop_counter, unsigned int yylineno)
{
	if (loop_counter == 0)
	{
		LOG_ERROR(PARSER, ERROR, "Illegal %s usage at line %u\n", name, yylineno );
		LOG_ERROR(PARSER, NOTE, "%s statement must be used inside a loop \n\n", name);
	
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* Check if the return statement is valid */
int Valid_return(ScopeStack_T stack, unsigned int yylineno)
{
	if(ScopeIsEmpty(stack))
	{
		LOG_ERROR(PARSER, ERROR, "Illegal return outside a Function at line %u\n",yylineno);
		LOG_ERROR(PARSER, NOTE, "return statement must be used inside a function \n\n");
	
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* This Functions generates a name for Functions with no name defined */
char* func_name_generator()
{
	char* name 			 = "$function_";
	char func_number[20];

	sprintf(func_number,"%d",unnamed_func_counter++);

	char* generated_name = malloc((strlen(name)+strlen(func_number))*sizeof(char)+1);
	assert(generated_name);

	strcpy(generated_name,name);
	strcat(generated_name,func_number);

	printf("%s\n",generated_name);

	return  generated_name;
}
