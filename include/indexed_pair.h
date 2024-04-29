#ifndef __INDEXED_PAIR_H__
#define __INDEXED_PAIR_H__

#include <expr.h>


typedef struct indexed_pair indexed_pair_t, *IndexPair_T;
typedef struct Pair_list Pair_list_t, *PairList_T;

struct indexed_pair
{
	expr *index;
	expr *value;
};

struct Pair_list
{
	IndexPair_T pair;
	PairList_T next;
};



IndexPair_T new_indexed_pair(expr* key, expr* value);
PairList_T PairList_new(void);
PairList_T PairList_insert(IndexPair_T pair, PairList_T next);


#endif