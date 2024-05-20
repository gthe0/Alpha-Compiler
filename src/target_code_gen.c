/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* target_code_gen.c										*/
/* 														    */
/* Implementation of the library target_code_gen			*/
/* that produces the target code run by the Alpha-VM		*/
/*----------------------------------------------------------*/

#include <target_code_gen.h>
#include <symTableEntry.h>
#include <func_stack.h>

#include <stdlib.h>
#include <assert.h>

/*
 This function table here will call the appropriate
 function based on the quad's opcode, which is an enum !

 WARNING!!!! : If you want to reorder the functions,
 please also reorder the fields of the iopcode enum in quad.h
*/
generator_func_t generators[] = {
	generate_ASSIGN,
    generate_ADD,
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD,
    generate_UMINUS,
    generate_AND,
    generate_OR,
    generate_NOT,
    generate_CALL,
    generate_PARAM,
    generate_RETURN,
    generate_GETRETVAL,
    generate_FUNCSTART,
    generate_FUNCEND,
    generate_NEWTABLE,
    generate_TABLEGETELEM,
    generate_TABLESETELEM,
    generate_IF_EQ,
    generate_IF_NOTEQ,
    generate_IF_LESSEQ,
    generate_IF_GREATEREQ,
    generate_IF_LESS,
    generate_IF_GREATER,
    generate_JUMP,
	generate_NOP
};

/* typedefs of the function Stack */
typedef struct incomplete_jump_t incomplete_jump_t, *InCompleteJump_T;

/* Incomplete jump list */
struct incomplete_jump_t
{
	unsigned 			instrNo;	/* Incstruction No. */
	unsigned 			iaddress;	/* Address of jump/branch destionation */
	InCompleteJump_T 	next;		/* Next Instruction in list */
};

/* Modules and Variables used  */
static Instruction_T instructions = (Instruction_T)0;
static unsigned total_instructions = 0;

static InCompleteJump_T ij_list = (InCompleteJump_T) 0;
static unsigned ij_total = 0;

static UserFunc_T userFuncs = (UserFunc_T)0;
static unsigned totalUserFuncs = 0;

/* Arrays to store Const variable information */
static double* numConsts;
static unsigned totalNumConsts = 0;

static char ** stringConsts;
static unsigned totalStringConsts = 0;

static char ** namedLibfuncs;
static unsigned totalNamedLibfuncs = 0;

/* The quad table */
extern Quad_T quad_table;


/* Add string s in the next available cell in stringConsts and return the index */
unsigned int consts_newstring(char *s)
{
	assert(s);

	if (!stringConsts)
	{
		stringConsts = malloc(sizeof(char *));
		stringConsts[0] = s;
		return 0;
	}

	unsigned int i = 0;
	while (stringConsts[i])
		i++;

	stringConsts = realloc(stringConsts, (i + 1) * sizeof(char *));
	stringConsts[i] = s;

	totalStringConsts++;
	return i;
}

/* Add number n in the next available cell in numConsts and return the index */
unsigned int consts_newnumber(double n)
{
	if (!numConsts)
	{
		numConsts = malloc(sizeof(double));
		numConsts[0] = n;
		return 0;
	}

	unsigned int i = 0;
	while (numConsts[i])
		i++;

	numConsts = realloc(numConsts, (i + 1) * sizeof(double));
	numConsts[i] = n;

	totalNumConsts++;
	return i;
}

/* Add a new library function in the next available cell in libfuncs_newused and return the index */
unsigned int libfuncs_newused(char *s)
{
	assert(s);

	if (!namedLibfuncs)
	{
		namedLibfuncs = malloc(sizeof(char *));
		namedLibfuncs[0] = s;
		return 0;
	}

	unsigned int i = 0;
	while (namedLibfuncs[i])
		i++;

	namedLibfuncs = realloc(namedLibfuncs, (i + 1) * sizeof(char *));
	namedLibfuncs[i] = s;

	totalNamedLibfuncs++;
	return i;
}

