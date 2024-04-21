/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* quad.c			            							*/
/* 														    */
/* An implementation of the quad.h module					*/
/*----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <quad.h>

#define QUAD_FILE "quads.txt"

Quad_T quad_table = NULL;

static unsigned	currQuad = 0u;
static unsigned	total	 = 0u;

/* Resizing table */
#define EXPAND_SIZE 	0x400
#define CURR_SIZE 		(total*sizeof(quad)) 
#define NEW_SIZE		(EXPAND_SIZE*sizeof(quad)+CURR_SIZE)


/* Write quads in the quad.txt file */
void write_quads(void)
{
	return;
}


/* Expand the Quad Table */
void expand(void)
{
	assert(total == currQuad);
	quad_table = realloc(quad_table,NEW_SIZE);
	total+=EXPAND_SIZE;
	return;
}


/* Emit/Create the quad and insert it in the Quad Table */
void emit(iopcode op, expr *result,
		  expr *arg1, expr *arg2,
		  unsigned line, unsigned label)
{
	if (currQuad == total)
		expand();

	Quad_T quad = quad_table + currQuad ++ ;

	quad->op 		= op;
	quad->result 	= result;
	quad->arg1		= arg1;
	quad->arg2		= arg2;
	quad->line		= line;
	quad->label		= label;

	return;
}