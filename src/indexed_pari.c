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
IndexPair_T new_indexed_pair(expr* index, expr* value,
							unsigned scope, unsigned yylineno)
{
	/* We evaluate the expressions in case of them being boolean */
	short_circuit_eval(index,scope,yylineno);
	short_circuit_eval(value,scope,yylineno);

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



/*
* This function, as you can see, inserts a new Indexed Parir 
* in the end of the list. This is because the Indexed Pair list
* is used only when creating tables and there we need the nodes to
* be in used in order of parsing. 
*/
PairList_T PairList_insert(IndexPair_T pair, PairList_T head)
{
    PairList_T new_node = PairList_new();
    new_node->pair = pair;
    new_node->next = NULL;

    if (head == NULL) 
        return new_node;

    PairList_T current = head;
    
	while (current->next != NULL)
		current = current->next;
    

    current->next = new_node;

    return head; /* Return the updated head. */
}