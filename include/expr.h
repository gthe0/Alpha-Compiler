/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* expr.h			            							*/
/* 														    */
/* Type definitions and ADT for the expressions				*/
/* encountered during parsing								*/
/*----------------------------------------------------------*/

#ifndef __EXPR_H__
#define __EXPR_H__ 

#include <symTableEntry.h>

/* typedefs of the various structs */
typedef enum expr_t expr_t ;
typedef struct expr expr, *Expr_T;

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

    nil_e,
    undef_e

};

/* expression list ??? */
struct expr
{
	expr_t type;
	expr *index;
	expr *next;
    
    SymEntry_T sym;
    
	double numConst;
    char *strConst;
    unsigned char boolConst;
};


#endif /* expr.h ADT */