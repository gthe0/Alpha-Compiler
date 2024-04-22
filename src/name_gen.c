#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static unsigned unnamed_func_counter = 0;
static unsigned temp_counter = 0;

/* This Functions generates a name for Functions with no name defined */
char *func_name_generator(void)
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

/* This Functions generates a name for hidden/temp variables */
char *new_temp_name(void)
{
	char *name = "_t";
	char var_number[20];

	sprintf(var_number, "%d", temp_counter++);

	char *generated_name = malloc((strlen(name) + strlen(var_number)) * sizeof(char) + 1);
	assert(generated_name);

	strcpy(generated_name, name);
	strcat(generated_name, var_number);

	return generated_name;
}

/* Resets temp counter */
void reset_temp(void)
{
	temp_counter = 0;
}
