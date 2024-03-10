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
enum LOG_TYPE {
	ERROR,
	WARNING,
	USAGE,
	NOTE
};

typedef enum LOG_TYPE LOG_TYPE;

/**
* @brief A variadic function that logs the errors in stderr with colors
*
* @param error The Log type of the message (WARNING, ERROR, NOTE, USAGE)
* @param msg The message to print
*/
void LOG_ERROR(LOG_TYPE error,const char* msg, ...);

#endif /*LOG interface*/