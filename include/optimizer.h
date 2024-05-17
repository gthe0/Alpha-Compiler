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

/* Define the various optimization levels... */
#define _USELESS_ASSIGN			0x1
#define _FUNC_JUMP_PATCH		0x2
#define _PEEP_HOLE_OPT			( _USELESS_ASSIGN | _FUNC_JUMP_PATCH )

#define DO_OPTIMIZATION(a,b,c)	if(a & b) c() ;

/**
* @brief This function eliminates useless temp assignments
*/
void useless_temp_elimination();

/**
* @brief This function propagates const values in-between quads  
*/
void funcjump_patchlist();

/**
* @brief A wrapper function to apply optimizations based on opt var 
* 
* @param opt Optimization level opt
*
* If Optimization level == 1, apply useless_temp_elimination 
* If Optimization level == 2, apply funcjump_patchlist 
* If Optimization level == 3, apply BOTH
*/
void optimization_level(int opt);

#endif /* Optimization lib */