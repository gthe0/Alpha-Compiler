/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* quad.h			            							*/
/* 														    */
/* A simple interfaces for the quads ADT which will 		*/
/* store the quads in a dynamic resizeable table 			*/
/*----------------------------------------------------------*/


#ifndef __QUADS_H__
#define __QUADS_H__

#include <expr.h>

#define _QUAD_FILE_ "quad.txt"

/* Type definitions to use */
typedef struct quad quad, *Quad_T ;
typedef enum iopcode iopcode;

enum iopcode{
	assign_i,
    add_i,
    sub_i,
    mul_i,
    div_i,
    mod_i,
    uminus_i,
    and_i,
    or_i,
    not_i,
    if_eq_i,
    if_noteq_i,
    if_lesseq_i,
    if_greatereq_i,
    if_less_i,
    if_greater_i,
    call_i,
    param_i,
    ret_i,
    getretval_i,
    funcstart_i,
    funcend_i,
    tablecreate_i,
    tablegetelem_i,
    tablesetelem_i,
    jump_i
};


struct quad{
	iopcode		op;
	expr*		result;
	expr*		arg1;
	expr*		arg2;
	unsigned	label;
	unsigned	line;
};


/**
* @brief Creates and emits the quad.
* 
* @param op The opcode of the quad.
* @param result The result argument of the quad.
* @param arg1 The first argument of the quad.
* @param arg2 The second argument of the quad.
* @param line The line where the quad was found in the source code
* @param label The label/relative address of the quad (probably used for jumps etc).
*
*/
void emit(
	iopcode 	op,
	expr* 		result,
	expr* 		arg1,
	expr* 		arg2,
	unsigned 	line,
	unsigned 	label
);

/**
* @brief expands quad table.
*/
void expand (void);

/**
* @brief A function to write the quads in quads.txt
*/
void write_quad(void);

#endif /* Quads module */
