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

static __inline__ char *getIdByNode(AST_NODE *node){
    if(node->nodeType == IDENTIFIER_NODE){
        return node->semantic_value.identifierSemanticValue.identifierName;
    }
    return NULL;
}

void codeGeneration(AST_NODE *root);
void genPrologue();
void genEpilogue();
void genProgram(AST_NODE *root);
void genGlobalDeclarationList(AST_NODE *root);
void genGlobalDeclaration(AST_NODE *root);
void genFunctionDeclaration(AST_NODE *root);

void codeGeneration(AST_NODE *root){
    outputFile = fopen("output.s", "w+");
    genProgram(root);
    fclose(outputFile);
    return;
}

void genPrologue(){
    fprintf(outputFile, "_start_%s:\n", _FUNC_NAME); /* Generation of function start label*/
    fprintf(outputFile, "\taddi\tsp,sp,-16\n");      /* addi    sp,sp,-16 */
    fprintf(outputFile, "\tsd\tra,8(sp)\n");         /* sd      ra,8(sp)  */
    fprintf(outputFile, "\tsd\ts0,0(sp)\n");         /* sd      s0,0(sp)  */ 
    fprintf(outputFile, "\taddi\ts0,sp,16\n");       /* addi    s0,sp,16  */
    fprintf(outputFile, "_begin_%s:\n", _FUNC_NAME); /* Generation of function begin label */
}   

void genEpilogue(){
    /* Restore */
    fprintf(outputFile, "_end_%s:\n", _FUNC_NAME);   /* Generation of function end label */
    fprintf(outputFile, "\tld\ts0,sp(0)\n");         /* ld      s0,sp(0) */
    fprintf(outputFile, "\tld\tra,sp(8)\n");         /* ld      ra,sp(8) */  
    fprintf(outputFile, "\taddi\tsp,sp,16\n");       /* addi    sp,sp,16 */
    fprintf(outputFile, "\tjr\tra\n");               /* jr      ra       */
}

void genProgram(AST_NODE *root){
    // There is two types of node being children of program node
    // VARIABLE_DECL_LIST_NODE and DECLARATION_NODE
    for(AST_NODE *global_decl = root->child; global_decl != NULL; global_decl = global_decl->rightSibling){
        if(global_decl->nodeType == VARIABLE_DECL_LIST_NODE){
            //printf("[DEBUG] Node type is VARIABLE_DECL_LIST_NODE\n");
            genGlobalDeclarationList(global_decl);      
        }
        else if(global_decl->nodeType == DECLARATION_NODE){
            //printf("[DEBUG] Node type is DECLARATION_NODE\n");
        }
    } 
}

void genGlobalDeclarationList(AST_NODE *root){
    fprintf(outputFile, ".data\n");
    for(AST_NODE *decl = root->child; decl != NULL; decl = decl->rightSibling){
        genGlobalDeclaration(decl->child);  
    }
}

void genGlobalDeclaration(AST_NODE *root){
    AST_NODE *typeNode = root;
    for(AST_NODE *id = typeNode->rightSibling; id != NULL; id = id->rightSibling){
        SymbolTableEntry *entry = retrieveSymbol(getIdByNode(id));
        if(entry->attribute->attributeKind == TYPE_ATTRIBUTE){
            // TODO
            printf("[DEBUG] Typedef");
        }
        else if(entry->attribute->attributeKind == VARIABLE_ATTRIBUTE){
            //printf("[DEBUG] Type is %s ", getIdByNode(typeNode));
            TypeDescriptor* desc = entry->attribute->attr.typeDescriptor;
            if(desc->kind == SCALAR_TYPE_DESCRIPTOR){
                // Generation Identifier Label
                //printf("Scalar type %s ", id->semantic_value.identifierSemanticValue.identifierName);
                IDENTIFIER_KIND idKind = id->semantic_value.identifierSemanticValue.kind;   
                
                if(idKind == NORMAL_ID){
                    // When declared without initialized, we initialize this variable to 0.
                    //printf("Without initial value\n");
                    if(typeNode->dataType == INT_TYPE){
                        fprintf(outputFile, "_%s:  .word  0\n", getIdByNode(id));        
                    }
                    else if (typeNode->dataType == FLOAT_TYPE){
                        fprintf(outputFile, "_%s:  .word 0x00000000\n");
                    }
                }
                else if (idKind == WITH_INIT_ID){
                    // Variables may be initialized as const value or expression.
                    //printf("With initial value\n");
                    if(typeNode->dataType == INT_TYPE){
                        if(id->child->nodeType == EXPR_NODE && id->child->semantic_value.exprSemanticValue.isConstEval){
                            // TODO
                        }
                        else if(id->child->nodeType == CONST_VALUE_NODE){
                            int val = id->child->semantic_value.const1->const_u.intval;
                            fprintf(outputFile, "_%s:  .word  %d\n", getIdByNode(id), val);
                        }
                    }
                    else if(typeNode->dataType == FLOAT_TYPE){
                        if(id->child->nodeType == EXPR_NODE && id->child->semantic_value.exprSemanticValue.isConstEval){
                            // TODO
                        }
                        else if(id->child->nodeType == CONST_VALUE_NODE){
                            float val = id->child->semantic_value.const1->const_u.fval;
                            fprintf(outputFile, "_%s:  .word  %#10x\n", getIdByNode(id), *(unsigned int*)&val);
                        }
                    }

                }
            }
            else if(desc->kind == ARRAY_TYPE_DESCRIPTOR){
                // TODO
                printf("Array type %s\n", id->semantic_value.identifierSemanticValue.identifierName);
            }
        }
    }
}

void genFunctionDeclaration(AST_NODE *root){

}
