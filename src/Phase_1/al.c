/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* al.c			                                            */
/* 														    */
/* Implementation of al.h                                   */
/*----------------------------------------------------------*/

#include <al.h>
#include <stdio.h>

void al(AlphaToken_T AlphaToken,FILE* ost)
{
    fprintf(ost,"======================== LEXICAL ANALYSIS ========================\n");
    AlphaToken_print_all(AlphaToken,ost);

    return ;
}