/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* log.h		                                            */
/* 														    */
/* An interface of a function to log err types				*/
/*----------------------------------------------------------*/

#ifndef __LOG__
#define __LOG__

#define OWNER_SIZE 0x10

/* Enums for error levels*/
enum LOG_TYPE {
	ERROR,
	WARNING,
	USAGE,
	NOTE
};

/* Enums for Owner of the Log*/
enum OWNER{
	LEXER,
	PARSER
};

typedef enum OWNER OWNER;

typedef enum LOG_TYPE LOG_TYPE;

/**
* @brief A variadic function that logs the errors in stderr with colors
*
* @param log_t The Log type of the message (WARNING, ERROR, NOTE, USAGE)
* @param owner Where the error was encountered
* @param msg The message to print
*/
void LOG_ERROR(OWNER owner,LOG_TYPE log_t,const char* msg, ...);

#endif /*LOG interface*/