/*-------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881               */
/* 														 */
/* RuleGen.c                                           	 */
/* 														 */
/* A simple program that generates a serires of rules 	 */
/* for the lexical analyzer generator FLEX				 */
/*-------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>

#define SZ 0x30

/* 
* A simple function which takes a string 
* with two or less worlds and divides it
* into sepparate substrings/tokens
*/
int tokenize(char* buff,char* type)
{
	while(*buff != '\0' && !isspace(*buff))
	{
		buff++;
	}

	*buff++ = '\0';

	while(*buff != '\0' && isspace(*buff))
	{
		buff++;
	}

	if(*buff == '\0')
		return 1;
	
	while(*buff != '\0' && !isspace(*buff))
		*type++ = *buff++;

	*type = '\0';
	return 0;
}

/*	
* The main function reads the input from STDIN 
* and then generates the FLEX rules
*
* The input must be in either form:
*		- <TOKEN1> OR <TOKEN1> <TOKEN2>
*/
int main(void)
{
	char buffer[SZ],type[SZ];

	while(fgets(buffer,SZ,stdin))
	{	
		if(tokenize(buffer,type))
			printf("{%s}\t\t\t{return (%s);}\n",buffer,buffer);
		else
			printf("\"%s\"\t\t\t{return (%s);}\n",buffer,type);
	}

	return 0;
}
