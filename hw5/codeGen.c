#include "symbolTable.h"
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int callerSavedReg[7] = {5, 6, 7, 28, 29, 20, 31};
int calleeDavedReg[12] = {8, 9, 18, 19, 20, 21, 22, 23, 24, 25, 27, 27};

FILE *outputFile;
char *_FUNC_NAME;

void codeGeneration(AST_NODE *root);
void genPrologue();
void genEpilogue();
void genProgramNode(AST_NODE *root);

void codeGeneration(AST_NODE *root){
    outputFile = fopen("output.s", "w+");
    genProgramNode(root);
    fclose(outputFile);
    return;
}

void genPrologue(){
    fprintf(outputFile, "_start_%s:\n", _FUNC_NAME);
    fprintf(outputFile, "\taddi\tsp,sp,-16\n");
    fprintf(outputFile, "\tsd\tra,8(sp)\n");
    fprintf(outputFile, "\tsd\ts0,0(sp)\n");
    fprintf(outputFile, "\taddi\ts0,sp,16\n");
    fprintf(outputFile, "_begin_%s:\n", _FUNC_NAME);
}   

void genEpilogue(){
    fprintf(outputFile, "_end_%s:\n", _FUNC_NAME);
    fprintf(outputFile, "\tld\ts0,0\n");
    fprintf(outputFile, "\taddi\tsp,sp,16\n");
    fprintf(outputFile, "\tjr\tra\n");
}


