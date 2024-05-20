/* 
*  Authors: csd4881  csd4988  csd5038
*			
* 	func_stack.c
*
*  Implementation of the FuncStack Module
*/

#include <func_stack.h>

#include <assert.h>
#include <stdlib.h>

typedef struct funcStack_t funcStack_t, *FuncStack_T;

/*
 Stack Used to store Function Information.
 We will implement it as a module only in this file here.
*/
struct funcStack_t{
	Function* top;
	FuncStack_T	prev;	
};

static FuncStack_T funcStack = (FuncStack_T)0;

/* Pushes things in funcStack */
void FuncStack_push(SymEntry_T sym)
{
	FuncStack_T t = malloc(sizeof(funcStack_t));
	
	if (!sym || sym->type < USERFUNC)
		return;

	t->top = sym->value.funcVal;
	t->prev = funcStack;
	funcStack = t;
}

/* Pops things out of funcStack */
Function* FuncStack_pop(void)
{
	/*
	 If the funcStack is NULL, then that means
	 that the quads or IR are Invalid and 
	 we abort the target code generation
	*/
	assert(funcStack);


	FuncStack_T t = funcStack;
	Function* top = t->top ;

	funcStack = funcStack->prev;
	free(t);

	return top;
}


