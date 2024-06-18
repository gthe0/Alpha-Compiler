/* 
*  Authors: csd4881  csd4988  csd5038
*			
* 	utils.h
*
*  A simple library containing utility functions
*/

#ifndef UTILS_H
#define UTILS_H

/**
* @brief This function turns doubles into strings, removing trailing zeroes 
* 
* @param num The double to be turned to string 
* 
* @return The generated string 
*/
char* double_to_string(double num);


/**
* @brief adds ""
* 
* @param str The string to which we will add the quotes
* 
* @return "str"
*/
char* add_quotes(char* str);

#endif /* Utility libs */