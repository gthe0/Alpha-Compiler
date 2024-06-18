/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* call.c			            							*/
/* 														    */
/* Implementation of the call_T ADT and its functions		*/
/*----------------------------------------------------------*/


#include <call.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/* Creates a new call_T object */
call_T new_call(expr* elist, 
				unsigned char method,
				char* name)
{
	call_T oCall = malloc(sizeof(call));

	assert(oCall);

	oCall->elist = elist;
	oCall->method = method;
	/* If it is NULL, do not call strdup, Prevents seg fault */
	oCall->name = name ? strdup(name) : NULL ;
	return oCall;
}