/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* stmt.c			            							*/
/* 														    */
/* Implementations of stmt.h								*/
/*----------------------------------------------------------*/

#include <stdlib.h>
#include <assert.h>

#include <stmt.h>
#include <quad.h>

extern Quad_T quad_table;

/* Merge two lists */
int mergelist(int l1, int l2)
{
	if (!l1)
		return l2;
	else if (!l2)
		return l1;
	else
	{
		int i = l1;
		while (quad_table[i].label)
			i = quad_table[i].label;
		quad_table[i].label = l2;
		return l1;
	}
}

/* Patches lists with the label */
void patchlist(int list, int label)
{
	while (list)
	{
		int next = quad_table[list].label;
		quad_table[list].label = label;
		list = next;
	}
}

/* Initializes a new list member */
int newlist(int i)
{
	quad_table[i].label = 0;
	return i;
}

/* Initializes a statement */
void make_stmt(stmt_T s)
{
	s->retlist = s->breaklist = s->contlist = 0;
}

/* Make a new stmt */
stmt_T new_stmt(void)
{
	stmt_T s = malloc(sizeof(stmt_t));
	assert(s);

	make_stmt(s);

	return s ; 
}

/* Merges two statements */
stmt_T Merge_stmt(stmt_T stmt1, stmt_T stmt2)
{
    stmt_T merged_stmt = new_stmt();
    merged_stmt ->retlist = mergelist(stmt1->retlist,stmt2->retlist);
    merged_stmt ->contlist = mergelist(stmt1->contlist,stmt2->contlist);
    merged_stmt ->breaklist = mergelist(stmt1->breaklist,stmt2->breaklist);
	
	return merged_stmt;
}