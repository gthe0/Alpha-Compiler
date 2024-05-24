/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* scopeSpace.c		            							*/
/* 														    */
/* An implementation scopeSpace.h module					*/
/*----------------------------------------------------------*/

#include <scopeSpace.h>

#include <assert.h>

static unsigned int programVarOffset	= 0 ;
static unsigned int functionLocalOffset = 0 ;
static unsigned int formalArgOffset 	= 0 ;
static unsigned int scopeSpaceCounter 	= 1 ;

/* Get current Scope Space */
ScopeSpace curr_scope_space(void)
{
    if (scopeSpaceCounter == 1)
    {
        return programvar;
    }
    else if (scopeSpaceCounter % 2 == 0)
    {
        return formalarg;
    }

    return functionlocal;
}

/* Get current Scope Space offset */
unsigned curr_scope_offset(void)
{
    switch (curr_scope_space())
    {
		case programvar		: 	return programVarOffset;
		case functionlocal	:	return functionLocalOffset;
		case formalarg		:	return formalArgOffset;
		default:
			assert(0);
    }
}


/* Inc current Scope Space offset */
void inccurr_scope_offset(void)
{
    switch (curr_scope_space())
    {
    	case programvar		: ++programVarOffset; break;
    	case functionlocal	: ++functionLocalOffset; break;
    	case formalarg		: ++formalArgOffset; break;
    	default:
    	    assert(0);
    }
}

void resetfunctionlocaloffset(void)
{functionLocalOffset = 0 ;}

void resetformalargoffset(void)
{formalArgOffset 	= 0 ;}

/* Restore the Scope offset to a previous state*/
void restore_curr_scope_offset(unsigned int n)
{
    switch (curr_scope_space())
	{
		case programvar		: programVarOffset = n; break;
		case functionlocal	: functionLocalOffset = n; break;
		case formalarg		: formalArgOffset = n; break;
		default:
			assert(0);
    }
}


void enterscopespace(void) { ++scopeSpaceCounter; }
void exitscopespace(void) {assert(scopeSpaceCounter > 1); --scopeSpaceCounter;}

