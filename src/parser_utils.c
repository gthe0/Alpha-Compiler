/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* parser_utils.c		            						*/
/* 														    */
/* Implementation of parser_utils.h							*/
/*----------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <log.h>
#include <call.h>
#include <symTable.h>
#include <name_gen.h>
#include <parser_utils.h>
#include <scopeTable.h>

#define NO_OF_LIBFUNCTS 12

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

/**
 * @brief Check if the locals are valid
 *
 * @param name The name of the token
 * @param line The line that we found the token
 * @param scope The current Scope
 *
 * @return the token if found or else NULL
 */
static SymEntry_T Valid_local(char *name, unsigned int line, unsigned int scope)
{
	assert(SymTable_isInit());
	assert(name);

	/* If shadowing of library functions happens then Exit*/
	if (Lib_shadow_check(name) == EXIT_FAILURE)
	{
		LOG_ERROR(PARSER, ERROR, "Shadowing Library Function, line %d, token %s\n", line, name);
		LOG_ERROR(PARSER, NOTE, "%s is a library Function\n\n", name);

		return SymTable_lookup_scope(name, 0);
	}

	return SymTable_lookup_scope(name, scope);
}

/**
 * @brief This Functions checks if the Function Definition is valid
 *
 * @param name The name of the token
 * @param line The line that we found the token
 * @param FromScope The current Scope
 * @param stack The Scope Stack
 *
 * @return The entry found or NULL
 */
static SymEntry_T Valid_Function(char *name, unsigned int line,
								 unsigned int FromScope, ScopeStack_T stack)
{
	assert(SymTable_isInit());
	assert(stack);
	assert(name);

	SymEntry_T entry = NULL;

	/* If shadowing of library functions happens then Exit*/
	if (Lib_shadow_check(name) == EXIT_FAILURE)
	{
		LOG_ERROR(PARSER, ERROR, "Shadowing Library Function, line %d, token %s\n", line, name);
		LOG_ERROR(PARSER, NOTE, "%s is a library Function\n\n", name);

		return SymTable_lookup_scope(name, 0);
	}

	/* If there is already an entry with this name, then it fails*/
	if (entry = SymTable_lookup(name, FromScope, IntStack_Top(stack)))
	{
		if (getScope(entry) == FromScope)
		{
			if (entry->type > FORMAL)
			{
				LOG_ERROR(PARSER, ERROR, "Invalid redeclaration of Function %s in line %u\n", name, line);
				LOG_ERROR(PARSER, NOTE, "Cannot Redeclare Functions\n\n");
			}
			else
			{
				LOG_ERROR(PARSER, ERROR, "Invalid redeclaration of Variable %s to a Function in line %u\n", name, line);
				LOG_ERROR(PARSER, NOTE, "Cannot Redeclare Variables as Functions\n\n");
			}
		}
		else
		{
			LOG_ERROR(PARSER, ERROR, "Token %s was inserted in line %u\n", name, getLine(entry));
		}
	}

	return entry;
}

