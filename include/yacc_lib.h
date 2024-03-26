/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* yacc_lib.h		            							*/
/* 														    */
/* A library with utility functions used in parser.y		*/
/*----------------------------------------------------------*/

#ifndef __YACC_LIB_H__
#define __YACC_LIB_H__

#include <symTable.h>
#include <ScopeStack.h>

int lvalue_local(SymTable_T oSymTable,char* name,int scope);
int lvalue_global(SymTable_T oSymTable,char* name);
int lvalue_Function(SymTable_T oSymTable,char* name,int FromScope,ScopeStack_T stack);

#endif /* Yacc lib */