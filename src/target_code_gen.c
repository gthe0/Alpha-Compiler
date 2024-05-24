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
#include <utils.h>
#include <log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Used to Resize tables */
#define EXPAND_SIZE 0x100
#define CURR_SIZE(a, b) (total_##a * sizeof(b))
#define NEW_SIZE(a, b) (EXPAND_SIZE * sizeof(b) + CURR_SIZE(a, b))

#define EXPAND_TABLE(a, b)              \
	if (curr_##a >= total_##a)          \
	{                                   \
		a = realloc(a, NEW_SIZE(a, b)); \
		assert(a);                      \
		total_##a += EXPAND_SIZE;       \
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
	generate_NOP};

/* typedefs of the function Stack */
typedef struct incomplete_jump_t incomplete_jump_t, *InCompleteJump_T;

/* Incomplete jump list */
struct incomplete_jump_t
{
	unsigned instrNo;	   /* Incstruction No. */
	unsigned iaddress;	   /* Address of jump/branch destionation */
	InCompleteJump_T next; /* Next Instruction in list */
};

/* Modules and Variables used  */
static Instruction_T instructions = (Instruction_T)0;
static unsigned total_instructions = 0;
static unsigned curr_instructions = 0;

static InCompleteJump_T ij_list = (InCompleteJump_T)0;
static unsigned ij_total = 0;

static UserFunc_T userFuncs = (UserFunc_T)0;
static unsigned total_userFuncs = 0;
static unsigned curr_userFuncs = 0;

/* Arrays to store Const variable information */
static double *numConsts;
static unsigned total_numConsts = 0;
static unsigned curr_numConsts = 0;

static char **stringConsts;
static unsigned total_stringConsts = 0;
static unsigned curr_stringConsts = 0;

static char **namedLibfuncs;
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

static void make_numberoperand(vmarg_T arg, unsigned val)
{
	arg->val = val;
	arg->type = number_a;

	return;
}

static void make_booloperand(vmarg_T arg, unsigned val)
{
	arg->val = val;
	arg->type = bool_a;

	return;
}

static void make_retvaloperand(vmarg_T arg)
{
	arg->type = retval_a;

	return;
}

/*
 Helper function that returns the current instruction No.
*/
static unsigned nextinstructionlabel()
{
	return curr_instructions;
}

/* This function produces the arguments */
void make_operand(expr *e, vmarg_T arg)
{
	if (!e)
		return;

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
		case programvar:
			arg->type = global_a;
			break;
		case functionlocal:
			arg->type = local_a;
			break;
		case formalarg:
			arg->type = formal_a;
			break;

		default:
			assert(0);
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

		libfuncs_newused((char *)getName(e->sym));

		break;

	case nil_e:

		arg->type = nil_a;
		break;

	default:
		assert(0);
	}

	return;
}

/* Add string s in the next available cell in stringConsts */
void consts_newstring(char *s)
{
	assert(s);

	EXPAND_TABLE(stringConsts, char *);

	stringConsts[curr_stringConsts++] = strdup(s);

	return;
}

/* Add number n in the next available cell in numConsts */
void consts_newnumber(double n)
{
	EXPAND_TABLE(numConsts, double);

	numConsts[curr_numConsts++] = n;

	return;
}

/* Add a new library function in the next available cell in libfuncs_newused */
void libfuncs_newused(char *s)
{
	assert(s);

	/* Prevent duplicate generation */
	for (int i = 0; i < curr_namedLibfuncs; i++)
	{
		if (!strcmp(s, namedLibfuncs[i]))
			return;
	}

	EXPAND_TABLE(namedLibfuncs, char *);

	namedLibfuncs[curr_namedLibfuncs++] = strdup(s);

	return;
}

/* Add a new user function in the next available cell in userFuncs */
void userfuncs_newfunc(SymEntry_T sym)
{
	assert(sym);

	EXPAND_TABLE(userFuncs, userfunc_t);

	/* Prevent duplicate generation */
	for (int i = 0; i < curr_userFuncs; i++)
	{
		if (!strcmp(getName(sym), userFuncs[i].id) && userFuncs[i].address == get_t_address(sym))
			return;
	}

	UserFunc_T func = userFuncs + curr_userFuncs++;

	func->id = getName(sym);
	func->address = get_t_address(sym);
	func->localSize = get_total_locals(sym);

	return;
}

