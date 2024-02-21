/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* al.c			                                            */
/* 														    */
/* Implementation of al.h                                   */
/*----------------------------------------------------------*/

#include <al.h>
#include <stdio.h>

void AL_print(AlphaToken_T AlphaToken)
{
    printf("======================== LEXICAL ANALYSIS ========================\n");
    AlphaToken_print_all(AlphaToken);

    return ;
}