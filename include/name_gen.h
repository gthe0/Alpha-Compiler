/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* name_gen.h		            							*/
/* 														    */
/* A library with utility functions for name creation		*/
/*----------------------------------------------------------*/

#ifndef __NAME_GEN_H__
#define	__NAME_GEN_H__

/**
* @brief This Functions generates a name for Functions with no name defined
* 
* @return a name with illegal characters so that it will be unique
*/
char* func_name_generator(void);

/**
* @brief Generaetes a temporary symbol name
*
* @return The temp name 
*/
char* new_temp_name(void);

/**
* @brief Resets temp_counter
*/
void reset_temp(void);

#endif /* Name Generation functions */