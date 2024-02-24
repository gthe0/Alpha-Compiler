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

typedef struct alpha_token_t alpha_token_t, *AlphaToken_T;

/** 
* @brief Return a new empty AlphaToken_T
*/
AlphaToken_T AlphaToken_new(void);

/**
* @brief It frees the binded memory of the AlphaToken .
*
* @param AlphaToken --Head of the struct that will be freed.
*/
void AlphaToken_free(AlphaToken_T AlphaToken);

/**
* @brief Inserts information in the token.
* 
* @param AlphaToken -- Head of the struct that will be used for insertion
* @param numline    -- Number of the line that the token was parsed
* @param content    -- Content of the Token
* @param macro_def  -- Macro definition of the token
* @param type       -- Type of the content (e.g. ID, INT, FLOAT etc)
* @param type       -- Category of the content (e.g. Keyword, Punct etc)
*/
int AlphaToken_insert(AlphaToken_T AlphaToken, unsigned int numline, char* content, char * macro_def, char *type, char* category);

/**
* @brief It prints all the alpha_token information stored in the struct.
*
* @param AlphaToken --Head of the struct that will be printed.
*/
void AlphaToken_print_all(AlphaToken_T AlphaToken);

#endif /*alpha token ADT*/