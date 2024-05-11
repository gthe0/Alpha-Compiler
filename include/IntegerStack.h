/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* IntegerStack.h		            						*/
/* 														    */
/* A simple interface for an integer Stack ADT				*/
/*----------------------------------------------------------*/

#ifndef __INTEGER_STACK__
#define __INTEGER_STACK__

typedef struct IntStack IntStack, *IntStack_T;
typedef struct IntStack LoopStack, *LoopStack_T;
typedef struct IntStack ScopeStack, *ScopeStack_T;
typedef struct IntStack OffsetStack, *OffsetStack_T;

struct  IntStack
{
	int isEmpty;			/* It equals 1 in the guaed node, else it equals 0 */
	unsigned int num;
	IntStack_T prev;
};

/**
* @brief Initialize Stack
* 
* @return An empty Stack
*/
IntStack_T IntStack_init(void);

/**
* @brief Free the stack
* @param stack The stack to be freed. 
*/
void IntStack_free(IntStack_T stack);

/**
* @brief Returns the Top element of the Stack
* @param stack The stack used. 
*
* @return The Top element of the stack (it is an integer)
*/
unsigned int IntStack_Top(IntStack_T stack);

/**
* @brief Pops out the Top element of the Stack
* @param stack The stack used. 
*
* @return The Top element of the stack (it is an integer)
*/
unsigned int IntStack_Pop(IntStack_T stack);

/**
* @brief Checks if stack is Empty
* @param stack The stack used. 
*
* @return 1 if it Empty or 0 if it is not.
*/
int IntStack_isEmpty(IntStack_T stack);

/**
* @brief Push an element in the stack.
* @param stack The stack to the pointer used. 
* @param scope To be pushed. 
*/
void IntStack_Push(IntStack_T* stack, unsigned int scope);

#endif /* IntStack  ADT*/
