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
#include <getopt.h>
#include <ctype.h>

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
#include <optimizer.h>
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
    
	int sflag = 0;
    int Ovalue = 0; /* To store the value after -O flag */
    int c;

    char *inputFile = NULL;
    
	opterr = 0; /* Disable getopt's error messages */


	/* If the final arg is not the input file then terminate program */
	if(*argv[argc-1] != '-' && !(yyin = fopen(argv[argc-1],"r")))
	{

		LOG_ERROR(PARSER, ERROR, "Could not open file stream %s\n",argv[argc-1]);
		LOG_ERROR(PARSER, USAGE,	"%s [-s| Print symbol table entries]"\
									" [-Ox| Optimize IR, replace x with 1,2 or 3]"\
									" <INPUT FILE>\n",argv[0]);				

		return 1;
	}


    while ((c = getopt(argc, argv, "sO:")) != -1) {
        
		switch (c) {

			case 's':
				
				/* Do not reopen stream if it was already opened */
				if(!sflag)
					ost = fopen("sym_table.txt","w");

                sflag = 1;

                break;
            
			case 'O':
				
                Ovalue = atoi(optarg); /* Convert the argument to an integer */

				/* Check if the Ovalue that was passed is correct, if not terminate */
                if (Ovalue < _USELESS_ASSIGN  || Ovalue > _PEEP_HOLE_OPT )
				{
                    LOG_ERROR(PARSER, ERROR, "Invalid value after -O flag. Please use 1, 2, or 3.\n");
					LOG_ERROR(PARSER, NOTE, "If the value is 1, do dead code elimination\n"
											"If the value is 2, do Constant Propagation\n"
											"If the value is 3, do both\n");
					fclose(ost);
                    return 1;
                }

                break;

            case '?':	/* In case of an unknown option, getopt passed '?' */

				LOG_ERROR(PARSER, USAGE,	"%s [-s| Print symbol table entries]"\
											" [-Ox| Optimize IR, replace x with 1,2 or 3]"\
											" <INPUT FILE>\n",argv[0]);				

                if (optopt == 'O') 
				{
                    LOG_ERROR(PARSER,NOTE, "Option -%c requires an argument.\n", optopt);
                } 
				else if (isprint(optopt))
				{
                    LOG_ERROR(PARSER,NOTE, "Unknown option '-%c'.\n", optopt);
                }
				else
				{
                    LOG_ERROR(PARSER,NOTE,"Unknown option character '\\x%x'.\n", optopt);
                }

				fclose(ost);
                return 1;

			default:
                abort();
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

	if(sflag)
		Tables_print(ost,0);
    
    if(ERROR_COMP == 0)
	{
		/*Optimize Quads*/
		optimization_level(Ovalue);

        write_quads();
	}


    /* Close streams and clean up */
    Tables_free();
	parser_stacks_free();

    fclose(ost);
    fclose(yyin);
    
    return 0;
}