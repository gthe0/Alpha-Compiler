/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* stmt.h			            							*/
/* 														    */
/* Interface for break and continue lists					*/
/*----------------------------------------------------------*/

#ifndef STMT_H
#define STMT_H

typedef struct stmt_t stmt_t, *stmt_T;

struct stmt_t
{
	int breaklist, contlist, retlist;
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
* @brief Initializes new list
* 
* @param i Index of stmt in the quad_table
* 
* @return i index unmodified 
*/
int newlist(int i);


/**
* @brief Initializes new statement 
* 
* @param label Pointer to the statement
* 
*/
void make_stmt(stmt_t *s);


/**
* @brief Makes a new stm_T 
* 
* @return s, The new statement initialized with 0s
* 
*/
stmt_T new_stmt(void);

/**
* @brief Merges two statements lists into a single one 
* 
* @param stmt1 The first statement
* @param stmt2 The second statement
*
* @return A new, merged statement 
*/
stmt_T Merge_stmt(stmt_T stmt1, stmt_T stmt2);

#endif