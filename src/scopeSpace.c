/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* scopeSpace.c		            							*/
/* 														    */
/* An implementation scopeSpace.h module					*/
/*----------------------------------------------------------*/

#include <scopeSpace.h>

static unsigned programVarOffset	= 0 ;
static unsigned functionLocalOffset = 0 ;
static unsigned formalArgOffset 	= 0 ;
static unsigned scopeSpaceCounter 	= 1 ;

/* Get current Scope Space */
ScopeSpace currscopespace(void)
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
unsigned currscopeoffset(void)
{
    switch (currscopespace())
    {
		case programvar		: 	return programVarOffset;
		case functionlocal	:	return functionLocalOffset;
		case formalarg		:	return formalArgOffset;
		default:
			assert(0);
    }
}


/* Inc current Scope Space offset */
void incurrscopeoffset(void)
{
    switch (currscopespace())
    {
    case programvar		: ++programVarOffset; break;
    case functionlocal	: ++functionLocalOffset; break;
    case formalarg		: ++formalArgOffset; break;
    default:
        assert(0);
    }
}

void enterscopespace(void) { ++scopeSpaceCounter; }
void exitscopespace(void) {assert(scopeSpaceCounter > 1); --scopeSpaceCounter;}