/* Used to emit instructions in the instructions table thorugh shallow copying*/
void emit_instr(instruction t)
{

	EXPAND_TABLE(instructions, instruction)

	instructions[curr_instructions++] = t;
	return;
}

/* Generic generate used for most of the instructions */
void generate(vmopcode op, Quad_T q)
{
	assert(q);

	instruction t = {0};

	t.opcode = op;
	t.srcLine = q->line;

	make_operand(q->arg1, &t.arg1);
	make_operand(q->arg2, &t.arg2);
	make_operand(q->result, &t.result);

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
	t.srcLine = q->line;

	make_operand(q->arg1, &t.arg1);
	make_operand(q->arg2, &t.arg2);

	t.result.type = label_a;

	if (q->label < curr_quad)
		t.result.val = quad_table[q->label].taddress;
	else
		add_incomplete_jump(nextinstructionlabel(), q->label);

	q->taddress = nextinstructionlabel();

	emit_instr(t);
}

/* Generate arithmetic Instructions */
void generate_ADD(Quad_T q) { generate(add_v, q); }
void generate_SUB(Quad_T q) { generate(sub_v, q); }
void generate_MUL(Quad_T q) { generate(mul_v, q); }
void generate_DIV(Quad_T q) { generate(div_v, q); }
void generate_MOD(Quad_T q) { generate(mod_v, q); }

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

	assert(q);

	/*
	 As you can see this is the code of generate
	 but we swap the arg2 make_operand with make_numberoeprand
	 to create a const num operand here
	*/
	instruction t = {0};

	t.opcode = mul_v;
	t.srcLine = q->line;

	make_operand(q->arg1, &t.arg1);
	make_operand(q->result, &t.result);

	/* We make a multiplication with -1 */
	make_numberoperand(&t.arg2, -1);

	q->taddress = nextinstructionlabel();

	emit_instr(t);

	return;
}

/* Generate Assign and Table Creation Instructions */
void generate_ASSIGN(Quad_T q) { generate(assign_v, q); }
void generate_NEWTABLE(Quad_T q) { generate(newtable_v, q); }
void generate_TABLEGETELEM(Quad_T q) { generate(tablegetelem_v, q); }
void generate_TABLESETELEM(Quad_T q) { generate(tablesetelem_v, q); }

void generate_NOP()
{
	instruction t = {0};
	t.opcode = nop_v;

	emit_instr(t);

	return;
}

/* Generate logic transfer Instructions */
void generate_JUMP(Quad_T q) { generate_relational(jump_v, q); }
void generate_IF_EQ(Quad_T q) { generate_relational(jeq_v, q); }
void generate_IF_NOTEQ(Quad_T q) { generate_relational(jne_v, q); }
void generate_IF_GREATER(Quad_T q) { generate_relational(jgt_v, q); }
void generate_IF_GREATEREQ(Quad_T q) { generate_relational(jge_v, q); }
void generate_IF_LESS(Quad_T q) { generate_relational(jlt_v, q); }
void generate_IF_LESSEQ(Quad_T q) { generate_relational(jle_v, q); }

/* Generate call related Instructions */

void generate_CALL(Quad_T q)
{
	instruction t = {0};

	t.opcode = call_v;
	t.srcLine = q->line;

	q->taddress = nextinstructionlabel();

	make_operand(q->arg1, &t.arg1);

	emit_instr(t);
}

void generate_PARAM(Quad_T q)
{
	instruction t = {0};

	t.opcode = pusharg_v;
	t.srcLine = q->line;

	q->taddress = nextinstructionlabel();

	make_operand(q->arg1, &t.arg1);

	emit_instr(t);
}

void generate_GETRETVAL(Quad_T q)
{
	instruction t = {0};

	t.opcode = assign_v;
	t.srcLine = q->line;

	q->taddress = nextinstructionlabel();

	make_operand(q->result, &t.result);
	make_retvaloperand(&t.arg1);

	emit_instr(t);
}

/* Generate function definition related Instructions */
void generate_FUNCSTART(Quad_T q)
{
	Function *func = q->arg1->sym->value.funcVal;

	func->taddress = q->taddress = nextinstructionlabel();
	FuncStack_push(func);

	instruction t = {0};

	t.opcode = funcenter_v;
	t.srcLine = q->line;

	make_operand(q->arg1, &t.result);

	emit_instr(t);
	return;
}

