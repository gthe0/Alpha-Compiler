/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* alpha_token.h                                            */
/* 														    */
/*  A simple interface for storing information about        */
/*  various tokens/leximes                                  */
/*----------------------------------------------------------*/

#ifndef __ALPHA_TOKEN__
#define __ALPHA_TOKEN__

typedef struct alpha_token_t *AlphaToken_T;

/* 
* Return a new empty AlphaToken_T
*/
AlphaToken_T AlphaToken_new(void);

/*
* It frees the binded memory of the AlphaToken .
* It is a checked runtime error for `alphaToken` to be NULL
*/
AlphaToken_T AlphaToken_free(AlphaToken_T AlphaToken);

/*
* Inserts information in the token
*
*/
int AlphaToken_insert(AlphaToken_T AlphaToken, unsigned int numline,
                        unsigned int numToken, char* content,
                        char *type);

#endif /*alpha token ADT*/