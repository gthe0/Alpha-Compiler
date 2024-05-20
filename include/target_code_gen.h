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

#endif /* Target code generation lib*/
