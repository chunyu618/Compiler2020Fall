%option noyywrap 
%{
#include <stdio.h>
#include "header.h" 
int linenumber;
symtab * lookup();
symtab * ptr;
void     insertID();
void 	 printSymTab();
/* You need to define for all tokens in C--, here are some examples */
#define LETTER      11
#define DIGIT       12
#define WHITE_SPACE 13
#define INT_CONST   14
#define FLOAT_CONST	15 
#define STR_CONST   16
#define IDENTIFIER  17
#define COMMENT     18
#define OP_ASSIGN   19
#define OP_ADD      20
#define OP_MINUS    21
#define OP_MUL      22
#define OP_DIV      23
#define OP_BIG_EQL  24
#define OP_BIG_THAN 25
#define OP_SML_EQL  26
#define OP_SML_THAN 27
#define OP_EQL      28
#define OP_NOT_EQL  29
#define OP_AND      30
#define OP_OR       31
#define OP_NOT      32
#define NEWLINE     33
#define DL_LPAREN   34
#define DL_RPAREN   35
#define DL_LBRACKET 36
#define DL_RBRACKET 37
#define DL_LBRACE   38
#define DL_RBRACE   39
#define DL_COMMA    40
#define DL_SEMICOL  41 
#define DL_DOT      42
#define ERROR 		100 

%}

letter [A-Za-z]
digit [0-9]
WS [ \t]+

/* Define constants */
Int_constant {digit}+
Float_constant {digit}\.{digit}
String_constant \"([^\n"\\]|\\.)*\"

/* Define identifiers */
ID {letter}({letter}|{digit}|"_")*

/* Define comments */
comment \/\*(\*+[^*/]|[^*])*\*+\/

/* Define operators and symbols */ 
OP_assign "="
OP_add "+"
OP_minus "-"
OP_mul "*"
OP_div "/"
OP_big_or_eql ">="
OP_big_than ">"
OP_small_or_eql "<="
OP_small_than "<"
OP_eql "=="
OP_not_eql "!="
OP_and "&&"
OP_or "||"
OP_not "!"

/* Define newline */
newline "\n"

/* Define separators */
DL_lparen "("
DL_rparen ")"
DL_lbracket "["
DL_rbracket "]"
DL_lbrace "{"
DL_rbrace "}"
DL_comma ","
DL_semicol ";"
DL_dot "."

error .

%%
{letter} return LETTER;
{digit} return DIGIT;
{WS} return WHITE_SPACE;
{Int_constant} return INT_CONST;
{Float_constant} return FLOAT_CONST;
{String_constant} {printf("Get string const %s\n", yytext); return STR_CONST;}
{ID} { 
    ptr = lookup(yytext);
    if (ptr == NULL)
        insertID(yytext);	
    else 
        ptr->counter++;
}
{comment} {
    printf("%s\n", yytext);
    return COMMENT;
}
{OP_assign} return OP_ASSIGN; 
{OP_add} return OP_ADD;
{OP_minus} return OP_MINUS;
{OP_mul} return OP_MUL;
{OP_div} return OP_DIV;
{OP_big_or_eql} return OP_BIG_EQL;
{OP_big_than} return OP_BIG_THAN;
{OP_small_or_eql} return OP_SML_EQL;
{OP_small_than} return OP_SML_THAN;
{OP_eql} return OP_EQL;
{OP_not_eql} return OP_NOT_EQL;
{OP_and} return OP_AND;
{OP_or} return OP_OR;
{OP_not} return OP_NOT;
{newline} {
    linenumber += 1;
    return NEWLINE;
}
{DL_lparen} return DL_LPAREN;
{DL_rparen} return DL_RPAREN; 
{DL_lbracket} return DL_LBRACKET;
{DL_rbracket} return DL_RBRACKET;
{DL_lbrace} return DL_LBRACE; 
{DL_rbrace} return DL_RBRACE; 
{DL_comma} return DL_COMMA;   
{DL_semicol} return DL_SEMICOL;
{DL_dot} return DL_DOT;
{error} printf("Read string %s with ERR \n", yytext);/* return ERROR; */

%%

int main(int argc, char **argv)
{
 argc--; ++argv;
   if (argc > 0)
      yyin = fopen(argv[0], "r");
   else
      yyin = stdin;
   while(yylex() > 0);
   printSymTab();
}

