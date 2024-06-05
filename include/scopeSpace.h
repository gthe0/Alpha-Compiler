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
ScopeSpace curr_scope_space(void);

/**
* @brief Calculates the current Scope Space offset
*
* @return The Scope Space offset.
*/
unsigned curr_scope_offset(void);

/**
* @brief Increases the Current Scope Space offset
*/
void inccurr_scope_offset(void);

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
void restore_curr_scope_offset(unsigned int n);

/**
* @brief Getter of programVarOffset
* @return programVarOffset 
*/
unsigned programVarOffset_getter(void);

/**
* @brief Increases the Scope Space counter
*/
void enterscopespace(void);

/**
* @brief Decreases the Scope Space counter
*/
void exitscopespace(void);

#endif /* scopeSpace module */