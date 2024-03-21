/*
	Authors: csd5038, csd4881, csd4988

*/
%{
    #include <stdio.h>
    #define YY_DECL int alpha_yylex(void * ylval)	
    int yyerror (char * yaccProvidedMessage);
    int yylex (void);

  //  void print_token_value (FILE *, int, YYSTYPE);
    
    extern int yylineno;
    extern char *yytext;
    extern FILE* yyin;
%}


%output "parser.c" 

//Declarations 
%start program

%union {
    int intVal;
    float floatVal;
    char * stringVal;
}

%{
    void print_token_value (FILE *, int, YYSTYPE);
%}

%token <stringVal> ID 
%token <intVal> INT 
%token <floatVal> FLOAT 
%token <stringVal> STRING 

%token IF ELSE WHILE FOR FUNC RETURN BREAK CONTINUE
%token AND OR NOT LOCAL TRUE FALSE NIL
%token BRACKETS_O BRACKETS_C BRACES_O BRACES_C PAREN_O PAREN_C CURLY_O CURLY_C COMMA SEMI_COLON COLON DOUBLE_COL DOT DOUBLE_DOT
%token ASSIGN PLUS MINUS MUL DIV MOD GREATER LESS
%token EQ_OP NE_OP INC_OP DEC_OP LE_OP GE_OP

%right ASSIGN
%left AND
%left OR
%left PLUS MINUS
%left MUL DIV MOD
%right NOT INC_OP DEC_OP 
%left DOT DOUBLE_DOT
%left BRACKETS_O BRACKETS_C
%left PAREN_O PAREN_C
%left UN_MINUS

%nonassoc EQ_OP NE_OP
%nonassoc LE_OP GE_OP GREATER LESS

//EXPLAIN THIS ONE!!!!!!! it reduces the number of shift/reduce conflicts by one
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
//Grammar
program: stmt_list /* stmt* means zero or more stmt */
       ;

stmt_list: /* empty production, making stmt_list optional */
         | stmt_list stmt 
         ;
    
stmt: expr SEMI_COLON
    | ifstmt
    | whilestmt
    | forstmt
    | returnstmt
    | BREAK SEMI_COLON
    | CONTINUE SEMI_COLON
    | block
    | funcdef
    | SEMI_COLON
    ;

expr: assginexpr
    | expr PLUS expr
    | expr MINUS expr
    | expr MUL expr
    | expr DIV expr
    | expr MOD expr
    | expr GREATER expr
    | expr GE_OP expr
    | expr LESS expr
    | expr LE_OP expr
    | expr EQ_OP expr
    | expr NE_OP expr
    | expr AND expr
    | expr OR expr
    | term
    ;

//op: PLUS | MINUS | MUL | DIV | MOD | GREATER | GE_OP | LESS | LE_OP  | EQ_OP | NE_OP | AND | OR ;

term: PAREN_O expr PAREN_C
    | MINUS expr
    | NOT expr
    | INC_OP lvalue
    | lvalue INC_OP
    | DEC_OP lvalue
    | lvalue DEC_OP
    | primary
    ;

assginexpr: lvalue ASSIGN expr 
    ;

primary: lvalue
    | call
    | objectdef
    | PAREN_O funcdef PAREN_C
    | const
    ;

lvalue: ID
    | LOCAL ID
    | DOUBLE_COL ID
    | member
    ;

member: lvalue DOT ID
    | lvalue BRACKETS_O expr BRACKETS_C
    | call DOT ID
    | call BRACKETS_O expr BRACKETS_C
    ;

call: call PAREN_O elist PAREN_C
    | lvalue callsuffix
    | PAREN_O funcdef PAREN_C PAREN_O elist PAREN_C
    ;

callsuffix: normcall
    | methodcall 
    ;

normcall: PAREN_O elist PAREN_C 
    ;

methodcall: DOUBLE_DOT ID PAREN_O elist PAREN_C 
    ; // equivalent to lvalue.id(lvalue, elist)

elist:  
    expr | elist COMMA expr
    ;

objectdef:  BRACKETS_O object_list BRACKETS_C ;

object_list:    
    elist | indexed
    ;

indexed:   
    | indexed COMMA indexedelem
    | indexedelem
    ;

indexedelem: BRACES_O expr COLON expr BRACES_C
    ;

block:  BRACES_O stmt_list BRACES_C 
    ;

funcdef: FUNC id_option PAREN_O idlist PAREN_C block
    ;

id_option: /* empty production, making id_option optional */
    | ID
    ;

const: INT | FLOAT | STRING | NIL | TRUE | FALSE 
    ;

idlist: 
    | ID
    | idlist COMMA ID
    ;

ifstmt: IF PAREN_O expr PAREN_C stmt
    | IF PAREN_O expr PAREN_C stmt ELSE stmt
    ;

whilestmt: WHILE PAREN_O expr PAREN_C stmt 
    ;

forstmt: FOR PAREN_O elist SEMI_COLON expr SEMI_COLON elist PAREN_C stmt ;

returnstmt: RETURN expr SEMI_COLON
    | RETURN SEMI_COLON
    ;
%%


int yyerror (char * yaccProvidedMessage) {
    fprintf (stderr, "%s: at line %d, before token %s\n", yaccProvidedMessage, yylineno, yytext);    fprintf (stderr,"INPUT NOT VALID\n");
}

int main (int argc, char **argv) {
    if (argc > 1) {
        if (!(yyin = fopen (argv[1], "r"))) {
            fprintf (stderr, "Cannot open file %s\n", argv[1]);
            return 1;
        }
    }
    yyparse();
    return 0;
}


