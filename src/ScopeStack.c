/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* ScopeStack.h		            							*/
/* 														    */
/* A implementation of the Scope ADT						*/
/*----------------------------------------------------------*/

#include <ScopeStack.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* Return an Empty Stack or NULL */
ScopeStack_T ScopeStack_init(void)
{
	ScopeStack_T new = malloc(sizeof(ScopeStack));

	/* If malloc fails abort */
	assert(new);

	new->prev = NULL;
	new->scope = 0u;

	return new;
}

/* Free the stack */
void ScopeFree(ScopeStack_T stack)
{
	ScopeStack_T temp;
	while(stack)
	{
		temp = stack;
		stack = stack->prev;
		free(temp);
	}
}

/* Push an element in the Stack */
ScopeStack_T ScopePush(ScopeStack_T stack, unsigned int scope)
{
	if(stack == NULL)
		return NULL;

	ScopeStack_T head = malloc(sizeof(ScopeStack));
	
	/* If malloc fails abort */	
	assert(head);

	head->scope = scope;
	head->prev = stack;

	return head;
}

/* Get the top element of the stack */
unsigned int ScopeTop(ScopeStack_T stack)
{
	assert(stack);
	return stack->scope;
}

/* Pop the top of the stack */
unsigned int ScopePop(ScopeStack_T stack)
{
	assert(stack);

	unsigned int top = ScopeTop(stack);
	
	ScopeStack_T temp = stack;

	stack = stack->prev;
	
	free(temp);

	return top;
}