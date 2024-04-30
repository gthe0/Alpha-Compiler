/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* indexed_pari.h											*/
/* 														    */
/* Type definitions and ADTs for the indexed pair 			*/
/* and the Pair list that are used in calls 				*/
/*----------------------------------------------------------*/

#ifndef __INDEXED_PAIR_H__
#define __INDEXED_PAIR_H__

#include <expr.h>

/* typedefs of the various structs */
typedef struct indexed_pair indexed_pair_t, *IndexPair_T;
typedef struct Pair_list Pair_list_t, *PairList_T;

/* Indexed pair Struct */
struct indexed_pair
{
	expr *index;
	expr *value;
};

/* Pair list struct */
struct Pair_list
{
	IndexPair_T pair;
	PairList_T next;
};



/**
* @brief Creates a new, filled indexed pair.
* @param index The index expression of the table.
* @param value The value stored in the table at this particular index.
*
* @return The Pair created
*/
IndexPair_T new_indexed_pair(expr* index, expr* value);


/**
* @brief Creates a new, blank  pair list.
*
* @return The Pair list created
*/
PairList_T PairList_new(void);


/**
* @brief Creates a new, filled pair lsit.
* @param pair The pair to be inserted in the list.
* @param next The next node of the list.
*
* @return The new head of the Pair list.
*/
PairList_T PairList_insert(IndexPair_T pair, PairList_T next);


#endif /* indexed pair and pair list ADTs */