/* Add a new user function in the next available cell in userFuncs and return the index */
unsigned int userfuncs_newfunc(SymEntry_T sym)
{
	assert(sym);

	if (!userFuncs)
	{
		userFuncs = malloc(sizeof(struct userfunc));
		userFuncs[0].address = sym->type;
		userFuncs[0].localSize = get_total_locals(sym);
		userFuncs[0].id = getName(sym);
		return 0;
	}

	unsigned int i = 0;
	while (userFuncs[i].id)
		i++;

	userFuncs = realloc(userFuncs, (i + 1) * sizeof(struct userfunc));
	userFuncs[i].address = get_t_address(sym);
	userFuncs[i].localSize = get_total_locals(sym);
	userFuncs[i].id = getName(sym);

	totalUserFuncs++;
	return i;
}


/* Generate arithmetic Instructions */
void generate_ADD(Quad_T q) 			{ generate(add_v,q); }
void generate_SUB(Quad_T q) 			{ generate(sub_v,q); }
void generate_MUL(Quad_T q) 			{ generate(mul_v,q); }
void generate_DIV(Quad_T q) 			{ generate(div_v,q); }
void generate_MOD(Quad_T q) 			{ generate(mod_v,q); }

void generate_UMINUS(Quad_T q)
{

}

/* Generate Assign and Table Creation Instructions */
void generate_ASSIGN(Quad_T q)          { generate(assign_v,q); }
void generate_NEWTABLE(Quad_T q)        { generate(newtable_v,q); }
void generate_TABLEGETELEM(Quad_T q)    { generate(tablegetelem_v,q); }
void generate_TABLESETELEM(Quad_T q)    { generate(tablesetelem_v,q); }

/* Generate logic transfer Instructions */
void generate_JUMP(Quad_T q)			{ generate_relational(jump_v,q);}
void generate_IF_EQ(Quad_T q)			{ generate_relational(jeq_v,q);}
void generate_IF_NOTEQ(Quad_T q)		{ generate_relational(jne_v,q);}
void generate_IF_GREATER(Quad_T q)		{ generate_relational(jgt_v,q);}
void generate_IF_GREATEREQ(Quad_T q)	{ generate_relational(jge_v,q);}
void generate_IF_LESS(Quad_T q)			{ generate_relational(jlt_v,q);}
void generate_IF_LESSEQ(Quad_T q)		{ generate_relational(jle_v,q);}

/* Generate call related Instructions */
void generate_CALL(Quad_T q)			{}
void generate_PARAM(Quad_T q)			{}
void generate_GETRETVAL(Quad_T q)		{}

/* Generate function definition related Instructions */
void generate_FUNCSTART(Quad_T q);
void generate_RETURN(Quad_T q);
void generate_FUNCEND(Quad_T q);

/* 
 Generate Boolean Instructions
 These are stub functions because we implemented short-circuiting
*/
void generate_NOP() {  }
void generate_OR(Quad_T q) {}
void generate_AND(Quad_T q) {}
void generate_NOT(Quad_T q) {}


/* Function used to add a node to the list */
void add_incomplete_jump(unsigned instrNo, unsigned iaddress) 
{
	InCompleteJump_T ij = malloc(sizeof(incomplete_jump_t));

	ij->instrNo = instrNo;
	ij->iaddress = iaddress;
	ij->next = ij_list;
	ij_list = ij;
	ij_total++;
}

/* 
 This function patches the incomplete jump instructions stored
 in the instructions table and whose info is stored in the ij_list
*/
void patch_incomplete_jumps(void) 
{
	InCompleteJump_T ij = ij_list;

	while (ij) 
	{
		if (ij->iaddress == curr_quad_label()) {
			instructions[ij->instrNo].result->val = total_instructions;
		} else {
			instructions[ij->instrNo].result->val = quad_table[ij->iaddress].taddress;
		}
		++ij;
	}
}