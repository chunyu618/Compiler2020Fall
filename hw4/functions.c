#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char *AST_TYPE_string[] = {"PROGRAM", "GLOBAL_DECL_LIST", "GLOBAL_DECL", "DECL_LIST", "FUNCTION_DECL", "PARAM_LIST", "PARAM", "DIM_FN", "DIMFN1", "EXPR_NULL", "BLOCK", "DECL", "TYPE_DECL", "VAR_DECL",
    "TYPE", "STRUCT_TYPE", "DEF_LIST", "DEF", "OPT_TAG", "TAG", "ID_LIST", "DIM_DECL", "CEXPR", "MCEXPR", "CFACTOR", "INIT_ID_LIST", "INIT_ID", "STMT_LIST", "STMT", "ASSIGN_EXPR_LIST",
    "NONEMPTY_ASSIGN_EXPR_LIST", "TEST", "ASSIGN_EXPR", "RELOP_EXPR", "RELOP_TERM", "RELOP_FACTOR", "REL_OP", "RELOP_EXPR_LIST", "NONEMPTY_RELOP_EXPR_LIST", "EXPR", "ADD_OP", "TERM",
    "MUL_OP", "FACTOR", "VAR_REF", "DIM", "STRUCT_TAIL", "NUL","ID_value", "CONST_value"};

char *IDENTIFIER_KIND_string[] = {"NORMAL_ID", "ARRAY_ID", "WITH_INIT_ID"};

int printGVNode(FILE *fp, AST_NODE* node, int count);

char *printLabelString(FILE *fp, AST_NODE *astNode)
{
    char *binaryOpString[] = {
        "+",
        "-",
        "*",
        "/",
        "==",
        ">=",
        "<=",
        "!=",
        ">",
        "<",
        "&&",
        "||"
    };
    char *unaryOpString[] = {
        "+",
        "-",
        "!"
    };
//    fprintf(fp, "%d ", astNode->linenumber);
    switch (astNode->nodeType) {
        case PROGRAM_NODE:
            fprintf(fp, "PROGRAM_NODE");
            break;
        case DECLARATION_NODE:
            fprintf(fp, "DECLARATION_NODE ");
            switch (astNode->semantic_value.declSemanticValue.kind) {
                case VARIABLE_DECL:
                    fprintf(fp, "VARIABLE_DECL");
                    break;
                case TYPE_DECL:
                    fprintf(fp, "TYPE_DECL");
                    break;
                case FUNCTION_DECL:
                    fprintf(fp, "FUNCTION_DECL");
                    break;
                case FUNCTION_PARAMETER_DECL:
                    fprintf(fp, "FUNCTION_PARAMETER_DECL");
                    break;
            }
            break;
        case IDENTIFIER_NODE:
            fprintf(fp, "IDENTIFIER_NODE ");
            fprintf(fp, "%s ", astNode->semantic_value.identifierSemanticValue.identifierName);
            switch (astNode->semantic_value.identifierSemanticValue.kind) {
                case NORMAL_ID:
                    fprintf(fp, "NORMAL_ID");
                    break;
                case ARRAY_ID:
                    fprintf(fp, "ARRAY_ID");
                    break;
                case WITH_INIT_ID:
                    fprintf(fp, "WITH_INIT_ID");
                    break;
            }
            break;
        case PARAM_LIST_NODE:
            fprintf(fp, "PARAM_LIST_NODE");
            break;
        case NUL_NODE:
            fprintf(fp, "NUL_NODE");
            break;
        case BLOCK_NODE:
            fprintf(fp, "BLOCK_NODE");
            break;
        case VARIABLE_DECL_LIST_NODE:
            fprintf(fp, "VARIABLE_DECL_LIST_NODE");
            break;
        case STMT_LIST_NODE:
            fprintf(fp, "STMT_LIST_NODE");
            break;
        case STMT_NODE:
            fprintf(fp, "STMT_NODE ");
            switch (astNode->semantic_value.stmtSemanticValue.kind) {
                case WHILE_STMT:
                    fprintf(fp, "WHILE_STMT");
                    break;
                case FOR_STMT:
                    fprintf(fp, "FOR_STMT");
                    break;
                case ASSIGN_STMT:
                    fprintf(fp, "ASSIGN_STMT");
                    break;
                case IF_STMT:
                    fprintf(fp, "IF_STMT");
                    break;
                case FUNCTION_CALL_STMT:
                    fprintf(fp, "FUNCTION_CALL_STMT");
                    break;
                case RETURN_STMT:
                    fprintf(fp, "RETURN_STMT");
                    break;
            }
            break;
        case EXPR_NODE:
            fprintf(fp, "EXPR_NODE ");
            switch (astNode->semantic_value.exprSemanticValue.kind) {
                case BINARY_OPERATION:
                    fprintf(fp, "%s", binaryOpString[astNode->semantic_value.exprSemanticValue.op.binaryOp]);
                    break;
                case UNARY_OPERATION:
                    fprintf(fp, "%s", unaryOpString[astNode->semantic_value.exprSemanticValue.op.unaryOp]);
                    break;
            }
            break;
        case CONST_VALUE_NODE:
            fprintf(fp, "CONST_VALUE_NODE ");
            switch (astNode->semantic_value.const1->const_type) {
                case  INTEGERC:
                    fprintf(fp, "%d", astNode->semantic_value.const1->const_u.intval);
                    break;
                case FLOATC:
                    fprintf(fp, "%f", astNode->semantic_value.const1->const_u.fval);
                    break;
                case STRINGC:
                    astNode->semantic_value.const1->const_u.sc[strlen(astNode->semantic_value.const1->const_u.sc) - 1] = 0;
                    fprintf(fp, "\\\"%s\\\"", astNode->semantic_value.const1->const_u.sc + 1);
                    astNode->semantic_value.const1->const_u.sc[strlen(astNode->semantic_value.const1->const_u.sc)] = '"';
                    break;
            }
            break;
        case NONEMPTY_ASSIGN_EXPR_LIST_NODE:
            fprintf(fp, "NONEMPTY_ASSIGN_EXPR_LIST_NODE");
            break;
        case NONEMPTY_RELOP_EXPR_LIST_NODE:
            fprintf(fp, "NONEMPTY_RELOP_EXPR_LIST_NODE");
            break;
        default:
            fprintf(fp, "default case in char *getLabelString(AST_TYPE astType)");
            break;
    }
}

