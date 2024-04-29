#ifndef __CALL_H__
#define __CALL_H__

#include <expr.h>

typedef struct call call, *call_T;

struct call
{
	expr *elist;
	int method;
	char *name;
};


call_T new_call(expr* elist, 
				unsigned char method,
				char* name);

#endif