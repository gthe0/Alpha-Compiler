/*----------------------------------------------------------*/
/* Author: George Theodorakis, AM: csd4881                  */
/* 														    */
/* alpha_token.h                                            */
/* 														    */
/*  An implementation of aplha_token ADT                    */
/*  using single linked list                                */
/*----------------------------------------------------------*/

#include <alpha_token.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * The type of the alpha_token_t
 */
struct alpha_token_t 
{
  unsigned int      numline;
  unsigned int      numToken;
  char              *content;
  char              *type;
  char              *category;

  AlphaToken_T      alpha_yylex; /* This is the next node of the list */
};


/*
 * Return a new empty AlphaToken_T
 */
AlphaToken_T AlphaToken_new(void)
{
    AlphaToken_T new = (AlphaToken_T)malloc(sizeof(alpha_token_t));

    new->numline     = 0;
    new->numToken    = 0;
    new->content     = NULL;
    new->type        = NULL; 
    new->category    = NULL; 
    new->alpha_yylex = NULL;
}

/*
 * It frees the binded memory of the AlphaToken.
 * If the AlphaToken is NULL then it will do nothing.
 */
void AlphaToken_free(AlphaToken_T AlphaToken)
{
    AlphaToken_T temp = NULL;

    if(AlphaToken == NULL)
        return ; 

    while(AlphaToken)
    {
        temp = AlphaToken;
        AlphaToken = AlphaToken -> alpha_yylex;
        free(temp->content);
        free(temp->type);
        free(temp);  
    }

    return ;
}

/*
* This function Inserts a new node at the end of the token list
*
* It returns:   0(EXIT_SUCCESS) if the insertion is successful
*               1(EXIT_FAILURE) if content or type are NULL and it fails             
*
*/
int AlphaToken_insert(AlphaToken_T AlphaToken, unsigned int numline,
                                char* content, char *type,
                                char* category)
{
    AlphaToken_T new    = NULL;

    if (content == NULL || type == NULL 
    || category == NULL || AlphaToken == NULL)
        return EXIT_FAILURE;
    
    new = AlphaToken_new(); 

    new -> content  = strdup(content);
    new -> type     = strdup(type);
    new -> category = strdup(category);
    new -> numline  = numline;
    new->alpha_yylex = NULL;

    if(AlphaToken->content == NULL)
    {
        *AlphaToken = *new;
        return EXIT_SUCCESS;
    } 

    while (AlphaToken->alpha_yylex);

    AlphaToken->alpha_yylex = new;    
    new->numToken    = AlphaToken->numToken + 1;
    
    return EXIT_SUCCESS;
}


/*
* It prints the contents of the AlphaToken list.
*/
void AlphaToken_print_all(AlphaToken_T AlphaToken)
{
    while (AlphaToken)
    {
        printf("Line : %d,\t#%d\tToken: %s\tType: %s\tCategory: %s\n",\
        AlphaToken->numline, AlphaToken->numToken, AlphaToken->content,
        AlphaToken->type, AlphaToken->category);

        AlphaToken=AlphaToken->alpha_yylex;
    }
    
    return ;
}