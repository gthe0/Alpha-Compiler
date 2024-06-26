/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* quad.h			            							*/
/* 														    */
/* A simple interfaces for the quads module which will 		*/
/* store the quads in a dynamic resizeable table 			*/
/*----------------------------------------------------------*/


#ifndef QUADS_H
#define QUADS_H

#include <expr.h>

#define IGNORE_FLAG 1

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
    call_i,
    param_i,
    ret_i,
    getretval_i,
    funcstart_i,
    funcend_i,
    tablecreate_i,
    tablegetelem_i,
    tablesetelem_i,
    if_eq_i,
    if_noteq_i,
    if_lesseq_i,
    if_greatereq_i,
    if_less_i,
    if_greater_i,
    jump_i,
	nop_i
};


struct quad{
	iopcode		op;
	expr*		result;
	expr*		arg1;
	expr*		arg2;
	unsigned	label;
	unsigned	line;
	unsigned	ignore;
	unsigned	taddress;
};


/**
* @brief Creates and emits the quad.
* 
* @param op The opcode of the quad.
* @param arg1 The first argument of the quad.
* @param arg2 The second argument of the quad.
* @param result The result argument of the quad.
* @param line The line where the quad was found in the source code
* @param label The label/relative address of the quad (probably used for jumps etc).
*
*/
void emit(
	iopcode 	op,
	expr* 		arg1,
	expr* 		arg2,
	expr* 		result,
	unsigned 	line,
	unsigned 	label
);

/**
* @brief Emits a quad if it is a table item
* @param e The expression to be checked
*
* @return An expression based on whether it is a table item of not
*/
expr* emit_iftableitem(expr* e);

/**
* @brief Getter of next quad label/index
*
* @return currQuad + 1
*/
unsigned int next_quad_label(void);

/**
* @brief Getter of current quad label/index
*
* @return currQuad
*/
unsigned int curr_quad_label(void);

/**
* @brief Getter of current quad label/index
*
* @param quadNo The number/index of the quad
* @param label The label to be patched
*
*/
void patchlabel(unsigned quadNo, unsigned label);

/**
* @brief expands quad table.
*/
void expand (void);

/**
* @brief generates a new table member expr
* 
* @param lv The expression/table
* @param name The name of the expression
*
* @return The member item
*/
expr* member_item (expr* lv, char* name);

/**
* @brief generates a function call and returns result
* 
* @param lv The function call/expression
* @param reversed_elist The reversed argument list
*
* @return The result of the function call
*/
expr* make_call (expr* lv, expr* reversed_elist);

/**
* @brief A function to write the quads in quads.txt
*
* @return EXIT_SUCCESS for success, EXIT_FAILURE for failure
*/
int write_quads(void);

/**
* @brief A function to activate the ignore flag of a quad
* @param index The index of the quad to be ignored by the targt code generation
*
*/
void ignore_instruction(unsigned index);

#endif /* Quads module */
