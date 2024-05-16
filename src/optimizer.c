/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* optimizer.c		            							*/
/* 														    */
/* Implementation of the library optimizer.h				*/
/*----------------------------------------------------------*/

#include <optimizer.h>
#include <expr.h>


/*======================= UTILITIES ===========================*/


/*===================== IMPLEMENTATION ========================*/


void dead_code_elimination()
{



}


void const_propagation()
{

}

/*
* Wrapper function for the various optimization level
*
* If O level == 1, do DCE 
* If O level == 2, do Constant Propagation 
* If O level == 3, do BOTH
*/
void optimization_level(int opt)
{

	if(opt & _DEAD_CE)
		dead_code_elimination();

	if(opt & _CONST_PROP)
		const_propagation();

	return ;
}
