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

#define GLOBAL_ID 5
#define LOCAL_ID 6

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
		if (!strcmp(LIB_FUNCTIONS[i], name))
		{
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
		LOG_ERROR(PARSER, ERROR, "%s local instance found at line %d\n", name, getLine(entry));
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
int Valid_Function(SymTable_T oSymTable, char *name,
				   unsigned int line, unsigned int FromScope,
				   ScopeStack_T stack)
{
	assert(oSymTable);
	assert(stack);
	assert(name);

	SymEntry_T entry = NULL;

	/* If shadowing of library functions happens then Exit*/
	if (Lib_shadow_check(name) == EXIT_FAILURE)
	{
		LOG_ERROR(PARSER, ERROR, "Shadowing Library Function, line %d, token %s\n", line, name);
		LOG_ERROR(PARSER, NOTE, "%s is a library Function\n\n", name);

		return EXIT_FAILURE;
	}

	/* If there is already an entry with this name, then it fails*/
	if (entry = SymTable_lookup(oSymTable, name, FromScope, ScopeTop(stack)))
	{
		LOG_ERROR(PARSER, ERROR, "Invalid Function %s. Token is already inserted in the Table\n", name);
		LOG_ERROR(PARSER, NOTE, "%s was inserted in line %u\n\n", name, getLine(entry));

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* Function to check if the arguments are Valid */
int Valid_args(SymTable_T oSymTable, char *name,
			   unsigned int line, unsigned int FromScope,
			   ScopeStack_T stack)
{
	assert(oSymTable);
	assert(stack);
	assert(name);

	SymEntry_T entry = NULL;

	/* If shadowing of library functions happens then Exit*/
	if (Lib_shadow_check(name) == EXIT_FAILURE)
	{
		LOG_ERROR(PARSER, ERROR, "Shadowing Library Function, line %d, token %s\n", line, name);
		LOG_ERROR(PARSER, NOTE, "%s is a library Function\n\n", name);

		return EXIT_FAILURE;
	}

	/* If there is already an entry with this name, then it fails*/
	if (entry = SymTable_lookup(oSymTable, name, FromScope, ScopeTop(stack)))
	{
		LOG_ERROR(PARSER, ERROR, "Invalid Formal %s. Token is already inserted in the Table\n", name);
		LOG_ERROR(PARSER, NOTE, "%s was inserted in line %u\n\n", name, getLine(entry));

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* Function to check if the globals exists */
int Valid_lvalue_ID(SymTable_T oSymTable, char *name,
					unsigned int line, unsigned int FromScope,
					ScopeStack_T stack)
{
	assert(oSymTable);
	assert(stack);
	assert(name);

	SymEntry_T entry = NULL;

	/* If shadowing of library functions happens then Exit*/
	if (Lib_shadow_check(name) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	int isEmpty = ScopeIsEmpty(stack);
	int top = ScopeTop(stack);

	/* If there is already an entry with this name, then it fails*/
	if (entry = SymTable_lookup(oSymTable, name, FromScope, 0))
	{
		int entry_scope = getScope(entry);
		
		/* If stack is Empty, do nothing*/
		if(isEmpty) return EXIT_FAILURE;

		if(entry_scope < top && entry->type <= FORMAL)
		{
			LOG_ERROR(PARSER, ERROR, "Token %s out of scope. Function has scope of %u \n", name, top);
			LOG_ERROR(PARSER, NOTE, "%s was inserted in line %u in scope %u. "\
			"It would have been accessible if it was initialized within these scopes: %u-%u\n\n"
			, name, getLine(entry),entry_scope,top,FromScope);
		}

		return EXIT_FAILURE;
	}

	/*If Entry is NULL then it was not found and we insert it*/
	if (entry == NULL)
	{
		if (FromScope == 0)
		{
			return GLOBAL_ID;
		}
		else if (FromScope > 0)
		{
			return LOCAL_ID;
		}
	}
	
	return EXIT_FAILURE;
}

/* Function to check if the locals are Valid */
int Valid_local(SymTable_T oSymTable, char *name,
				unsigned int line, unsigned int scope)
{
	assert(oSymTable);
	assert(name);

	SymEntry_T entry = NULL;

	/* If shadowing of library functions happens then Exit*/
	if (Lib_shadow_check(name) == EXIT_FAILURE)
	{
		LOG_ERROR(PARSER, ERROR, "Shadowing Library Function, line %d, token %s\n", line, name);
		LOG_ERROR(PARSER, NOTE, "%s is a library Function\n\n", name);

		return EXIT_FAILURE;
	}

	/* If there is already an entry with this name, then it fails*/
	if (entry = SymTable_lookup_scope(oSymTable, name, scope))
	{
		LOG_ERROR(PARSER, ERROR, "Invalid Local %s. Token is already inserted in the Table\n", name);
		LOG_ERROR(PARSER, NOTE, "%s was inserted in line %u\n\n", name, getLine(entry));

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* Function to check if the globals exists */
int global_exist(SymTable_T oSymTable, char *name,
				 unsigned int line)
{
	assert(oSymTable);
	assert(name);

	SymEntry_T entry = NULL;

	/* If there is not an entry, throw error*/
	if (SymTable_lookup_scope(oSymTable, name, 0) == NULL)
	{
		LOG_ERROR(PARSER, ERROR, "Invalid GLOBAL %s in line %u. Token does not exist in Table\n", name, line);

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* Check if the loop-only keys are valid */
int Valid_loop_token(char *name, int loop_counter, unsigned int yylineno)
{
	if (loop_counter == 0)
	{
		LOG_ERROR(PARSER, ERROR, "Illegal %s usage at line %u\n", name, yylineno);
		LOG_ERROR(PARSER, NOTE, "%s statement must be used inside a loop \n\n", name);

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* Check if the return statement is valid */
int Valid_return(ScopeStack_T stack, unsigned int yylineno)
{
	if (ScopeIsEmpty(stack))
	{
		LOG_ERROR(PARSER, ERROR, "Illegal return outside a Function at line %u\n", yylineno);
		LOG_ERROR(PARSER, NOTE, "return statement must be used inside a function \n\n");

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* This Functions generates a name for Functions with no name defined */
char *func_name_generator()
{
	char *name = "$function_";
	char func_number[20];

	sprintf(func_number, "%d", unnamed_func_counter++);

	char *generated_name = malloc((strlen(name) + strlen(func_number)) * sizeof(char) + 1);
	assert(generated_name);

	strcpy(generated_name, name);
	strcat(generated_name, func_number);

	return generated_name;
}
