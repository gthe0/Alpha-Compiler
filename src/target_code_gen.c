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
#include <string.h>
#include <assert.h>

/* Used to Resize tables */
#define EXPAND_SIZE 0x100
#define CURR_SIZE(a) (total_##a * sizeof(a))
#define NEW_SIZE(a) (EXPAND_SIZE * sizeof(a) + CURR_SIZE(a))

#define EXPAND_TABLE(a)		if(curr_##a == total_##a){		\
								a = realloc(a, NEW_SIZE(a));\
								total_##a += EXPAND_SIZE;	\
							}

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
static unsigned curr_instructions = 0;

static InCompleteJump_T ij_list = (InCompleteJump_T) 0;
static unsigned ij_total = 0;

static UserFunc_T userFuncs = (UserFunc_T)0;
static unsigned total_userFuncs = 0;
static unsigned curr_userFuncs = 0;

/* Arrays to store Const variable information */
static double* numConsts;
static unsigned total_numConsts = 0;
static unsigned curr_numConsts = 0;

static char ** stringConsts;
static unsigned total_stringConsts = 0;
static unsigned curr_stringConsts = 0;

static char ** namedLibfuncs;
static unsigned total_namedLibfuncs = 0;
static unsigned curr_namedLibfuncs = 0;

/* The current quad that is processed */
static unsigned curr_quad = 0;

/* The quad table */
extern Quad_T quad_table;

/*
 Helper functions to produce common arguments for
 generated instructions, like 1, 0, "true", "false"
 and function return value
*/

static void make_numberoperand( vmarg_T arg, unsigned val)
{
	arg->val = val;
	arg->type = number_a;

	return ;
}


static void make_booloperand( vmarg_T arg, unsigned val)
{
	arg->val = val;
	arg->type = bool_a;

	return ;
}

static void make_retvaloperand(vmarg_T arg)
{
	arg->type = retval_a;

	return ;
}

/* This function produces the arguments */
void make_operand(expr* e, vmarg_T arg)
{
	if(!e)	return;

	switch (e->type)
	{
		case var_e:
		case assignexpr_e:
		case tableitem_e:
		case arithexpr_e:
		case boolexpr_e:
		case newtable_e:

			assert(e->sym);
			arg->val = getOffset_val(e->sym);

			switch (getSpace_val(e->sym))
			{
				case programvar: arg->type = global_a; break;
				case functionlocal:	arg->type = local_a; break;
				case formalarg:	arg->type = formal_a; break;

				default: assert(0);
			}

			break;

		case constbool_e:
			
			arg->val = e->boolConst;
			arg->type = bool_a;

			break;

		case conststring_e:
			
			arg->val = curr_stringConsts;
			arg->type = string_a;

			consts_newstring(e->strConst);

			break;

		case constnum_e:

			arg->val = curr_numConsts;
			arg->type = number_a;
			
			consts_newnumber(e->numConst);
			
			break;

		case programfunc_e:
			
			arg->val = curr_userFuncs; 
			arg->type = userfunc_a;

			userfuncs_newfunc(e->sym);

			break;

		case libraryfunc_e:

			arg->val = curr_namedLibfuncs; 
			arg->type = libfunc_a;
		
			libfuncs_newused(getName(e->sym));

			break;
		
		case nil_e:

			arg->type = nil_a;
			break;

		default: assert(0);
	}

	return ;
}


/* Add string s in the next available cell in stringConsts */
void consts_newstring(char *s)
{
	assert(s);

	EXPAND_TABLE(stringConsts);
	
	char** string = stringConsts + curr_stringConsts++;
	*string = strdup(s);

	return ;
}

/* Add number n in the next available cell in numConsts */
void consts_newnumber(double n)
{
	EXPAND_TABLE(numConsts);

	double* num = numConsts + curr_numConsts++;
	*num = n;

	return;
}

/* Add a new library function in the next available cell in libfuncs_newused */
void libfuncs_newused(char *s)
{
	assert(s);

	EXPAND_TABLE(namedLibfuncs);
	
	char** string = namedLibfuncs + curr_namedLibfuncs ++;
	*string = strdup(s);

	return ;
}

/* Add a new user function in the next available cell in userFuncs */
void userfuncs_newfunc(SymEntry_T sym)
{
	assert(sym);

	EXPAND_TABLE(userFuncs);

	UserFunc_T func = userFuncs + total_userFuncs++;

	func->id = getName(sym);
	func->address = get_i_address(sym);
	func->localSize = get_total_locals(sym);

	return ;
}

/* Used to emit instructions in the instructions table thorugh shallow copying*/
void emit_instr(instruction t)
{
	EXPAND_TABLE(instructions)

	Instruction_T i = instructions + curr_instructions++;
	*i = t;

	return ;
}

/* Generic generate used for most of the instructions */
void generate(vmopcode op, Quad_T q)
{
	assert(q);

	instruction t = {0};

	t.opcode = op;
	t.srcLine= q->line;
	
	make_operand(q->arg1,&t.arg1); 
	make_operand(q->arg2,&t.arg2); 
	make_operand(q->result,&t.result); 

	emit_instr(t);
}

