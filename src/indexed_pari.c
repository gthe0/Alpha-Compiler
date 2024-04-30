/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* indexed_pari.c											*/
/* 														    */
/* Implementation of the indexed_pair.h's various ADTs		*/
/*----------------------------------------------------------*/

#include <indexed_pair.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>


/* Creates a new pair filled with the values passed */
IndexPair_T new_indexed_pair(expr* index, expr* value)
{
	IndexPair_T new_pair = malloc(sizeof(indexed_pair_t));
	assert(new_pair);

	new_pair -> index = index;
	new_pair -> value = value;

	return new_pair;
}



/* Creates a new node for the pair list */
PairList_T PairList_new(void)
{
	PairList_T new_bind = malloc(sizeof(Pair_list_t));
	assert(new_bind);

	new_bind -> pair = NULL;
	new_bind -> next = NULL;

	return new_bind;
}



/* Inserts a new node in the pair list */
PairList_T PairList_insert(IndexPair_T pair, PairList_T next)
{
	PairList_T list = PairList_new();

	list->pair = pair;
	list->next = next;

	return list;
}