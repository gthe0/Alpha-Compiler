/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* al.h			                                            */
/* 														    */
/* A function that generates the wanted output				*/
/*----------------------------------------------------------*/

#ifndef AL
#define AL

#include <alpha_token.h>

/**
* @brief It prints all the alpha_token information stored in the struct
*		 in the wanted fromat
*
* @param AlphaToken Head of the struct.
*/
void al(AlphaToken_T AlphaToken,FILE* ost);

#endif /*AL interface*/