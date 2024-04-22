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
#include <log.h>

#include <tables.h>
#include <symTable.h>
#include <scopeTable.h>


#define QUAD_FILE "quads.txt"

Quad_T quad_table = NULL;

static unsigned	int currQuad = 0u;
static unsigned	int total	 = 0u;

extern int yylineno;

/* Resizing table */
#define EXPAND_SIZE 	0x400
#define CURR_SIZE 		(total*sizeof(quad)) 
#define NEW_SIZE		(EXPAND_SIZE*sizeof(quad)+CURR_SIZE)


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

/*Generates a Quad whether e is a table item of not*/
expr* emit_iftableitem(	expr* e,
						unsigned scope,
						unsigned yylineno )
{
	if(e->type != tableitem_e)
		return e;


	expr* result = newexpr(var_e);
	result->sym  = newtemp(scope,yylineno);
	emit(
		tablegetelem_i,
		e,
		e->index,
		result,
		yylineno,
		0
	);
	return result;
}


/* Expand the Quad Table */
void expand(void)
{
	assert(total == currQuad);
	quad_table = realloc(quad_table,NEW_SIZE);
	total+=EXPAND_SIZE;
	return;
}

/* patch label */
void patchlabel(unsigned quadNo, unsigned label)
{
	assert(quadNo < currQuad);
	quad_table[quadNo].label = label;
}


/* Get next Quad */
unsigned int next_quad_label(void)
{
	return (currQuad + 1);
}

/* Get current quad */
unsigned int curr_quad_label(void)
{
	return (currQuad);
}

static void quad_decode(FILE* ost, unsigned  index)
{
	if(!ost)
	{
		LOG_ERROR(PARSER, ERROR, "Cannot write to specified file %s\n",QUAD_FILE);
		return ;
	}
	
	return ;
}

/* Write quads in the quad.txt file */
int write_quads(void)
{
	FILE* ost;

	/* If the output file is not provided use ost */
	if(!(ost = fopen(QUAD_FILE,"w")))
	{
		LOG_ERROR(PARSER, ERROR, "Cannot write to specified file %s\n",QUAD_FILE);
		return EXIT_FAILURE;
	}

	for (unsigned i = 0; i < currQuad ; i++)
		quad_decode(ost, i);


	return EXIT_SUCCESS;
}
