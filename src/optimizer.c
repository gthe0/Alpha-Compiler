/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* optimizer.c		            							*/
/* 														    */
/* Implementation of the library optimizer.h				*/
/*----------------------------------------------------------*/

#include <optimizer.h>

#include <quad.h>
#include <expr.h>

extern Quad_T quad_table;

/*======================= UTILITIES ===========================*/


/*===================== IMPLEMENTATION ========================*/


void useless_temp_elimination()
{
	int Current_temp = 0;

	/* If quad table is not initialized, then return */
	if(quad_table == NULL)
		return ;

	return ;
}


void const_propagation()
{
	/* If quad table is not initialized, then return */
	if(quad_table == NULL)
		return ;

	return ;
}

/*
* Wrapper function for the various optimization level
*
* If Optimization level == 1, do DCE 
* If Optimization level == 2, do Constant Propagation 
* If Optimization level == 3, do BOTH
*/
void optimization_level(int opt)
{

	DO_OPTIMIZATION(opt,_DEAD_CE,useless_temp_elimination);
	DO_OPTIMIZATION(opt,_CONST_PROP,const_propagation);

	return ;
}
