/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* parser_utils.h		            						*/
/* 														    */
/* A library with utility functions used in parser.y		*/
/*----------------------------------------------------------*/

#ifndef __YACC_LIB_H__
#define __YACC_LIB_H__

#include <tables.h>
#include <stmt.h>
#include <quad.h>
#include <forpref.h>
#include <indexed_pair.h>
#include <IntegerStack.h>


/**
* @brief Check if the Arguments are valid
* 
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The current Scope 
* @param stack The Scope Stack
*
* @return EXIT_FAILURE or EXIT_SUCCESS 
*/
int Valid_args(char* name, unsigned int line,
				 unsigned int FromScope, ScopeStack_T stack);

int eval_lvalue(SymEntry_T entry,char* operation, int yylineno);

/**
* @brief Manages break and continue rules
* 
* @param name The name of the token
* @param loop_counter A counter to see if we are inside a loop. 
* @param yylineno The line that we found the token
*
* @return stmt pointer 
*/
stmt_T Manage_loop_token(char *name,
						 int loop_counter,
						 unsigned int yylineno);

/**
* @brief Manages the Local L-value
* 
* @param name The name of the token
* @param yylineno The line that we found the token
* @param scope The scope of the Entry 
*
* @return Return the entry
*/
SymEntry_T Manage_lv_local (char *name,
						 		unsigned  yylineno,
								int scope);

/**
* @brief Manages the ID L-value
* 
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The scope of the Entry 
* @param stack The scope stack of the Entry 
*
* @return Return the entry or a new one
*/
SymEntry_T Manage_lv_ID(char *name, unsigned int line,
 							unsigned int FromScope,ScopeStack_T stack);

/**
* @brief Check if the global token exists
* 
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The current Scope 
* @param stack The Scope Stack
*
* @return The token if found or else return NULL if not found 
*/
SymEntry_T Manage_lv_global(char *name,unsigned int line);

/**
* @brief This Functions checks if the Function Definition is valid
* 
* @param name The name of the token
* @param line The line that we found the token
* @param FromScope The current Scope 
* @param stack The Scope Stack
*
* @return The function symbol 
*/
SymEntry_T Manage_func_pref(char *name, unsigned int line,
					 unsigned int FromScope, ScopeStack_T stack);

/**
* @brief This Functions checks Manages the anonymous function definition
* 
* @param scope The current Scope 
* @param yylineno The line that we found the token
*
* @return The function symbol 
*/
char* Manage_func_name_anonymous(int scope,unsigned yylineno);


/**
* @brief This Functions checks and manages the arithmetic expressions
*
* @param arg1 The first argument of the expression
* @param arg2 The second argument of the expression
* @param op	The opcdode of the instruction
* @param context The context in which it was used. 
* @param scope The current Scope 
* @param yylineno The line that we found the token
*
* @return The function symbol 
*/
expr* Manage_arithmetic_expr(expr* arg1, expr* arg2,
							 iopcode op, char* context,
							 unsigned scope, unsigned yylineno);


/**
* @brief This function manages the return statement
* 
* @param stack The Scope Stack
* @param yylineno The line that we found the token
* @param e	The expression that the function returns
*
* @return the stmt pointer 
*/
stmt_T Manage_ret_stmt(ScopeStack_T stack,unsigned scope, unsigned yylineno, expr* e);

/**
* @brief This function manages the assignexpr
* 
* @param lvalue The l-value of the assignment
* @param rvalue The r-value of the assignment
* @param scope The current Scope 
* @param yylineno The line that we found the assignment
*
* @return the result 
*/
expr* Manage_assignexpr(expr* lvalue, expr* rvalue,
						unsigned int scope,
						 unsigned int yylineno);

/**
* @brief This function manages the call->lvalue callsuffix rule
* 
* @param lvalue The l-value of the assignment
* @param call_suffix The Information of the call struct
*
* @return the result 
*/
expr* Manage_call_lv_suffix(expr* lvalue, call_T call_suffix);

/**
* @brief This function manages the members of the tables
* 
* @param call The result of the call rule
* @param index The index expression of the table
*
* @return the result 
*/
expr* Manage_member(expr* call, expr* index);


/**
* @brief This function manages obj_list->indexed rule
* 
* @param index_list The list of the tables index-value pairs
* @param scope The current Scope 
* @param yylineno The current line
*
* @return the result 
*/
expr* Manage_obj_indexed(PairList_T index_list, unsigned scope, unsigned yylineno);

