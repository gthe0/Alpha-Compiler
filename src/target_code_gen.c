/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* target_code_gen.c										*/
/* 														    */
/* Implementation of the library target_code_gen			*/
/* that produces the target code run by the Alpha-VM		*/
/*----------------------------------------------------------*/

#include <target_code_gen.h>
#include <quad.h>

/* typedefs of the various structs */
typedef struct incomplete_jump_t incomplete_jump_t, *IncompleteJump_T;
typedef void (*generator_func_t)(Quad_T);

/* Incomplete jump list */
struct incomplete_jump_t
{
	unsigned 			instrNo;	/* Incstruction No. */
	unsigned 			iaddress;	/* Address of jump/branch destionation */
	IncompleteJump_T 	next;		/* Next Instruction in list */
};

/* Generic Generate functions */
void generate(Quad_T q);
void generate_relational(Quad_T q);

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
void generate_TABLEGETELM(Quad_T q);
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

generator_func_t generators[] = {
};