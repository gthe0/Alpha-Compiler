/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* ScopeStack.h		            							*/
/* 														    */
/* A simple interface for a Stack ADT that holds the latest	*/
/* function scope											*/
/*----------------------------------------------------------*/

#ifndef __SCOPE_STACK__
#define __SCOPE_STACK__

int ScopeTop();
int ScopePop();
int ScopePush();
int ScopeFree();

#endif /* ScopeStack  ADT*/