/**
* @brief This function manages obj_list->elist rule
* 
* @param elist The list of the various expression/members of the table
* @param scope The current Scope 
* @param yylineno The current line
*
* @return the result 
*/
expr* Manage_obj_elist(expr* elist, unsigned scope, unsigned yylineno);

/**
* @brief This function manages unary minus rule
* 
* @param val The expression we want to negate
* @param scope The current Scope 
* @param yylineno The current line
*
* @return the result 
*/
expr* Manage_unary_minus(expr* val, unsigned scope, unsigned yylineno);

/**
* @brief This function manages not expression
* 
* @param val The expression we want to turn into a not boolean expression
* @param scope The current Scope 
* @param yylineno The current line
*
* @return the result 
*/
expr* Manage_not_expr(expr* val, unsigned scope, unsigned yylineno);


/**
* @brief This Functions checks and manages the relationship expressions
*
* @param arg1 The first argument of the expression
* @param arg2 The second argument of the expression
* @param op	The opcdode of the instruction
* @param context The context in which it was used. 
* @param scope The current Scope 
* @param yylineno The line that we found the token
*
* @return The function symbol 
*/
expr* Manage_rel_expr(expr *arg1, expr *arg2,
					iopcode op, char *context,
					unsigned scope, unsigned yylineno);


/**
* @brief This Functions checks and manages the equality expressions
*
* @param arg1 The first argument of the expression
* @param arg2 The second argument of the expression
* @param op	The opcdode of the instruction
* @param context The context in which it was used. 
* @param scope The current Scope 
* @param yylineno The line that we found the token
*
* @return The function symbol 
*/
expr* Manage_eq_expr(expr *arg1, expr *arg2,
					  iopcode op, char *context,
					  unsigned scope, unsigned yylineno);

/**
* @brief This Functions checks and manages the lvalue++ and lvalue-- expressions
*
* @param lvalue The lvalue of the operation
* @param op	The opcdode of the instruction
* @param context The context in which it was used. 
* @param scope The current Scope 
* @param yylineno The line that we found the token
*
* @return The resutling expression (term) 
*/
expr *Manage_lv_arithmetic_right(expr *lvalue, iopcode op,
								 char *context, unsigned scope,
								 unsigned yylineno);


/**
* @brief This Functions checks and manages the ++lvalue and --lvalue expressions
*
* @param lvalue The lvalue of the operation
* @param op	The opcdode of the instruction
* @param context The context in which it was used. 
* @param scope The current Scope 
* @param yylineno The line that we found the token
*
* @return The resutling expression (term) 
*/
expr *Manage_lv_arithmetic_left(expr *lvalue, iopcode op,
								 char *context, unsigned scope,
								 unsigned yylineno);

/**
* @brief This Functions patches things up in and & not expressions.
*
* @param arg1 The first argument of the expression
* @param arg2 The second argument of the expression
* @param op	The opcdode of the instruction
* @param label The label which we use to patch. 
* @param scope The current Scope 
* @param yylineno The line that we found the token
*
* @return The resutling expression (term) 
*/
expr* Manage_conjunctions(expr* arg1, expr*arg2,
						 iopcode op, int label,
						 unsigned scope,
						 unsigned yylineno);


/**
* @brief This function manages the condition of various rules
* 
* @param condition The expression of the condition 
* @param yylineno The line that the rule was met
*
* @return the quad of the jump that we emit 
*/
unsigned int Manage_cond(expr* condition,
						unsigned scope,
						unsigned int yylineno);


/**
* @brief This function manages the whilestmt rule
* 
* @param start The label in which the while stmt started 
* @param cond The label of the condition
* @param loop_stmt The stmt struct that was created in the body
* @param yylineno The line that the rule ends
*
* @return the loop_stmt itself 
*/
stmt_T Manage_while_stmt(unsigned start,unsigned cond,
						stmt_T loop_stmt,unsigned yylineno);


/**
* @brief Manages the if ... else ... rule
* 
* @param ifpref The label at which the if stmt started 
* @param if_stmt The body of the if statement
* @param elsepref The label at which the else stmt started 
* @param else_stmt The body of the else statement
* @param yylineno The line that the rule ends
*
* @return A merged stmt struct from the if and else statements. 
*/
stmt_T Manage_if_else(unsigned ifpref,stmt_T if_stmt,
						unsigned elsepref, stmt_T else_stmt,
						unsigned yylineno);

stmt_T Manage_for_stmt(forpref_T for_prefix, unsigned N1,
						unsigned N2, stmt_T loopstmt, unsigned N3);

#endif /* parser utilities */