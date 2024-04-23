/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* stmt.h			            							*/
/* 														    */
/* Interface for break and continue lists					*/
/*----------------------------------------------------------*/

#ifndef __STMT_H__
#define __STMT_H__

typedef struct stmt_t stmt_t, *stmt_T;

struct stmt_t
{
	int breakList, contList;
};


/**
* @brief Merges two lists 
* 
* @param l1 1st list
* @param l2 2nd list
* 
* @return Returns the merged list index
*/
int mergelist(int l1, int l2);


/**
* @brief Patches list with the label 
* 
* @param list list to patch
* @param label Label used for patching
* 
*/
void patchlist(int list, int label);


/**
* @brief Initializes new statement 
* 
* @param label Pointer to the statement
* 
*/
void make_stmt(stmt_t *s);


/**
* @brief Initializes new list
* 
* @param i Index of stmt in the quad_table
* 
* @return i index unmodified 
*/
int newlist(int i);

#endif