/* 
*  Authors: csd4881  csd4988  csd5038
*			
* 	utils.c
*
*  An Implementation of the utility functions 
*/

#include <utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* The maximum length of a double string */
#define MAX_DOUBLE_LENGTH 25

/* Used to generate a string out of double nums */
char* double_to_string(double num)
{
	char string_num[MAX_DOUBLE_LENGTH];
	sprintf(string_num, "%lf", num);

	int is_decimal = 0;
	char* head = string_num;
	
	/* If it is a float remove trailing decimals */
	while(*head != '\0')
		if(*head++ == '.')
			is_decimal = 1;
	
	/* Go inside the string */
	head-- ;

	/* Do not overextend */
	while (is_decimal && *head == '0' && head != string_num)
		head-- ;

	if(is_decimal)
		if(*head == '.')
			*head = '\0';
		else if(*head != '0' && head != string_num)
			*++head = '\0';

	/* Allocate the memory needed and return the string */	
	char *generated_string = malloc((strlen(string_num)) * sizeof(char) + 1);
	assert(generated_string);

	strcpy(generated_string, string_num);

	return generated_string;
}

/* Adds quotes to a string and returns it */
char* add_quotes(char* str)
{
	char* strConst = malloc(strlen(str)*sizeof(char)+3);
	sprintf(strConst,"\"%s\"",str);
	return strConst;
}


