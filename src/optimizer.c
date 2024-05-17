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


void useless_temp_elimination()
{
	int Current_temp = 0;

	/* If quad table is not initialized, then return */
	if(quad_table == NULL)
		return ;

	return ;
}


void funcjump_patchlist()
{
	/* If quad table is not initialized, then return */
	if(quad_table == NULL)
		return ;

	return ;
}

/*
* Wrapper function for the various optimization level
*
* If Optimization level == 1, apply useless_temp_elimination 
* If Optimization level == 2, apply funcjump_patchlist 
* If Optimization level == 3, apply BOTH
*/
void optimization_level(int opt)
{

	DO_OPTIMIZATION(opt,_USELESS_ASSIGN,useless_temp_elimination);
	DO_OPTIMIZATION(opt,_FUNC_JUMP_PATCH,funcjump_patchlist);

	return ;
}
