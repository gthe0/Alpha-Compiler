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


void dead_code_elimination()
{
	int Current_temp = 0;

	/* If quad table is not initialized, then return */
	if(quad_table == NULL)
		return ;

	for (int i =  curr_quad_label() - 1 ; i > 0 ; i--)
	{
		if(is_temp_expr(quad_table[i].arg1))
		{
		}
	}
	
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

	if(opt & _DEAD_CE)
		dead_code_elimination();

	if(opt & _CONST_PROP)
		const_propagation();

	return ;
}