/*
 Generate function used for control transfer
 instructions (e.g. jumps and branches)
*/
void generate_relational(vmopcode op, Quad_T q)
{
	assert(q);

	instruction t = {0};

	t.opcode = op;
	t.srcLine= q->line;
	
	make_operand(q->arg1,&t.arg1); 
	make_operand(q->arg2,&t.arg2); 

	t.result.type = label_a;

	if (q->label < curr_quad) t.result.val = quad_table[q->label].taddress;
	else add_incomplete_jump(curr_instructions, q->label );
	
	q->taddress = curr_instructions;

	emit_instr(t);
}


/* Generate arithmetic Instructions */
void generate_ADD(Quad_T q) 			{ generate(add_v,q); }
void generate_SUB(Quad_T q) 			{ generate(sub_v,q); }
void generate_MUL(Quad_T q) 			{ generate(mul_v,q); }
void generate_DIV(Quad_T q) 			{ generate(div_v,q); }
void generate_MOD(Quad_T q) 			{ generate(mod_v,q); }

void generate_UMINUS(Quad_T q)
{
	/* 
	 Basically generate_UMINUS will produce a 
	 multiplication of the argument with -1 
	 or we could do something fancier like 
	 xoring the bits and adding 1. But for
	 the purposes of this program and for making it
	 understandable by all, I won't implemenet 
	 the second method yet. It may be done once the VM is ready.
	*/
}

/* Generate Assign and Table Creation Instructions */
void generate_ASSIGN(Quad_T q)          { generate(assign_v,q); }
void generate_NEWTABLE(Quad_T q)        { generate(newtable_v,q); }
void generate_TABLEGETELEM(Quad_T q)    { generate(tablegetelem_v,q); }
void generate_TABLESETELEM(Quad_T q)    { generate(tablesetelem_v,q); }

void generate_NOP()
{
	instruction t = {0};
	t.opcode = nop_v;

	emit_instr(t);

	return ;
}

/* Generate logic transfer Instructions */
void generate_JUMP(Quad_T q)			{ generate_relational(jump_v,q);}
void generate_IF_EQ(Quad_T q)			{ generate_relational(jeq_v,q);}
void generate_IF_NOTEQ(Quad_T q)		{ generate_relational(jne_v,q);}
void generate_IF_GREATER(Quad_T q)		{ generate_relational(jgt_v,q);}
void generate_IF_GREATEREQ(Quad_T q)	{ generate_relational(jge_v,q);}
void generate_IF_LESS(Quad_T q)			{ generate_relational(jlt_v,q);}
void generate_IF_LESSEQ(Quad_T q)		{ generate_relational(jle_v,q);}

/* Generate call related Instructions */

void generate_CALL(Quad_T q)
{
	instruction t = {0};

	t.opcode = call_v;
	t.srcLine = q->line;

	q->taddress = curr_instructions;

	make_operan(q->arg1, &t.arg1);

	emit_instr(t);
}

void generate_PARAM(Quad_T q)
{
	instruction t = {0};

	t.opcode = pusharg_v;
	t.srcLine = q->line;

	q->taddress = curr_instructions;

	make_operan(q->arg1, &t.arg1);

	emit_instr(t);
}

void generate_GETRETVAL(Quad_T q)
{
	instruction t = {0};

	t.opcode = assign_v;
	t.srcLine = q->line;

	q->taddress = curr_instructions;

	make_operan(q->arg1, &t.arg1);
	make_retvaloperand(&t.arg1);

	emit_instr(t);

}

/* Generate function definition related Instructions */
void generate_FUNCSTART(Quad_T q)
{
	Function* func = q->result->sym->value.funcVal;
	
	func->taddress = q->taddress = curr_instructions;
	FuncStack_push(q->result->sym);

	instruction t = {0};
	
	t.opcode = funcenter_v;
	t.srcLine = q->line;

	make_operand(q->result,&t.result);

	emit_instr(t);
	return;
}

void generate_RETURN(Quad_T q)
{
	q->taddress = curr_instructions;
	
	instruction t = {0};

	t.opcode = assign_v;
	t.srcLine = q->line;

	make_retvaloperand(&t.result);
	make_operand(q->arg1,&t.arg1);

	emit_instr(t);

	Function* func = FuncStack_top();
	RetList_insert(&func->retlist,curr_instructions);

	t.opcode = jump_v;

	t.arg1.val = 0;
	t.arg2.val = 0;

	t.result.type = label_a;

	emit_inst(t);

	return;
}

void generate_FUNCEND(Quad_T q)
{
	Function* func = FuncStack_pop();

	retlist_T list = func->retlist;

	while (list)
	{
		instructions[list->taddress].result.type = label_a;
		instructions[list->taddress].result.val = curr_instructions;
		list=list->next;
	}

	q->taddress = curr_instructions;
	instruction t;

	t.opcode = funcexit_v;
	t.srcLine = q->line;

	make_operand(q->result,&t.result);
	emit_instr(t);

	return;
}

/* 
 Generate Boolean Instructions
 These are stub functions because 
 we implemented short-circuiting
*/
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
			instructions[ij->instrNo].result.val = curr_instructions;
		} else {
			instructions[ij->instrNo].result.val = quad_table[ij->iaddress].taddress;
		}
		ij = ij->next;
	}
}