/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* log.c		                                            */
/* 														    */
/* Implementation of log.h interface						*/
/*----------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <log.h>

/*
* A function to print the errors in stderr
*/
void LOG_ERROR(OWNER owner,LOG_TYPE log_t ,const char* msg,...)
{
	/* If we are on linux machines, use ansi escape sequences */
	#if ( __linux__)
		char* MAG 		= "\e[1;35m"; 	/*Warning == MAG*/
		char* RED		= "\e[1;31m";	/*Error == RED*/
		char* YEL		= "\e[1;33m";	/*Usage == YEL*/
		char* CYAN		= "\e[1;36m";	/*Note == CYN*/
		char* reset		= "\e[0m";		/*RESET*/
	#else
		char* MAG 		= ""
		char* RED		= ""
		char* YEL		= ""
		char* CYAN		= ""
		char* reset		= ""
	#endif
	
	fflush(stdout);

	char o_name[OWNER_SIZE] = "";

	switch (owner)
	{
		case LEXER: strcpy(o_name,"LEXER"); break;
		case PARSER: strcpy(o_name,"PARSER"); break;
		default:
	}

	switch (log_t) 
	{
		case WARNING: fprintf(stderr,"%s[%s] > WARNING: %s",MAG,o_name,reset); break;
		case ERROR: fprintf(stderr,"%s[%s] > ERROR: %s",RED,o_name,reset); break;
		case USAGE: fprintf(stderr,"%s[%s] > Usage: %s",YEL,o_name,reset); break;
		case NOTE: fprintf(stderr,"%s[%s] > note: %s",CYAN,o_name,reset); break;
		default:
	}

	/* Initialize arguments */
	va_list args;
	va_start(args, msg);

	vfprintf(stderr,msg,args);

	/* Free them */
	va_end(args);

	return ;
}