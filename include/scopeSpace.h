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
* @brief Reset the formal argument offset, makes it 0
*/
void resetformalargoffset(void);

/**
* @brief Reset the Function local offset, makes it 0
*/
void resetfunctionlocaloffset(void);

/**
* @brief Restores the Current Scope Space offset
*/
void restorecurrscopeoffset(unsigned int n);

/**
* @brief Increases the Scope Space counter
*/
void enterscopespace(void);

/**
* @brief Decreases the Scope Space counter
*/
void exitscopespace(void);

#endif /* scopeSpace module */