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

/* Utility function to find the last jump of a funcstart jump list*/
static unsigned find_last_func_jump(unsigned list)
{	
	int c = 0;
	/* Iterate through funcstart jumps until the list ends */
	while ( list &&
			quad_table[list].op == jump_i	&&
			list + 1 < curr_quad_label() 	&&
			quad_table[list+1].op == funcstart_i)
	{
		c = 1;
		list = quad_table[list].label;
	}

	return c == 0  ? c : list ;
}


/* Patches lists with the label */
static void funstart_patcher(int list, int label)
{
	while (list && list < label)
	{
		int next = quad_table[list].label;
		quad_table[list].label = label;
		list = next;
	}
}


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

	unsigned label = 0;
	unsigned total_quads  =  curr_quad_label() ;

	for (unsigned i = 1 ; i < total_quads; i++)
	{
		label = find_last_func_jump(i);
		funstart_patcher(i,label);
		label = 0;
	}
	
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
