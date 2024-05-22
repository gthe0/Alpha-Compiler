/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* target_code_gen.h										*/
/* 														    */
/* Library used to turn quads into the Alpha-binary used	*/
/* by the Alpha Virtual Machine (AVM) to interpret it		*/
/* and run the program, live the JVM does with byte code	*/
/*----------------------------------------------------------*/

#ifndef __TARGET_CODE_GEN_H__
#define __TARGET_CODE_GEN_H__

#include <quad.h>
#include <avm-types.h>

/* typedefs of the various structs */
typedef void (*generator_func_t)(Quad_T);

/* Generic Generate functions */
void generate(vmopcode op, Quad_T q);
void generate_relational(vmopcode op, Quad_T q);

/* Generate arithmetic Instructions */
void generate_ADD(Quad_T q);
void generate_SUB(Quad_T q);
void generate_MUL(Quad_T q);
void generate_DIV(Quad_T q);
void generate_MOD(Quad_T q);
void generate_UMINUS(Quad_T q);

/* Generate logic transfer Instructions */
void generate_JUMP(Quad_T q);
void generate_IF_EQ(Quad_T q);
void generate_IF_NOTEQ(Quad_T q);
void generate_IF_GREATER(Quad_T q);
void generate_IF_GREATEREQ(Quad_T q);
void generate_IF_LESS(Quad_T q);
void generate_IF_LESSEQ(Quad_T q);

/* Generate Assign and Table Creation Instructions */
void generate_NOP();
void generate_ASSIGN(Quad_T q);
void generate_NEWTABLE(Quad_T q);
void generate_TABLEGETELEM(Quad_T q);
void generate_TABLESETELEM(Quad_T q);

/* Generate call related Instructions */
void generate_CALL(Quad_T q);
void generate_PARAM(Quad_T q);
void generate_GETRETVAL(Quad_T q);

/* Generate function definition related Instructions */
void generate_FUNCSTART(Quad_T q);
void generate_RETURN(Quad_T q);
void generate_FUNCEND(Quad_T q);

/* Generate Boolean Instructions */
void generate_OR(Quad_T q);
void generate_AND(Quad_T q);
void generate_NOT(Quad_T q);


/* INCOMPLETE JUMP MODULE FUNCTIONS */

/**
* @brief Adds a node to the Incomplete Jump module (IJ Module)
* 
* @param instrNo The instruction Number
* @param iaddress The instruction address
*/
void add_incomplete_jump(unsigned instrNo, unsigned iaddress);

/**
* @brief Patches all the incomplete jumps using the info stored in the IJ Module 
*/
void patch_incomplete_jumps(void);

void printfffff();
/* VECOTR MANAGEMENT FUNCTIONS */

/**
* @brief Appends userfunc table with the info stored in sym 
* 
* @param sym The symbol holding the userfunc information
*/
void userfuncs_newfunc(SymEntry_T sym);


/**
* @brief Appends const_string table with the string s 
* 
* @param s String to be pushed back in the table
*/
void consts_newstring(char *s);


/**
* @brief Appends const_number table with the number n 
* 
* @param n The number to be pushed back in the table
*/
void consts_newnumber(double n);


/**
* @brief Appends library function table with the name of the function 
* 
* @param s The name of the lib function to be pushed back in the table
*/
void libfuncs_newused(char *s);

/**
* @brief Used to emit a new instruction in the instruction table 
* 
* @param t The instruction to be emitted and to be pushed back in the table
*/
void emit_instr(instruction t);

/**
* @brief Generates the needed vmarg from the expression e 
*
* @param e The expression of the quad
* @param arg The address of the vmarg that will be produced
*/
void make_operand(expr* e, vmarg_T arg);

/**
* @brief Generates the target code in the instructions table
*/
void generate_target_code(void);

/**
* @brief Print the contents of the tcg buffers 
*
* @param ost The output stream
*/
void print_tcg_arrays(FILE* ost);

#endif /* Target code generation lib*/
