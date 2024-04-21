/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* scopeSpace.h		            							*/
/* 														    */
/* A simple interface for a Scope Space module				*/
/*----------------------------------------------------------*/

#ifndef _SCOPE_SPACE_H_
#define _SCOPE_SPACE_H_

typedef enum scopespace_t ScopeSpace;

/*
*  Enums for scope space types
*/
enum scopespace_t
{
    programvar,
    functionlocal,
    formalarg
};

/**
* @brief Calculates the current ScopeSpace
*
* @return The Scope Space.
*/
ScopeSpace currscopespace(void);

/**
* @brief Calculates the current Scope Space offset
*
* @return The Scope Space offset.
*/
unsigned currscopeoffset(void);

/**
* @brief Increases the Current Scope Space offset
*/
void inccurrscopeoffset(void);

/**
* @brief Increases the Scope Space counter
*/
void enterscopespace(void);

/**
* @brief Decreases the Scope Space counter
*/
void exitscopespace(void);

/**
* @brief Restores the Current Scope Space offset
*/
void restorecurrscopeoffset(unsigned int n);

#endif /* scopeSpace module */