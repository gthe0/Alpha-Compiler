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
typedef struct funcStack_t funcStack_t, *FuncStack_T;


/* Incomplete jump list */
struct incomplete_jump_t
{
	unsigned 			instrNo;	/* Incstruction No. */
	unsigned 			iaddress;	/* Address of jump/branch destionation */
	InCompleteJump_T 	next;		/* Next Instruction in list */
};

/*
 Stack Used to store Function Information.
 We will implement it as a module only in this file here.
*/
struct funcStack_t{
	Function* top;
	FuncStack_T	prev;	
};

/* Modules and Variables used  */
static InCompleteJump_T ij_list = NULL;
static FuncStack_T funcStack = NULL;


/**
* @brief Utility function used to push things in the Stack module
* 
* @param sym The entry containing the Function struct
*/
static void FuncStack_push(SymEntry_T sym)
{
	FuncStack_T t = malloc(sizeof(funcStack_t));
	
	if (!sym || sym->type < USERFUNC)
		return;

	t->top = sym->value.funcVal;
	t->prev = funcStack;
	funcStack = t;
}

/**
* @brief Utility function used to pop things out of the Stack module
*
* @return The top element of the stack 
*/
static Function* FuncStack_pop(void)
{
	/*
	 If the funcStack is NULL, then that means
	 that the quads or IR are Invalid and 
	 we abort the target code generation
	*/
	assert(funcStack);


	FuncStack_T t = funcStack;
	Function* top = t->top ;

	funcStack = funcStack->prev;
	free(t);

	return top;
}





/* Generate arithmetic Instructions */
void generate_ADD(Quad_T q) 			{ generate(add_v,q); }
void generate_SUB(Quad_T q) 			{ generate(sub_v,q); }
void generate_MUL(Quad_T q) 			{ generate(mul_v,q); }
void generate_DIV(Quad_T q) 			{ generate(div_v,q); }
void generate_MOD(Quad_T q) 			{ generate(mod_v,q); }

void generate_UMINUS(Quad_T q)	{

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

