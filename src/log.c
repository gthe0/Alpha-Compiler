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
void LOG_ERROR(ERROR_TYPE error_t ,const char* msg,...)
{
	/* If we are on linux machines, use ansi escape sequences */
	#if ( __linux__)
		char* warning = "\e[1;35mWARNING: \e[0m";
		char* error  = "\e[1;31mERROR: \e[0m";
	#else
		char* warning = "WARNING: ";
		char* error	  = "ERROR: ";
	#endif
	
	fflush(stdout);

	if(error_t == WARNING)
		fprintf(stderr,"%s",warning);
	else if(error_t == ERROR)
		fprintf(stderr,"%s",error);

	/* Initialize arguments */
	va_list args;
	va_start(args, msg);

	vfprintf(stderr,msg,args);

	/* Free them */
	va_end(args);

	return ;
}