void generate_RETURN(Quad_T q)
{
	q->taddress = nextinstructionlabel();

	instruction t = {0};

	t.opcode = assign_v;
	t.srcLine = q->line;

	make_retvaloperand(&t.result);
	make_operand(q->result, &t.arg1);

	emit_instr(t);

	Function *func = FuncStack_top();
	RetList_insert(&func->retlist, nextinstructionlabel());

	t.opcode = jump_v;

	t.arg1.val = label_a;
	t.arg1.val = 0;

	t.arg2.val = label_a;
	t.arg2.val = 0;

	t.result.type = label_a;

	emit_instr(t);

	return;
}

void generate_FUNCEND(Quad_T q)
{
	Function *func = FuncStack_pop();

	retlist_T list = func->retlist;

	/* Patch return list */
	while (list)
	{
		instructions[list->taddress].result.type = label_a;
		instructions[list->taddress].result.val = nextinstructionlabel();
		list = list->next;
	}

	q->taddress = nextinstructionlabel();
	instruction t={0};

	t.opcode = funcexit_v;
	t.srcLine = q->line;

	make_operand(q->arg1, &t.result);
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
	assert(ij);

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
		if (ij->iaddress == curr_quad_label())
		{
			instructions[ij->instrNo].result.val = nextinstructionlabel();
		}
		else
		{
			instructions[ij->instrNo].result.val = quad_table[ij->iaddress].taddress;
		}
		ij = ij->next;
	}
}

/* Generates the target code */
void generate_target_code(void)
{
	unsigned total = curr_quad_label();

	for (curr_quad = 1; curr_quad < total; curr_quad++)
		(*generators[quad_table[curr_quad].op])(quad_table + curr_quad);

	patch_incomplete_jumps();

	return;
}

/* Number of OUTLINE_CHARs to be printend */
#define OUTLINE_PRINT_NUM 50
#define OUTLINE_CHAR "="

#define OUTLINE_FUNC(a)                         \
	for (int i = 0; i < OUTLINE_PRINT_NUM; i++) \
	{                                           \
		fprintf(ost, OUTLINE_CHAR);             \
		if (i == OUTLINE_PRINT_NUM / 2)         \
			fprintf(ost, a);                    \
	}                                           \
	fprintf(ost, "\n");

/* Function used to prin the numConst array */
static void print_numConsts(FILE *ost)
{
	/* Print a stylized outline for the array */
	OUTLINE_FUNC(" CONSTANT NUMBERS ");

	for (int i = 0; i < curr_numConsts; i++)
		fprintf(ost, "#%-3u %s\n", i, double_to_string(numConsts[i]));

	return;
}

static void print_stringConsts(FILE *ost)
{
	/* Print a stylized outline for the array */
	OUTLINE_FUNC(" CONSTANT STRINGS ");

	for (int i = 0; i < curr_stringConsts; i++)
		fprintf(ost, "#%-3u %s\n", i, add_quotes(stringConsts[i]));

	return;
}

static void print_namedLibfuncs(FILE *ost)
{
	/* Print a stylized outline for the array */
	OUTLINE_FUNC(" LIBRARY FUNCTIONS ");

	for (int i = 0; i < curr_namedLibfuncs; i++)
		fprintf(ost, "#%-3u %s\n", i, add_quotes(namedLibfuncs[i]));

	return;
}

static void print_userFuncs(FILE *ost)
{
	OUTLINE_FUNC(" USER FUNCTIONS ");

	for (int i = 0; i < curr_userFuncs; i++)
		fprintf(ost, "#%-3u, address: %-3u, "
					 "total locals: %-3u, "
					 "id: %s\n",
				i, userFuncs[i].address, userFuncs[i].localSize, userFuncs[i].id);

	return;
}

/* This function here is used to output an arg's info*/
static void write_arg(FILE* ost,vmarg arg)
{
	/* If the argument is a label == 0 with a 0 value, then do not print it*/
	switch (arg.type)
	{
    	case label_a:
		/* If the argument val is 0, then this argument is empty */
			if(arg.val == 0)
				return;

			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(label),",arg.val);
			break;
    	case global_a:
			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(global),",arg.val);
			break; 
    	case formal_a:
			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(formal),",arg.val);
			break;
    	case local_a:
			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(local),",arg.val);
			break;
    	case number_a:
			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(num),",arg.val);
			break;
    	case string_a:
			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(string),",arg.val);
			break;
    	case bool_a:
			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(bool),",arg.val);
			break;
    	case nil_a:
			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(nil),",arg.val);
			break;
    	case userfunc_a:
			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(userfunc),",arg.val);
			break;
    	case libfunc_a:
			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(libfunc),",arg.val);
			break;
    	case retval_a:
			fprintf(ost,"%u%-12s %-3u ",(unsigned)arg.type,"(retval),",arg.val);
			break;	
		default:
			break;
	}

	
	return ;
}

