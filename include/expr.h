/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* expr.h			            							*/
/* 														    */
/* Type definitions and ADT for the expressions				*/
/* encountered during parsing								*/
/*----------------------------------------------------------*/

#ifndef EXPR_H
#define EXPR_H 

#include <symTableEntry.h>

/* typedefs of the various structs */
typedef enum expr_t expr_t ;
typedef struct expr expr ;

/* expression types */
enum expr_t
{
    var_e,
    tableitem_e,

    programfunc_e,
    libraryfunc_e,

    arithexpr_e,
    boolexpr_e,
    assignexpr_e,
    newtable_e,

    constnum_e,
    constbool_e,
    conststring_e,

    nil_e
};

/* expression struct */
struct expr
{
	expr_t type;
	expr *index;
	expr *next;
    
    SymEntry_T sym;
    
	double numConst;
    char *strConst;
    unsigned char boolConst;

	int true_list;
	int false_list;
};


/**
* @brief Creates a new blank expression.
* @param type The expression type e.g. variable, function, nil e.t.c.
*
* @return The expression created
*/
expr* newexpr(expr_t type);

/**
* @brief Creates a new constant String expression 
* @param strConst The string used in the expression 
*
* @return The String expression
*/
expr* new_string_expr(char* strConst);

/**
* @brief Creates a new constant boolean expression 
* @param boolConst Value of the boolean expression  
*
* @return The boolean expression
*/
expr* new_bool_expr(unsigned char boolConst);

/**
* @brief Creates a new nil expression 
*
* @return The nil expression
*/
expr* new_nil_expr(void);


/**
* @brief Creates a new constant number expression 
* @param numConst Value of the boolean expression  
*
* @return The constant number expression
*/
expr* new_num_expr(double numConst);

/**
* @brief Generate an lvalue expression
* @param oSymEntry The Symbol Table Entry
*
* @return The lvalue expression
*/
expr* lvalue_expr(SymEntry_T oSymEntry);

/**
* @brief Checks if e is temp
* @param e The expression
*
* @return 1 if it is and 0 if not 
*/
unsigned int is_temp_expr(expr* e);

/**
* @brief Checks if arithmetic expression is valid or not
* @param e The expression
* @param context The context where an error was found
*
* @return 1 if it is and 0 if not 
*/
void check_arith (expr* e, const char* context);

/**
* @brief Creates a new boolean expression with true and false lists
*
* @param scope The scope in which the expression was found
* @param yylineno The line where this function was triggered
*
* @return The new boolean expression
*/
expr* make_bool_expr(unsigned scope,
					unsigned yylineno);

/**
* @brief Emits the instructions neeeded for a boolean expressions
* @param e The expression
* @param scope The scope in which the expression was found
* @param yylineno The line where this function was triggered
*
* @return either the boolean version of e or e itself if already boolean
*/
expr* boolean_create(expr* e, 
					unsigned scope,
					unsigned yylineno);

/**
* @brief Decodes the expression and gets based on its type the part to be printed (Used in write_quads)
* @param e The expression
*
* @return The string of the field that is to be printed based on the type of the expression
*/
const char* expr_decode(expr* e);

/**
* @brief Backpatches lists and emits evaluation instructions 
* @param e The final boolean expression
* @param scope The scope in which the expression was found
* @param yylineno The line where this function was triggered
*
*/
void short_circuit_eval(expr* e, 
						unsigned scope,
						unsigned yylineno);

#endif /* expr.h ADT */