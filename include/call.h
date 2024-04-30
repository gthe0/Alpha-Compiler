/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* call.h			            							*/
/* 														    */
/* Type definitions and ADT for the call struct				*/
/* used to store call information							*/
/*----------------------------------------------------------*/

#ifndef __CALL_H__
#define __CALL_H__

#include <expr.h>

/* typedefs of the call struct */
typedef struct call call, *call_T;

/* The call struct */
struct call
{
	expr *elist;
	unsigned char method;
	char *name;
};


/**
* @brief Creates a new, filled expression.
* @param elist The parameters of the call.
* @param method A flag signaling whether it's a method call or a normal call.
* @param name The name of the call i.e. the name of the function called. 
*
* @return The call created
*/
call_T new_call(expr* elist, 
				unsigned char method,
				char* name);

#endif /*call ADT*/