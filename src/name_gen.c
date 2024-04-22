#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static unsigned unnamed_func_counter = 0;

/* This Functions generates a name for Functions with no name defined */
char *func_name_generator()
{
	char *name = "$function_";
	char func_number[20];

	sprintf(func_number, "%d", unnamed_func_counter++);

	char *generated_name = malloc((strlen(name) + strlen(func_number)) * sizeof(char) + 1);
	assert(generated_name);

	strcpy(generated_name, name);
	strcat(generated_name, func_number);

	return generated_name;
}