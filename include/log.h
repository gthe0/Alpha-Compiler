/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* log.h		                                            */
/* 														    */
/* An interface of a function to log err types				*/
/*----------------------------------------------------------*/

#ifndef __LOG__
#define __LOG__

/* Enums for error levels*/
enum ERROR_TYPE {
	ERROR,
	WARNING,
};

typedef enum ERROR_TYPE ERROR_TYPE;

/**
* @brief A variadic function that logs the errors in stderr with colors
*
* @param error The Error type
* @param msg The message to print
*/
void LOG_ERROR(ERROR_TYPE error,const char* msg, ...);

#endif /*LOG interface*/