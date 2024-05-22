/* 
*  Authors: csd4881  csd4988  csd5038
*			
* 	func_stack.h
*
*  A simple interface for the FuncStack Module
*/

#ifndef FUNC_STACK_H
#define FUNC_STACK_H

#include <symTableEntry.h>

/**
* @brief Pushes things in the Stack module
* 
* @param func The Function information to be stored
*/
void FuncStack_push(Function* func);


/**
* @brief Pops things out of the Stack module
* 
* @return The top element of the Stack
*/
Function* FuncStack_pop(void);


/**
* @brief Get the Top element of the stack
*
* @return The top element of the Stack
*/
Function* FuncStack_top(void);

#endif /* FuncStack module */