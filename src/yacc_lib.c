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

#define GLOBAL_SCOPE 0
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


/**
* @brief Check if we shadow library Functions. 
* 
* @param name name of the token
*
* @return EXIT_SUCCESS if we do not, EXIT_FAILURE otherwise.
*/
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
		if (getScope(entry) == FromScope)
		{
			if (entry->type > FORMAL)
			{
				LOG_ERROR(PARSER, ERROR, "Invalid redeclaration of Function %s in line %u\n", name,line);
				LOG_ERROR(PARSER, NOTE, "Cannot Redeclare Functions\n\n");
			}
			else
			{
				LOG_ERROR(PARSER, ERROR, "Invalid redeclaration of Variable %s to a Function in line %u\n", name,line);
				LOG_ERROR(PARSER, NOTE, "Cannot Redeclare Variables as Functions\n\n");
			}
		}
		else
		{
			LOG_ERROR(PARSER, ERROR, "Token %s was inserted in line %u\n", name,getLine(entry));
		}

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
SymEntry_T Valid_lvalue_ID(SymTable_T oSymTable,ScopeTable_T oScopeTable, char *name,
					unsigned int line, unsigned int FromScope,
					ScopeStack_T stack)
{
	assert(oSymTable);
	assert(oScopeTable);
	assert(stack);
	assert(name);

	SymEntry_T entry = NULL;

	/* If shadowing of library functions happens then Exit*/
	if (Lib_shadow_check(name) == EXIT_FAILURE)
	{
		return SymTable_lookup_scope(oSymTable, name, 0);
	}

	int isEmpty = ScopeIsEmpty(stack);
	int top = ScopeTop(stack);

	/* If there is already an entry with this name, then it fails*/
	if (entry = SymTable_lookup(oSymTable, name, FromScope, 0))
	{
		int entry_scope = getScope(entry);
		
		/* If stack is Empty, do nothing*/
		if(isEmpty) return entry;

		if(entry_scope < top && entry->type <= FORMAL)
		{
			LOG_ERROR(PARSER, ERROR, "Token %s out of scope. Function has a minimum scope of %u \n", name, top);
			LOG_ERROR(PARSER, NOTE, "%s was inserted in line %u with a scope of %u. "\
			"It would have been accessible if it was a function or initialized within these scopes: %u-%u\n\n"
			, name, getLine(entry),entry_scope,top,FromScope);

		}

		return entry;
	}

	/*If Entry is NULL then it was not found and we insert it*/
	if (entry == NULL)
	{
		if (FromScope == 0)
		{
			entry = SymEntry_create(GLOBAL,name,FromScope,line);
		}
		else if (FromScope > 0)
		{
			entry = SymEntry_create(LOCAL,name,FromScope,line);
		}

		Tables_insert_Entry(oSymTable,oScopeTable,entry);
	}
	
	return entry;
}

/* Function to check if the locals are Valid */
SymEntry_T Valid_local(SymTable_T oSymTable, char *name,
				unsigned int line, unsigned int scope)
{
	assert(oSymTable);
	assert(name);

	/* If shadowing of library functions happens then Exit*/
	if (Lib_shadow_check(name) == EXIT_FAILURE)
	{
		LOG_ERROR(PARSER, ERROR, "Shadowing Library Function, line %d, token %s\n", line, name);
		LOG_ERROR(PARSER, NOTE, "%s is a library Function\n\n", name);

		return  SymTable_lookup_scope(oSymTable, name, 0);
	}

	return SymTable_lookup_scope(oSymTable, name, scope);
}

/* Function to check if the globals exists */
SymEntry_T find_global(SymTable_T oSymTable, char *name,
				 unsigned int line)
{
	assert(oSymTable);
	assert(name);

	SymEntry_T entry = NULL;

	/* If there is not an entry, throw error*/
	if ((entry = SymTable_lookup_scope(oSymTable, name, 0)) == NULL)
	{
		LOG_ERROR(PARSER, ERROR, "Invalid GLOBAL %s in line %u. Token does not exist in Table\n", name, line);
	}

	return entry;
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

int eval_lvalue(SymEntry_T entry,char* operation, int yylineno)
{
	if(entry == NULL) return EXIT_FAILURE;
	
	/* If Functions are used with a variable operator, then EXIT_FAILURE*/
	if (entry->type == LIBFUNC)
	{
		LOG_ERROR(PARSER, ERROR, "Illegal usage of %s operation in line %u\n",operation,yylineno);
		LOG_ERROR(PARSER, NOTE, "%s is a library Function.\n\n", getName(entry));

		return EXIT_FAILURE;
	}

	if (entry->type == USERFUNC)
	{
		LOG_ERROR(PARSER, ERROR, "Illegal usage of %s operation in line %u\n",operation,yylineno);
		LOG_ERROR(PARSER, NOTE, "%s is a User Function.\n\n", getName(entry));

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}