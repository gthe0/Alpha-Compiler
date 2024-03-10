/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* log.c		                                            */
/* 														    */
/* Implementation of log.h interface						*/
/*----------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>

#include <log.h>

/*
* A function to print the errors in stderr
*/
void LOG_ERROR(LOG_TYPE error_t ,const char* msg,...)
{
	/* If we are on linux machines, use ansi escape sequences */
	#if ( __linux__)
		char* warning 	= "\e[1;35mWARNING: \e[0m"; /*Warning == MAG*/
		char* error		= "\e[1;31mERROR: \e[0m";	/*Error == RED*/
		char* usage		= "\e[1;33mUsage: \e[0m";	/*Usage == YEL*/
		char* note		= "\e[1;34mNote: \e[0m";	/*Note == BLU*/
	#else
		char* warning	= "WARNING: ";
		char* usage		= "Usage: ";
		char* error		= "ERROR: ";
		char* note		= "Note: ";
	#endif
	
	fflush(stdout);

	switch (error_t) 
	{
		case WARNING: fprintf(stderr,"%s",warning); break;
		case ERROR: fprintf(stderr,"%s",error); break;
		case USAGE: fprintf(stderr,"%s",usage); break;
		case NOTE: fprintf(stderr,"%s",note); break;
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