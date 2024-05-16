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

#define _DEAD_CE		0x1
#define _CONST_PROP		0x2

void dead_code_elimination();
void const_propagation();

/**
* @brief A wrapper function to apply optimizations based on opt var 
* 
* @param opt Optimization level opt 
*/
void optimization_level(int opt);

#endif /* Optimization lib */