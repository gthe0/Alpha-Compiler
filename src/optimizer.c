/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* optimizer.c		            							*/
/* 														    */
/* Implementation of the library optimizer.h				*/
/*----------------------------------------------------------*/

#include <string.h>

#include <optimizer.h>
#include <quad.h>
#include <expr.h>

extern Quad_T quad_table;


/* ========================= O1 UTILS ========================= */

#define INSTRUCTION(i)	quad_table[i]
#define OP(i) INSTRUCTION(i).op

#define RES(i)	INSTRUCTION(i).result
#define ARG1(i)	INSTRUCTION(i).arg1
#define ARG2(i)	INSTRUCTION(i).arg2

#define COMP_TEMP(a,b)	is_temp_expr(a) && strcmp(getName(a->sym),getName(b->sym)) == 0

/**
* @brief A Function used to Ignore useless assignments to temp variables
*
* @param k The index of the assignment instruction
* @param sym The symbol of the result
*/
static void ignore_assignments(unsigned k)
{

	unsigned 	i  = k + 1,
				ignore = 0,
				total = curr_quad_label() - 1,
				f = 0;

	while (i < total)
	{
		if (OP(i) == funcstart_i) ++f;

		/* If we reach the end, ignore the quad */		
		if (i >= total || (OP(i) == funcend_i && --f==0) )
		{	
			ignore = 1;
		}

		/* If we reach a branch or the quad is used somewhere, stop the function */		
		else if((OP(i) >= if_eq_i && OP(i) != jump_i) ||
				((COMP_TEMP(ARG1(i),RES(k)))|| (COMP_TEMP(ARG2(i),RES(k)))))
		{
			return ;
		}

		/* If we find the quad reused down the line, then the previous result is ignored*/
		else if(COMP_TEMP(RES(i),RES(k)))
		{
			ignore = 1;
		}

		if (ignore) ignore_instruction(k);

		/* Follow unconditional jump */
		if(OP(i) == jump_i)	i = INSTRUCTION(i).label;
		else i++;
	}
}


/* ========================= O2 UTILS ========================= */

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
	int total_quads = curr_quad_label() ;

	/* Iterate through funcstart jumps until the list ends */
	while ( list &&
			quad_table[list].op == jump_i	&&
			list + 1 < total_quads			&&
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

	/* If quad table is not initialized, then return */
	if(quad_table == NULL)
		return ;
	
	unsigned total_quads  =  curr_quad_label() ;

	for (unsigned i = 1 ; i < total_quads; i++)
	{
		/*If the command is already ignored, skip it*/
		if(INSTRUCTION(i).ignore)
			continue;
		
		/* Ignore useless assignment */
		if (OP(i) == assign_i && is_temp_expr(RES(i)))
			ignore_assignments(i);
		
		/* Ignore inaccessible branches */
		if (OP(i) >= if_eq_i && OP(i) != jump_i);
			//ignore_branch(i);
	}
	
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