void printGV(AST_NODE *root, char* fileName)
{
    if (fileName == NULL) {
        fileName = "AST_Graph.gv";
    }
    FILE *fp;
    fp = fopen(fileName, "w");
    if (!fp) {
        printf("Cannot open file \"%s\"\n", fileName);
        return;
    }
    fprintf(fp , "Digraph AST\n");
    fprintf(fp , "{\n");
    fprintf(fp , "label = \"%s\"\n", fileName);

    int nodeCount = 0;
    printGVNode(fp, root, nodeCount);

    fprintf(fp , "}\n");
    fclose(fp);
}

// count: the (unused) id number to be used
// return: then next unused id number
int printGVNode(FILE *fp, AST_NODE* node, int count)
{
    if (node == NULL) {
        return count;
    }

    int currentNodeCount = count;
    fprintf(fp, "node%d [label =\"", count);
    printLabelString(fp, node);
    fprintf(fp, "\"]\n");
    ++count;
    int countAfterCheckChildren = count;
    if (node->child) {
        countAfterCheckChildren = printGVNode(fp, node->child, count);
        fprintf(fp, "node%d -> node%d [style = bold]\n", currentNodeCount, count);
    }

    int countAfterCheckSibling = countAfterCheckChildren;
    if (node->rightSibling) {
        countAfterCheckSibling = printGVNode(fp, node->rightSibling, countAfterCheckChildren);
        fprintf(fp, "node%d -> node%d [style = dashed]\n", currentNodeCount, countAfterCheckChildren);
    }

    return countAfterCheckSibling;
}

void printNode(AST_NODE *root, int level, int levelCount[]){
    //printf("%d", level);
    AST_NODE *child;
    switch(root->nodeType){
        case PROGRAM_NODE:
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Program Node\n");
            child = root->child;
            while(child != NULL){
                printNode(child, level + 1, levelCount);
                child = child->rightSibling;
                if(child != NULL && child->rightSibling == NULL){
                    levelCount[level] = 0;
                }
            }
            break;
        case DECLARATION_NODE:
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Declaration Node\n");
            child = root->child;
            while(child != NULL){
                printNode(child, level + 1, levelCount);
                child = child->rightSibling;
                if(child != NULL && child->rightSibling == NULL){
                    levelCount[level] = 0;
                }
            }
            break;
        case IDENTIFIER_NODE:
            levelCount[level] = 1;
            printLevel(level, levelCount);
            char type[32] = {};
            char name[32] = {};
            strcpy(type, IDENTIFIER_KIND_string[root->semantic_value.identifierSemanticValue.kind]);
            strcpy(name, root->semantic_value.identifierSemanticValue.identifierName);
            printf("-- Identifier Node:\n");
            printLevel(level + 1, levelCount);
            printf("-- type %s\n", type);
            levelCount[level] = 0;
            printLevel(level + 1, levelCount);
            printf("-- name %s\n", name);
            break;
        case PARAM_LIST_NODE:
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Parameters List Node\n");
            child = root->child;
            while(child != NULL){
                printNode(child, level + 1, levelCount);
                child = child->rightSibling;
                if(child != NULL && child->rightSibling == NULL){
                    levelCount[level] = 0;
                }
            }
            break;
        case NUL_NODE:
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- NULL Node\n");
            break;
        case BLOCK_NODE:
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Block Node\n");
            child = root->child;
            while(child != NULL){
                printNode(child, level + 1, levelCount);
                child = child->rightSibling;
                if(child != NULL && child->rightSibling == NULL){
                    levelCount[level] = 0;
                }
            }
            break;
        case VARIABLE_DECL_LIST_NODE:    
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Variables Declaration List Node\n");
            child = root->child;
            while(child != NULL){
                printNode(child, level + 1, levelCount);
                child = child->rightSibling;
                if(child != NULL && child->rightSibling == NULL){
                    levelCount[level] = 0;
                }
            }
            break;
        case STMT_LIST_NODE:
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Statements List Node\n");
            break;
        case STMT_NODE:    
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Statement Node\n");
            break;
        case EXPR_NODE:
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Expression Node\n");
            break;
        case CONST_VALUE_NODE:
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Const Value Node\n");
            break;
        case NONEMPTY_ASSIGN_EXPR_LIST_NODE:
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Non-empty Assignment Expression Node\n");
            break;
        case NONEMPTY_RELOP_EXPR_LIST_NODE:    
            levelCount[level] = 1;
            printLevel(level, levelCount);
            printf("-- Non-empty Relop Expression Node\n");
            break;
        default:
            break;
    }
}

void printLevel(int level, int levelCount[]){
    //printf("|");
    for(int i = 0; i < level - 1; i++){
        (levelCount[i])? printf("    |") : printf("    ");
    }
    if(level > 0){
        (levelCount[level - 1])? printf("    |") : printf("    `");
    }
    //printf("|");
}
