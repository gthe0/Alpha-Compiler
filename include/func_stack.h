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
* @param sym The entry containing the Function struct
*/
void FuncStack_push(SymEntry_T sym);


/**
* @brief Pops things out of the Stack module
* 
* @return The top element of the Stack
*/
Function* FuncStack_pop(void);

#endif /* FuncStack module */