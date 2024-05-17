/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* optimizer.h		            							*/
/* 														    */
/* Library used for optimizing the IR through various		*/
/* techniques (e.g DCE, constant propagation etc)			*/
/*----------------------------------------------------------*/

#ifndef __OPTIMIZER_H__
#define __OPTIMIZER_H__

#include <expr.h>

/* typedefs of the various structs used */
typedef struct const_expr_list_t const_expr_list_t, *ConstList_T; 

/* list to store expression to use in const propagation */
struct const_expr_list_t
{
	expr*  e ;

	double numConst;
    char *strConst;
    unsigned char boolConst;

	ConstList_T next;
};

/* Define the various optimization levels... */
#define _DEAD_CE				0x1
#define _CONST_PROP				0x2
#define _ALL_OPTIMIZATIONS		( _DEAD_CE | _CONST_PROP )

#define DO_OPTIMIZATION(a,b,c)	if(a & b) c() ;

/**
* @brief This function eliminates useless temp assignments
*/
void useless_temp_elimination();

/**
* @brief This function propagates const values in-between quads  
*/
void const_propagation();

/**
* @brief A wrapper function to apply optimizations based on opt var 
* 
* @param opt Optimization level opt 
*/
void optimization_level(int opt);

#endif /* Optimization lib */