/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/*----------------------------------------------------------*/
/* ScopeStack.h		            							*/
/* 														    */
/* A simple interface for an integer Stack module			*/
/*----------------------------------------------------------*/

#ifndef __SCOPE_STACK__
#define __SCOPE_STACK__

typedef struct ScopeStack ScopeStack, *ScopeStack_T;

struct  ScopeStack
{
	int isBottom;
	unsigned int scope;
	ScopeStack_T prev;
};

/**
* @brief Initialize Stack
* 
* @return An empty Stack
*/
ScopeStack_T ScopeStack_init(void);

/**
* @brief Free the stack
* @param stack The stack to be freed. 
*/
void ScopeFree(ScopeStack_T stack);

/**
* @brief Returns the Top element of the Stack
* @param stack The stack used. 
*
* @return The Top element of the stack (it is an integer)
*/
unsigned int ScopeTop(ScopeStack_T stack);

/**
* @brief Pops out the Top element of the Stack
* @param stack The stack used. 
*
* @return The Top element of the stack (it is an integer)
*/
unsigned int ScopePop(ScopeStack_T stack);

/**
* @brief Checks if stack is Empty
* @param stack The stack used. 
*
* @return 1 if it Empty or 0 if it is not.
*/
int ScopeIsEmpty(ScopeStack_T stack);

/**
* @brief Push an element in the stack.
* @param stack The stack used. 
* @param scope To be pushed. 
*
* @return true if it has succeded and false if not 
*/
ScopeStack_T ScopePush(ScopeStack_T stack, unsigned int scope);

#endif /* ScopeStack  ADT*/
