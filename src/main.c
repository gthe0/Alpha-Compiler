/*----------------------------------------------------------*/
/* Authors: csd4881, csd4988, csd5038						*/
/* 														    */
/* main.c			            							*/
/* 														    */
/* Here lies the main function which starts the program		*/
/*----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <expr.h>
#include <stmt.h>
#include <call.h>
#include <quad.h>
#include <indexed_pair.h>
#include <symTableEntry.h>
#include <forpref.h>
#include <parser.h>

#include <log.h>
#include <tables.h>
#include <IntegerStack.h>

extern FILE* yyin;
extern int yydebug;
extern int ERROR_COMP;

/* Used to initialize and free the stacks */
void parser_stacks_init(void);
void parser_stacks_free(void);

/* main */
int main(int argc,char** argv)
{
	int print_sym = 0;
    FILE* ost = stdout;
 
 	/* If you want debugging make it 1 */
	yydebug = 0;

	/* If there are too few or too many arguments then terminate the program */
    if(argc == 1 || argc > 3)
    {
        LOG_ERROR(PARSER, USAGE,"%s [-s| output the symbol talbe] <INPUT_FILE>\n", argv[0]);
        return EXIT_FAILURE;
    }

	/* If there are is only one argument and it is -s,
	then throw error and terminate the program*/
	if(argc == 2 && strcmp("-s",argv[1]) == 0)
	{
        LOG_ERROR(PARSER, USAGE,"%s [-s| output the symbol talbe] <INPUT_FILE>\n", argv[0]);
        LOG_ERROR(PARSER, NOTE,"<INPUT_FILE> wasn't provided!\n");
        return EXIT_FAILURE;
	}

	/* If last argument/input file is -s terminate the program */
	if(strcmp("-s",argv[argc - 1]) == 0)
	{
        LOG_ERROR(PARSER, USAGE,"%s [-s| output the symbol talbe] <INPUT_FILE>\n", argv[0]);
        return EXIT_FAILURE;
	}

    /* Open the input file. It is always the last argument */
    if(!(yyin = fopen(argv[argc - 1],"r")))
    {
        LOG_ERROR(PARSER, ERROR, "Cannot read Input file %s\n", argv[argc - 1]);
        return EXIT_FAILURE;
    }
    
	/* If -s arg exists, make print_sym == 1 */
	print_sym = strcmp("-s",argv[1]) == 0 ? 1 : 0 ;
	
	/* If the flag was passed, open a .txt file stream*/
	if(print_sym)
	{
		if(!(ost = fopen("sym_table.txt","w")))
		{
			LOG_ERROR(PARSER, ERROR, "Cannot open Output file sym_table.txt\n");
			ost = stdout;
		}
	}

    /* Initializes tables and stack */
	parser_stacks_init();

    expand();

	/* Emit a blank instruction for padding */
    emit(blank_i,NULL , NULL, NULL, 0, 0);
    
	Tables_init();
    
	/* Call the Parser */
    yyparse();

	if(print_sym == 1)
		Tables_print(ost,0);
    
    if(ERROR_COMP == 0)
        write_quads();

    /* Close streams and clean up */
    Tables_free();
	parser_stacks_free();

    fclose(ost);
    fclose(yyin);
    
    return 0;
}