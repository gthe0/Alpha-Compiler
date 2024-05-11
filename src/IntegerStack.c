/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* IntegerStack.c	            							*/
/* 														    */
/* A implementation of the IntStack ADT						*/
/*----------------------------------------------------------*/

#include <IntegerStack.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* Return an Empty Stack or NULL */
IntStack_T IntStack_init(void)
{
	IntStack_T new = malloc(sizeof(IntStack));

	/* If malloc fails abort */
	assert(new);

	new->prev = NULL;
	new->num = 0u;
	new->isEmpty = 1;

	return new;
}

/* Free the stack */
void IntStack_free(ScopeStack_T stack)
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
void IntStack_Push(IntStack_T* stack, unsigned int scope)
{
	if(stack == NULL || *stack == NULL)
		return;

	IntStack_T head = malloc(sizeof(IntStack));
	
	/* If malloc fails abort */	
	assert(head);

	head->num = scope;
	head->prev = *stack;
	head->isEmpty = 0;

	*stack = head ;

	return;
}

/* Get the top element of the stack */
unsigned int IntStack_Top(IntStack_T stack)
{
	assert(stack);
	return stack->num;
}

/* Pop the top of the stack */
unsigned int IntStack_Pop(IntStack_T stack)
{
	assert(IntStack_isEmpty(stack) == 0);

	unsigned int top = IntStack_Top(stack);

	IntStack_T temp = stack->prev;

	*stack = *(temp);
	
	free(temp);

	return top;
}

/* Check if the stack is Empty */
int IntStack_isEmpty(IntStack_T stack)
{
	assert(stack);
	return stack->isEmpty;
}