/* Helper function to write an instrucion to the ost stream */
static void write_instr_i(FILE *ost, unsigned i)
{
	fprintf(ost,"#%-3u ",i);

	switch (instructions[i].opcode)
	{
		case assign_v:
			fprintf(ost,"%-15s","assign");
			break;
		/* Airthmetic opcodes */
		case add_v:
			fprintf(ost,"%-15s","add");
			break;
		case sub_v:
			fprintf(ost,"%-15s","sub");
			break;
		case mul_v:
			fprintf(ost,"%-15s","mul");
			break;
		case div_v:
			fprintf(ost,"%-15s","div");
			break;
		case mod_v:
			fprintf(ost,"%-15s","mod");
			break;
		case uminus_v:
			fprintf(ost,"%-15s","uminus");
			break;
	
		/* Jump opcode */
		case jump_v:
			fprintf(ost,"%-15s","jump");
			break;
	
		/* Branch opcodes */
		case jeq_v:
			fprintf(ost,"%-15s","jeq");
			break;
		case jne_v:
			fprintf(ost,"%-15s","jne");
			break;
		case jgt_v:
			fprintf(ost,"%-15s","jgt");
			break;
		case jge_v:
			fprintf(ost,"%-15s","jge");
			break;
		case jlt_v:
			fprintf(ost,"%-15s","jlt");
			break;
		case jle_v:
			fprintf(ost,"%-15s","jle");
			break;
	
		/* call related opcodes */
		case call_v:
			fprintf(ost,"%-15s","call");
			break;
		case pusharg_v:
			fprintf(ost,"%-15s","pusharg");
			break;
	
		/* function defintion related opcode */
		case funcenter_v:
			fprintf(ost,"%-15s","funcenter");
			break;
		case funcexit_v:
			fprintf(ost,"%-15s","funcexit");
			break;
	
		/* Boolean expression related opcode */
		case or_v:
			fprintf(ost,"%-15s","or");
			break;
		case and_v:
			fprintf(ost,"%-15s","and");
			break;
		case not_v:
			fprintf(ost,"%-15s","not");
			break;
	
		/* Table related opcodes */
		case newtable_v:
			fprintf(ost,"%-15s","newtable");
			break;
		case tablegetelem_v:
			fprintf(ost,"%-15s","tablegetelem");
			break;
		case tablesetelem_v:
			fprintf(ost,"%-15s","tablesetelem");
			break;
	
		/* NOP */
		case nop_v:
			fprintf(ost,"%-15s","nop");
			break;
	
		default:
			break;
	}

	/* Print the argument values */
	write_arg(ost,instructions[i].result);
	write_arg(ost,instructions[i].arg1);
	write_arg(ost,instructions[i].arg2);
	
	fprintf(ost,"\n");

	return ;	
}

static void print_instructions(FILE *ost)
{
	OUTLINE_FUNC(" INSTRUCTIONS ");
	fprintf(ost, "\n");


	for (int i = 0; i < curr_instructions; i++)
	{
		write_instr_i(ost,i);
	}

	return;
}

/* Print all the tcg buffers */
void print_tcg_arrays(FILE *ost)
{
	if (!ost)
		return;

	print_numConsts(ost);
	print_stringConsts(ost);
	print_namedLibfuncs(ost);
	print_userFuncs(ost);

	fprintf(ost, "\n");

	print_instructions(ost);
}

#define TCG_WRITE(a) fwrite(&a, sizeof(a), 1, ost)

/* Function used to create binary file */
void createAVMBin(char *BinFileName)
{
	unsigned magicNum = 3401334;
	char *tcgFileName = BinFileName == NULL ? "a.abc" : BinFileName;

	FILE *ost = NULL;

	if (!(ost = fopen(tcgFileName, "wb")))
	{
		LOG_ERROR(TGC, ERROR, "Could not open file stream %s\n", tcgFileName);
		exit(EXIT_FAILURE);
	}

	fclose(ost);
	return;
}