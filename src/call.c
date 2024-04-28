#include <call.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

call_T new_call(expr* elist, 
				unsigned char method,
				char* name)
{
	call_T oCall = malloc(sizeof(call));

	assert(oCall);

	oCall->elist = elist;
	oCall->method = method;
	oCall->name = name ? strdup(name) : NULL ;
	return oCall;
}