/* Function to check if the arguments are Valid */
int Valid_args(char *name,
			   unsigned int line, unsigned int FromScope,
			   ScopeStack_T stack)
{
	assert(SymTable_isInit());
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
	if (entry = SymTable_lookup(name, FromScope, IntStack_Top(stack)))
	{
		LOG_ERROR(PARSER, ERROR, "Invalid Formal %s. Token is already inserted in the Table\n", name);
		LOG_ERROR(PARSER, NOTE, "%s was inserted in line %u\n\n", name, getLine(entry));

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
 * @brief This Functions checks if the loop token is valid
 *
 * @param name The name of the token
 * @param loop_counter A counter to see if we are inside a loop.
 * @param yylineno The line that we found the token
 *
 * @return EXIT_FAILURE or EXIT_SUCCESS
 */
static int Valid_loop_token(char *name, int loop_counter, unsigned int yylineno)
{
	if (loop_counter == 0)
	{
		LOG_ERROR(PARSER, ERROR, "Illegal %s usage at line %u\n", name, yylineno);
		LOG_ERROR(PARSER, NOTE, "%s statement must be used inside a loop \n\n", name);

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


/**
* @brief This Functions checks if the return statement is valid
* 
* @param stack The Scope Stack
* @param yylineno The line that we found the token
*
* @return EXIT_FAILURE or EXIT_SUCCESS 
*/
static int Valid_return(ScopeStack_T stack, unsigned int yylineno)
{
	if (IntStack_isEmpty(stack))
	{
		LOG_ERROR(PARSER, ERROR, "Illegal return outside a Function at line %u\n", yylineno);
		LOG_ERROR(PARSER, NOTE, "return statement must be used inside a function \n\n");

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int eval_lvalue(SymEntry_T entry, char *operation, int yylineno)
{
	if (entry == NULL)
		return EXIT_FAILURE;

	/* If Functions are used with a variable operator, then EXIT_FAILURE*/
	if (entry->type == LIBFUNC)
	{
		LOG_ERROR(PARSER, ERROR, "Illegal usage of %s operation in line %u\n", operation, yylineno);
		LOG_ERROR(PARSER, NOTE, "%s is a library Function.\n\n", getName(entry));

		return EXIT_FAILURE;
	}

	if (entry->type == USERFUNC)
	{
		LOG_ERROR(PARSER, ERROR, "Illegal usage of %s operation in line %u\n", operation, yylineno);
		LOG_ERROR(PARSER, NOTE, "%s is a User Function.\n\n", getName(entry));

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*========================= MANAGE FUNCTIONS =======================*/

/* Manages break; and continue; statements */
stmt_T Manage_loop_token(char *name,
						int loop_counter,
						unsigned int yylineno)
{
	stmt_T stmt = new_stmt();
	
	/*Checks if it is a Valid loop token*/
	if (Valid_loop_token(name, loop_counter, yylineno) == EXIT_FAILURE)
		return stmt;

	if (strcmp("break", name) == 0)
		stmt->breaklist = curr_quad_label();
	else if (strcmp("continue", name) == 0)
		stmt->contlist = curr_quad_label();

	emit(jump_i, NULL, NULL, NULL, yylineno, 0);

	return stmt;
}

/* Manage Local ID L_Value*/
SymEntry_T Manage_lv_local(char *name,
						   unsigned yylineno,
						   int scope)
{

	SymEntry_T entry;

	if ((entry = Valid_local(name, yylineno, scope)) == NULL)
	{
		entry = SymEntry_create(scope == 0 ? GLOBAL : LOCAL, name, scope, yylineno);
		Tables_insert_Entry(entry);
	}

	return entry;
}

/* Manage ID L_Value */
SymEntry_T Manage_lv_ID(char *name, unsigned int line,
						unsigned int FromScope, ScopeStack_T stack)
{
	assert(SymTable_isInit());
	assert(ScopeTable_isInit());
	assert(stack);
	assert(name);

	SymEntry_T entry = NULL;

	/* If shadowing of library functions happens then Exit*/
	if (Lib_shadow_check(name) == EXIT_FAILURE)
	{
		return SymTable_lookup_scope(name, 0);
	}

	int isEmpty = IntStack_isEmpty(stack);
	int top = IntStack_Top(stack);

	/* If there is already an entry with this name, then it fails*/
	if (entry = SymTable_lookup(name, FromScope, 0))
	{
		int entry_scope = getScope(entry);

		/* If stack is Empty, do nothing*/
		if (isEmpty)
			return entry;

		if (entry_scope < top && entry->type <= FORMAL && entry->type != GLOBAL)
		{
			LOG_ERROR(PARSER, ERROR, "Token %s out of scope. Function has a minimum scope of %u \n", name, top);
			LOG_ERROR(PARSER, NOTE, "%s was inserted in line %u with a scope of %u. "
									"It would have been accessible if it was a function or initialized within these scopes: %u-%u\n\n",
					  name, getLine(entry), entry_scope, top, FromScope);
		}

		return entry;
	}

	/*If Entry is NULL then it was not found and we insert it*/
	if (entry == NULL)
	{
		if (FromScope == 0)
		{
			entry = SymEntry_create(GLOBAL, name, FromScope, line);
		}
		else if (FromScope > 0)
		{
			entry = SymEntry_create(LOCAL, name, FromScope, line);
		}

		Tables_insert_Entry(entry);
	}

	return entry;
}

/* Function to check if the globals exists */
SymEntry_T Manage_lv_global(char *name, unsigned int line)
{
	assert(SymTable_isInit());
	assert(name);

	SymEntry_T entry = NULL;

	/* If there is not an entry, throw error*/
	if ((entry = SymTable_lookup_scope(name, 0)) == NULL)
	{
		LOG_ERROR(PARSER, ERROR, "Invalid GLOBAL %s in line %u. Token does not exist in Table\n", name, line);
	}

	return entry;
}

/* Manages function id */
SymEntry_T Manage_func_pref(char *name, unsigned int line,
							unsigned int FromScope, ScopeStack_T stack)
{
	SymEntry_T entry;

	if ((entry = Valid_Function(name, line, FromScope, stack)) == NULL)
	{
		entry = SymEntry_create(USERFUNC, name, FromScope, line);
		Tables_insert_Entry(entry);

		return entry;
	}

	return NULL;
}

/* Function to manage arithmetic expressions */
expr *Manage_arithmetic_expr(expr *arg1, expr *arg2,
							 iopcode op, char *context,
							 unsigned scope, unsigned yylineno)
{

	if (!arg1 || !arg2)
		return NULL;

	check_arith(arg1, context);
	check_arith(arg2, context);

	expr *result = newexpr(arithexpr_e);
	result->sym = newtemp(scope, yylineno);

	emit(op, arg1, arg2, result, yylineno, 0);

	return result;
}


/* Manage relation expression */
expr* Manage_rel_expr(expr *arg1, expr *arg2,
					iopcode op, char *context,
					unsigned scope, unsigned yylineno)
{

	if (!arg1 || !arg2)
		return NULL;

	check_arith(arg1, context);
	check_arith(arg2, context);

	expr *result = newexpr(boolexpr_e);
	result->sym = newtemp(scope,yylineno);


	result->true_list = curr_quad_label();
	emit(op,arg1, arg2, NULL, yylineno, 0);
	
	
	result->false_list = curr_quad_label();
	emit(jump_i, NULL, NULL, NULL, yylineno, 0);

	return result;
}



/* Manage return statement */
stmt_T Manage_ret_stmt(ScopeStack_T stack, unsigned yylineno,expr* e)
{
	stmt_T stmt = new_stmt();
	
	/*Checks if it is a Valid return statement */
	if(Valid_return(stack,yylineno) == EXIT_FAILURE)
		return stmt;
	
	emit(ret_i,NULL, NULL, e, yylineno, 0);
	
	stmt->retlist = curr_quad_label();
	
	emit(jump_i, NULL, NULL, NULL, yylineno, 0);

	return stmt;
}

/* Manage assignment */
expr* Manage_assignexpr(expr* lvalue, expr* rvalue,
						unsigned int scope,
						 unsigned int yylineno){

	expr* assignexpr = NULL ;
	assert(lvalue);
	assert(rvalue);

	eval_lvalue(lvalue->sym,"assignment",yylineno);
	
	if(lvalue->type == tableitem_e)
	{
		emit(tablesetelem_i,
			lvalue,
			lvalue->index,
			rvalue,
			yylineno,
			0);

		assignexpr 			= emit_iftableitem(lvalue);
		assignexpr->type 	= assignexpr_e;
	}
	else
	{
		emit(assign_i,
			rvalue,
			NULL,
			lvalue,
			yylineno,
			0);

		assignexpr = newexpr(assignexpr_e);
		assignexpr->sym = newtemp(scope,yylineno);

		emit(assign_i,lvalue,NULL,assignexpr,yylineno,0);
	}

	return assignexpr;
}

/* Manage call->lvalue callsuffix*/
expr* Manage_call_lv_suffix(expr* lvalue, call_T call_suffix){

	expr 	*list = NULL,
			*curr = NULL, 
			*prev = NULL;

	assert(lvalue && call_suffix);

	lvalue = emit_iftableitem(lvalue);

	if(call_suffix->method == 1)
	{
			list = lvalue;
    		curr = call_suffix->elist;
			lvalue = emit_iftableitem(member_item(list,call_suffix->name));
	
		while (curr)
		{
			prev = curr;
			curr = curr->next;
		}

		if(curr == NULL)
		{
			call_suffix->elist = list;
		}
		else
		{
			prev-> next = list;
		}
	
	}

	return make_call(lvalue, call_suffix->elist);;
}


/*Manage member*/
expr* Manage_member(expr* call, expr* index)
{
	call = emit_iftableitem(call);

	expr* temp = newexpr(tableitem_e);
	temp->sym = call->sym;
	temp->index = index;

	return temp;
}



expr* Manage_obj_elist(expr* elist, unsigned scope, unsigned yylineno)
{
	expr* t = newexpr(newtable_e);
	t->sym = newtemp(scope,yylineno);
	
	emit(tablecreate_i, t, NULL, NULL, yylineno, scope);

	for (int i = 0; elist; elist = elist->next)
		emit(tablesetelem_i, t, new_num_expr(i++), elist,yylineno,0);
	
	return t;
}



expr* Manage_obj_indexed(PairList_T index_list, unsigned scope, unsigned yylineno)
{
	expr* t = newexpr(newtable_e);
	t->sym = newtemp(scope,yylineno);
	
	emit(tablecreate_i, t, NULL, NULL, yylineno, scope);

	for ( ;index_list; index_list = index_list->next)
		emit(tablesetelem_i, t, index_list->pair->index, index_list->pair->value ,yylineno,0);
	
	return t;
}



expr* Manage_unary_minus(expr* val, unsigned scope, unsigned yylineno)
{

  	expr *new_expr;
    new_expr = newexpr(arithexpr_e);

    check_arith(val,"unary minus");

    new_expr->sym = newtemp(scope,yylineno);

    emit(uminus_i, val, NULL, new_expr, yylineno, 0);

    return new_expr;

}



expr* Manage_not_expr(expr* val, unsigned scope, unsigned yylineno)
{
	expr* term = newexpr(boolexpr_e);
	term->sym = newtemp(scope,yylineno);
	emit(not_i,val, NULL, term, yylineno, 0);

	return term;
}