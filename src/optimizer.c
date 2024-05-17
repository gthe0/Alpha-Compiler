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

/* ========================= UTILS ========================= */


/**
* @brief This functions tries to find the last jump of multiple continuous func definitions
*
* @param list The jump list quad index that we pass as a parameter
*
* @return 0, if it fails, the label if it succeeds
*/
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

	/*If we did not get inside the loop, return 0*/
	return c == 0  ? c : list ;
}



/**
* @brief Patches the jump list of continuous function declaration with label
*
* @param list The jump list quad index that we pass as a parameter
* @param label The label to patch the list
*/
static void funstart_patcher(int list, int label)
{

	while (list && list < label)
	{
		int next = quad_table[list].label;
		quad_table[list].label = label;
		list = next;
	}

	return ;
}

/* ========================= IMPLEMENTATION ========================= */

void ignore_useless_quads()
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

	unsigned total_quads  =  curr_quad_label() ;

	for (unsigned i = 1 ; i < total_quads; i++)
	{
		unsigned label = find_last_func_jump(i);
		funstart_patcher(i,label);
	}
	
	return ;
}

/*
* Wrapper function for the various optimization level
*
* If Optimization level == 1, apply ignore_useless_quads 
* If Optimization level == 2, apply funcjump_patchlist 
* If Optimization level == 3, apply BOTH
*/
void optimization_level(int opt)
{

	DO_OPTIMIZATION(opt,_USELESS_ASSIGN,ignore_useless_quads);
	DO_OPTIMIZATION(opt,_FUNC_JUMP_PATCH,funcjump_patchlist);

	return ;
}
