#include "symbolTable.h"
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

FILE *outputFile;
char *functionName;
int sIntRegs[12] = {};
int sFloatRegs[12] = {};
int tIntRegs[7] = {};
int tFloatRegs[7] = {};
int constNumber = 0;
int labelCount = 0;
static int localVarOffset;

static __inline__ DATA_TYPE getDataTypeByEntry(SymbolTableEntry* entry){return entry->attribute->attr.typeDescriptor->properties.dataType;}

static __inline__ void freeSIntReg(int reg){ sIntRegs[reg] = 0;}
static __inline__ void freeSFloatReg(int reg){ sFloatRegs[reg] = 0;}
static __inline__ void freeTIntReg(int reg){ tIntRegs[reg] = 0;}
static __inline__ void freeTFloatReg(int reg){ tFloatRegs[reg] = 0;}

int allocateSIntReg(){
    for(int i = 0; i < 12; i++){
        if(sIntRegs[i] == 0){
            sIntRegs[i] = 1;
            return i;
        }
    }
    return -1;
}

int allocateSFloatReg(){
    for(int i = 0; i < 12; i++){
        if(sFloatRegs[i] == 0){
            sFloatRegs[i] = 1;
            return i;
        }
    }
    return -1;
}

int allocateTIntReg(){
    for(int i = 0; i < 7; i++){
        if(tIntRegs[i] == 0){
            tIntRegs[i] = 1;
            return i;
        }
    }
    return -1;
}

int allocateTFloatReg(){
    for(int i = 0; i < 7; i++){
        if(tFloatRegs[i] == 0){
            tFloatRegs[i] = 1;
            return i;
        }
    }
    return -1;
}

static __inline__ char *getIdByNode(AST_NODE *node){
    if(node->nodeType == IDENTIFIER_NODE){
        return node->semantic_value.identifierSemanticValue.identifierName;
    }
    return NULL;
}

void genIntToFloat(AST_NODE *node, REG_TYPE regType){
    int tmp;
    char old, new;
    if(regType == T_REG){
        tmp = allocateTFloatReg();
        new = 't';
    }
    else if(regType == S_REG){
        tmp = allocateSFloatReg();
        new = 's';
    }
    if(node->regType == T_REG){
        freeTIntReg(node->reg[T_REG]);
        old = 't';
    }
    else if(node->regType == S_REG){
        freeSIntReg(node->reg[T_REG]);
        old = 's';
    }
    fprintf(outputFile, "\tfcvt.s.w\tf%c%d,%c%d\n", new, tmp, old, node->reg[node->regType]);
    node->regType = (new == T_REG)? T_REG : S_REG;
    node->reg[node->regType] = tmp;
    return;
}

void codeGeneration(AST_NODE *root);
void genPrologue();
void genEpilogue();
void genProgram(AST_NODE *root);
void genGlobalDeclarationList(AST_NODE *root);
void genGlobalDeclaration(AST_NODE *root);
void genFunctionDeclaration(AST_NODE *root);
void genBlockNode(AST_NODE *root);
void genGeneralNode(AST_NODE *node);
void genStatementList(AST_NODE *node);
void genStmtNode(AST_NODE *node);
void genFunctionCall(AST_NODE *node);
void genExprRelated(AST_NODE *node);
void genConstValue(AST_NODE *node);
void genLocalDeclarationList(AST_NODE *node);
void genLocalDeclaration(AST_NODE *node);
void genExpr(AST_NODE *node);
void genVariableRValue(AST_NODE *node);

void codeGeneration(AST_NODE *root){
    outputFile = fopen("output.s", "w+");
    genProgram(root);
    fclose(outputFile);
    return;
}

void genPrologue(){
    fprintf(outputFile, ".text\n");
    fprintf(outputFile, "_start_%s:\n", functionName); /* Generation of function start label*/
    fprintf(outputFile, "\tsd\tra,0(sp)\n");           /* sd    ra,0(sp) */
    fprintf(outputFile, "\tsd\tfp,-8(sp)\n");          /* sd    fp,-8(sp)  */
    fprintf(outputFile, "\taddi\tfp,sp,-8\n");         /* addi  fp,sp,-8  */ 
    fprintf(outputFile, "\taddi\tsp,sp,-16\n");        /* addi  sp,sp,-16  */

    
}   

void genEpilogue(){
    /* Restore */
    fprintf(outputFile, "\tld\tra,8(fp)\n");          /* ld    ra, 8(fp) */
    fprintf(outputFile, "\tmv\tsp,fp\n");             /* mv    sp, fp */  
    fprintf(outputFile, "\taddi\tsp,sp,8\n");         /* addi  sp, sp, 8 */
    fprintf(outputFile, "\tld\tfp,0(fp)\n");          /* ld    fp, 0(fp) */
    fprintf(outputFile, "\tjr\tra\n");                /* jr    ra */

}

void genProgram(AST_NODE *root){
    // There is two types of node being children of program node
    // VARIABLE_DECL_LIST_NODE and DECLARATION_NODE
    for(AST_NODE *globalDecl = root->child; globalDecl != NULL; globalDecl = globalDecl->rightSibling){
        if(globalDecl->nodeType == VARIABLE_DECL_LIST_NODE){
            //printf("[DEBUG] Node type is VARIABLE_DECL_LIST_NODE\n");
            genGlobalDeclarationList(globalDecl);      
        }
        else if(globalDecl->nodeType == DECLARATION_NODE){
            //printf("[DEBUG] Node type is DECLARATION_NODE\n");
            genFunctionDeclaration(globalDecl);
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
    DATA_TYPE dataType = processTypeNode(typeNode);
    //printf("TypeNode is %d\n", dataType);
    for(AST_NODE *id = typeNode->rightSibling; id != NULL; id = id->rightSibling){
        SymbolTableEntry *entry = id->semantic_value.identifierSemanticValue.symbolTableEntry;
        if(entry->attribute->attributeKind == TYPE_ATTRIBUTE){
            // TODO
            // printf("[DEBUG] Typedef\n");
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
                    if(dataType == INT_TYPE){
                        fprintf(outputFile, "_%s:  .word  0\n", getIdByNode(id));        
                    }
                    else if (dataType == FLOAT_TYPE){
                        fprintf(outputFile, "_%s:  .word 0x00000000\n");
                    }
                }
                else if (idKind == WITH_INIT_ID){
                    // Variables may be initialized as const value or expression.
                    //printf("With initial value\n");
                    //printf("Type is %d\n", typeNode->dataType);
                    if(dataType == INT_TYPE){
                        int val;
                        if(id->child->nodeType == EXPR_NODE && id->child->semantic_value.exprSemanticValue.isConstEval){
                            val = id->child->semantic_value.exprSemanticValue.constEvalValue.iValue;
                        }
                        else if(id->child->nodeType == CONST_VALUE_NODE){
                            val = id->child->semantic_value.const1->const_u.intval;
                        }
                        fprintf(outputFile, "_%s:  .word  %d\n", getIdByNode(id), val);
                    }
                    else if(dataType == FLOAT_TYPE){
                        float fVal;
                        if(id->child->nodeType == EXPR_NODE && id->child->semantic_value.exprSemanticValue.isConstEval){
                            fVal = id->child->semantic_value.exprSemanticValue.constEvalValue.fValue;
                        }
                        else if(id->child->nodeType == CONST_VALUE_NODE){
                            fVal = id->child->semantic_value.const1->const_u.fval;
                            //printf("%s=%f\n", getIdByNode(id), val);
                        }
                        fprintf(outputFile, "_%s:  .word  %#10x\n", getIdByNode(id), *(unsigned int*)&fVal);
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
    // Get function name and retur type
    functionName = getIdByNode(root->child->rightSibling);
    DATA_TYPE returnType = processTypeNode(root->child);
    //printf("Function name is %s and return type is %d\n", functionName, returnType);
    
    // Get all parameters and calculate offset
    AST_NODE *paramList = root->child->rightSibling->rightSibling;
    for(AST_NODE *param  = paramList->child; param != NULL; param = param->rightSibling){
        // In hw5 there is no function with parameters except "write", so we don't process this block this time.
        //printf("Param name is %s and type is %d\n", getIdByNode(param->child->rightSibling), processTypeNode(param->child));
    }

    genPrologue();
    localVarOffset = 0;
    genBlockNode(paramList->rightSibling);
    genEpilogue();
    
}


void genBlockNode(AST_NODE *root){
    for(AST_NODE *child = root->child ; child != NULL ; child = child->rightSibling){
        genGeneralNode(child);
    }
}

void genGeneralNode(AST_NODE *node){
    switch(node->nodeType){
        case VARIABLE_DECL_LIST_NODE:
            genLocalDeclarationList(node);
            break;
        case STMT_LIST_NODE:
            genStatementList(node);
            break;
        case NONEMPTY_ASSIGN_EXPR_LIST_NODE:
            /* TODO */
            break;
        case NONEMPTY_RELOP_EXPR_LIST_NODE:
            /* TODO */
            break;
        case NUL_NODE:
        default:
            break;
    }
}

void genStatementList(AST_NODE *node){
    for(AST_NODE *stmt = node->child ; stmt != NULL ; stmt = stmt->rightSibling){
        genStmtNode(stmt);
    }
}

void genStmtNode(AST_NODE *node){
    if(node->nodeType == NUL_NODE){
        return;
    }else if(node->nodeType == BLOCK_NODE){
        genBlockNode(node);
    }else{
        switch(node->semantic_value.stmtSemanticValue.kind){
            case WHILE_STMT:
                /* TODO */
                break;
            case FOR_STMT:
                /* TODO */
                break;
            case ASSIGN_STMT:
                /* TODO */
                break;
            case IF_STMT:
                /* TODO */
                break;
            case FUNCTION_CALL_STMT:
                genFunctionCall(node);
                break;
            case RETURN_STMT:
                /* TODO */
                //genReturnStmt(node);
                break;
        }
    }
}

void genFunctionCall(AST_NODE *node){
    char *functionName = getIdByNode(node->child);
    //printf("Function Name is %s\n", functionName);
    AST_NODE *relop_expr = node->child->rightSibling;
    if(strcmp(functionName, "write") == 0){
        genExprRelated(relop_expr->child);
        C_type constType = relop_expr->child->semantic_value.const1->const_type;
        if(constType == INTEGERC){
            fprintf(outputFile, "\tmv\ta0,t%d\n", relop_expr->child->reg[T_REG]);
            freeTIntReg(relop_expr->child->reg[T_REG]);
            fprintf(outputFile, "\tcall\t_write_int\n");
        }
        else if(constType == FLOATC){
            fprintf(outputFile, "\tfmv.s\tfa0,ft%d\n", relop_expr->child->reg[T_REG]);
            freeTFloatReg(relop_expr->child->reg[T_REG]);
            fprintf(outputFile, "\tcall\t_write_float\n");
        }
        else if(constType == STRINGC){
            fprintf(outputFile, "\tmv\ta0,t%d\n", relop_expr->child->reg[T_REG]);
            freeTIntReg(relop_expr->child->reg[T_REG]);
            fprintf(outputFile, "\tcall\t_write_str\n");
        }
    }
}

void genExprRelated(AST_NODE *node){
    //printf("gen Expr Related\n");
    //printf("ID name %s, type %d\n", getIdByNode(node), node->nodeType);
    switch(node->nodeType){
        case EXPR_NODE:
            genExpr(node);
            break;
        case STMT_NODE:
            genFunctionCall(node);
            break;
        case IDENTIFIER_NODE:
            /* TODO */
            genVariableRValue(node);
            break;
        case CONST_VALUE_NODE:
            genConstValue(node);
            break;
        default:
            break;
    }
}

void genConstValue(AST_NODE *node){
    C_type constType = node->semantic_value.const1->const_type; 
    if(constType == INTEGERC){
        //printf("int\n");
        node->regType = T_REG;
        node->reg[T_REG] = allocateTIntReg();
        fprintf(outputFile, "\tli\tt%d,%d\n", node->reg[T_REG], node->semantic_value.const1->const_u.intval);
    }
    else if(constType == FLOATC){
        //printf("float\n");
        node->regType = T_REG;
        node->reg[T_REG] = allocateTFloatReg();
        fprintf(outputFile, ".data\n");
        int tmpReg = allocateTIntReg();
        float fval = node->semantic_value.const1->const_u.fval;
        fprintf(outputFile, "_CONSTANT_%d:  .word  %#010x\n", constNumber, *(unsigned int*)&fval);
        fprintf(outputFile, ".text\n");
        fprintf(outputFile, "\tflw\tft%d,_CONSTANT_%d,t%d\n", node->reg[T_REG], constNumber, tmpReg);
        freeTIntReg(tmpReg);
        constNumber++;
    }
    else if(constType == STRINGC){
        //printf("string\n");
        node->regType = T_REG;
        node->reg[T_REG] = allocateTIntReg();
        fprintf(outputFile, ".data\n");
        int length = strlen(node->semantic_value.const1->const_u.sc);
        char *sval = (char*)malloc(sizeof(char) * (length + 3));
        strncpy(sval, node->semantic_value.const1->const_u.sc, length);
        sval[length - 1] = '\\';
        sval[length] = '0';
        sval[length + 1] = '\"';
        sval[length + 2] = '\0';
        //printf("%s\n", sval);
        fprintf(outputFile, "_CONSTANT_%d:  .ascii  %s\n", constNumber, sval);
        fprintf(outputFile, ".text\n");
        fprintf(outputFile, "\tla\tt%d,_CONSTANT_%d\n", node->reg[T_REG], constNumber);
        constNumber++;
        free(sval);
    }
}

void genLocalDeclarationList(AST_NODE *node){
    for(AST_NODE *decl = node->child ; decl != NULL ; decl = decl->rightSibling){
        genLocalDeclaration(decl);
    }
}

void genLocalDeclaration(AST_NODE *node){
    AST_NODE *typeNode = node->child;
    for(AST_NODE *id = typeNode->rightSibling ; id != NULL ; id = id->rightSibling){
        SymbolTableEntry *entry = id->semantic_value.identifierSemanticValue.symbolTableEntry;
        if(entry == NULL){
            printf("[DEBUG] entry is NULL.\n");
            exit(1);
        }
        TypeDescriptorKind kind = entry->attribute->attr.typeDescriptor->kind;
        if(kind == SCALAR_TYPE_DESCRIPTOR){
            localVarOffset -= 8;
            entry->offset = localVarOffset;
            if(id->semantic_value.identifierSemanticValue.kind == WITH_INIT_ID){
                AST_NODE *relop = id->child;
                genExprRelated(relop);
                DATA_TYPE dataType = processTypeNode(typeNode);
                if(dataType == INT_TYPE){
                    fprintf(outputFile ,"\tsd\tt%d,%d(s0)\n", relop->reg[T_REG], entry->offset);
                    freeTIntReg(relop->reg[T_REG]);
                }
                else if(dataType == FLOAT_TYPE){
                    fprintf(outputFile ,"\tfsd\tft%d,%d(s0)\n", relop->reg[T_REG], entry->offset);
                    freeTFloatReg(relop->reg[T_REG]);
                }
            }
        }
        else if(kind == ARRAY_TYPE_DESCRIPTOR){
            int size = 8;
            ArrayProperties arrayProperty = entry->attribute->attr.typeDescriptor->properties.arrayProperties;
            for(int dim = 0 ; dim < arrayProperty.dimension ; dim++){
                size *= arrayProperty.sizeInEachDimension[dim];
            }
            localVarOffset -= size;
            entry->offset = localVarOffset;
            //printf("size = %d\n", size);
        }
    }
}

void genExpr(AST_NODE *node){
    if(node->semantic_value.exprSemanticValue.isConstEval){
        if(node->dataType == INT_TYPE){
            node->regType = T_REG;
            node->reg[T_REG] = allocateTIntReg();
            int val = node->semantic_value.exprSemanticValue.constEvalValue.iValue;
            fprintf(outputFile, "\tli\tt%d,%d\n", val);
        }
        else if(node->dataType == FLOAT_TYPE){
            node->regType = T_REG;
            node->reg[T_REG] = allocateTFloatReg();
            float fVal = node->semantic_value.exprSemanticValue.constEvalValue.fValue;
            fprintf(outputFile, ".data\n");
            fprintf(outputFile, "_CONSTANT_%d: .word  %d\n", node->reg[T_REG], *(unsigned int*)&fVal);
            fprintf(outputFile, ".text\n");
            fprintf(outputFile, "\tli\tt%d,%d\n", fVal);
        }
        return;
    }
    
    EXPRSemanticValue exprSemanticValue = node->semantic_value.exprSemanticValue;
    if(exprSemanticValue.kind == UNARY_OPERATION){
        genExprRelated(node->child);
        node->regType = T_REG;
        node->reg[T_REG] = node->child->reg[T_REG];
        if(exprSemanticValue.op.unaryOp == UNARY_OP_NEGATIVE){
            printf("node type is %d\n", node->dataType);
            if(node->dataType == INT_TYPE){
                fprintf(outputFile, "\tneg\tt%d,t%d\n", node->reg[T_REG], node->reg[T_REG]);
            }
            else if(node->dataType == FLOAT_TYPE){
                fprintf(outputFile, "\tfneg.s\tft%d,ft%d\n", node->reg[T_REG], node->reg[T_REG]);
            }
        }
        else if(exprSemanticValue.op.unaryOp == UNARY_OP_LOGICAL_NEGATION){
            if(node->child->dataType == FLOAT_TYPE){
                int tmp = allocateTIntReg();
                fprintf(outputFile, "\tfcvt.w.s\tt%d,ft%d,rtz\n", tmp, node->child->reg[T_REG]);
                node->reg[T_REG] = tmp;
                node->dataType = INT_TYPE;
                freeTFloatReg(node->child->reg[T_REG]);
            }
            int reg = node->reg[T_REG];
            fprintf(outputFile, "\tnot\tt%d,t%d\n", reg, reg);        
        }
    }
    else{
        //BINARY_OPERATION
        AST_NODE *LHS = node->child;
        AST_NODE *RHS = LHS->rightSibling;
        genExprRelated(LHS);
        
        switch(node->semantic_value.exprSemanticValue.op.binaryOp){
            case BINARY_OP_ADD:
            case BINARY_OP_SUB:
            case BINARY_OP_MUL:
            case BINARY_OP_DIV:
            case BINARY_OP_EQ:
            case BINARY_OP_GE:
            case BINARY_OP_LE:
            case BINARY_OP_NE:
            case BINARY_OP_GT:
            case BINARY_OP_LT:
                genExprRelated(RHS);
                break;
            default:
                break;
        }
        

        //printf("LHS is %s type %d, RHS is %s type %d\n", getIdByNode(LHS), LHS->dataType, getIdByNode(RHS), RHS->dataType);
        DATA_TYPE logical_op_type;
        if(LHS->dataType != RHS->dataType){
            if(node->semantic_value.exprSemanticValue.op.binaryOp != BINARY_OP_AND && node->semantic_value.exprSemanticValue.op.binaryOp != BINARY_OP_AND){
                printf("Different!!\n");  
                if(LHS->dataType == INT_TYPE){
                    genIntToFloat(LHS, T_REG);
                }
                else if(RHS->dataType == INT_TYPE){
                    genIntToFloat(RHS, T_REG);
                }
            }
            else{
                if(LHS->dataType == INT_TYPE){
                    node->regType = T_REG;
                    node->reg[node->regType] = LHS->reg[T_REG];
                }
                else{
                    node->regType = T_REG;
                    node->reg[node->regType] = RHS->reg[T_REG];
                }
            }    
        }
        else if(LHS->dataType == RHS->dataType && LHS->dataType == INT_TYPE){
            switch(node->semantic_value.exprSemanticValue.op.binaryOp){
                case BINARY_OP_EQ:
                case BINARY_OP_GE:
                case BINARY_OP_LE:
                case BINARY_OP_NE:
                case BINARY_OP_GT:
                case BINARY_OP_LT:
                case BINARY_OP_AND:
                case BINARY_OP_OR:
                    logical_op_type = FLOAT_TYPE;
                    node->dataType = INT_TYPE;
                    node->regType = T_REG;
                    node->reg[T_REG] = allocateTIntReg();
                    break;
                default:
                    node->regType = T_REG;
                    node->reg[T_REG] = LHS->reg[T_REG];
                    break;
            }
        }
        else if(LHS->dataType == RHS->dataType){
            logical_op_type = INT_TYPE;
            node->regType = T_REG;
            node->reg[node->regType] = LHS->reg[T_REG];
        }

        switch(node->semantic_value.exprSemanticValue.op.binaryOp){
            case BINARY_OP_ADD:
            case BINARY_OP_SUB:
            case BINARY_OP_MUL:
            case BINARY_OP_DIV:
                if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_ADD){
                    fprintf(outputFile, "\tadd\tt%d,t%d,t%d\n", LHS->reg[T_REG], LHS->reg[T_REG], RHS->reg[T_REG]);
                }
                else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_SUB){
                    fprintf(outputFile, "\tsub\tt%d,t%d,t%d\n", LHS->reg[T_REG], LHS->reg[T_REG], RHS->reg[T_REG]);
                }
                else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_MUL){
                    fprintf(outputFile, "\tmul\tt%d,t%d,t%d\n", LHS->reg[T_REG], LHS->reg[T_REG], RHS->reg[T_REG]);
                }
                else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_DIV){
                    fprintf(outputFile, "\tdiv\tt%d,t%d,t%d\n", LHS->reg[T_REG], LHS->reg[T_REG], RHS->reg[T_REG]);
                }

                if(node->dataType == INT_TYPE){
                    freeTIntReg(RHS->reg[T_REG]);
                }    
                else{
                    freeTFloatReg(RHS->reg[T_REG]);
                }    
                break;
            case BINARY_OP_EQ:
            case BINARY_OP_GE:
            case BINARY_OP_LE:
            case BINARY_OP_NE:
            case BINARY_OP_GT:
            case BINARY_OP_LT:
                if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_EQ){
                    fprintf(outputFile, "\txor\tt%d,t%d,t%d\n", LHS->reg[T_REG], LHS->reg[T_REG], RHS->reg[T_REG]);
                    fprintf(outputFile, "\tseqz\tt%d,t%d\n", node->reg[T_REG], LHS->reg[T_REG]);
                }
                else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_GE){
                    fprintf(outputFile, "\tslt\tt%d,t%d,t%d\n", LHS->reg[T_REG], LHS->reg[T_REG], RHS->reg[T_REG]);
                    fprintf(outputFile, "\txori\tt%d,t%d,1\n", node->reg[T_REG], LHS->reg[T_REG]);
                }
                else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_LE){
                    fprintf(outputFile, "\tslt\tt%d,t%d,t%d\n", RHS->reg[T_REG], RHS->reg[T_REG], LHS->reg[T_REG]);
                    fprintf(outputFile, "\txori\tt%d,t%d,1\n", node->reg[T_REG], RHS->reg[T_REG]);
                }
                else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_NE){
                    fprintf(outputFile, "\txor\tt%d,t%d,t%d\n", LHS->reg[T_REG], LHS->reg[T_REG], RHS->reg[T_REG]);
                    fprintf(outputFile, "\tsnez\tt%d,t%d\n", node->reg[T_REG], LHS->reg[T_REG]);
                }
                else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_GT){
                    fprintf(outputFile, "\tslt\tt%d,t%d,t%d\n", LHS->reg[T_REG], LHS->reg[T_REG], RHS->reg[T_REG]);
                }
                else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_LT){
                    fprintf(outputFile, "\tslt\tt%d,t%d,t%d\n", RHS->reg[T_REG], RHS->reg[T_REG], LHS->reg[T_REG]);
                }

                if(LHS->dataType == FLOAT_TYPE){
                    freeTFloatReg(LHS->reg[T_REG]);
                }
                if(RHS->dataType == INT_TYPE){
                    freeTIntReg(RHS->reg[T_REG]);
                }
                else {
                    freeTFloatReg(RHS->reg[T_REG]);
                }
                break;

            case BINARY_OP_AND:{
                int short_circuit_label = labelCount;
                int end_label = labelCount + 1;
                labelCount++;
                fprintf(outputFile, "\tbeq\tt%d,x0,_LABEL_%d\n", LHS->reg[T_REG], short_circuit_label);
                
                /* RHS */
                fprintf(outputFile, "\tj\t_LABEL_%d\n", end_label);
                fprintf(outputFile, "_LABEL_%d:\n", end_label);
                genExprRelated(RHS);
                fprintf(outputFile, "\tand\tt%d,t%d,t%d\n", LHS->reg[T_REG], LHS->reg[T_REG], RHS->reg[T_REG]);
                fprintf(outputFile, "\tj\t_LABEL_%d\n", short_circuit_label);
                fprintf(outputFile, "_LABEL_%d:\n", short_circuit_label);
                fprintf(outputFile, "\tand\tt%d,t%d,1\n", node->reg[T_REG], LHS->reg[T_REG]);
                
                if(LHS->dataType == FLOAT_TYPE){
                    freeTFloatReg(LHS->reg[T_REG]);
                }
                else if(LHS->reg[T_REG] == node->reg[T_REG]){
                    freeTIntReg(LHS->reg[T_REG]);
                }    
                if(RHS->dataType == FLOAT_TYPE){
                    freeTFloatReg(RHS->reg[T_REG]);
                }
                else if(RHS->reg[T_REG] == node->reg[T_REG]){
                    freeTIntReg(RHS->reg[T_REG]);
                }
                break;
            }
            
            case BINARY_OP_OR:{
                int short_circuit_label = labelCount;
                int end_label = labelCount + 1;
                labelCount++;
                fprintf(outputFile, "\tbne\tt%d,x0,_LABEL_%d\n", LHS->reg[T_REG], short_circuit_label);
                
                /* RHS */
                fprintf(outputFile, "\tj\t_LABEL_%d\n", end_label);
                fprintf(outputFile, "_LABEL_%d:\n", end_label);
                genExprRelated(RHS);
                fprintf(outputFile, "\tor\tt%d,t%d,t%d\n", LHS->reg[T_REG], LHS->reg[T_REG], RHS->reg[T_REG]);
                fprintf(outputFile, "\tj\t_LABEL_%d\n", short_circuit_label);
                fprintf(outputFile, "_LABEL_%d:\n", short_circuit_label);
                fprintf(outputFile, "\tor\tt%d,t%d,1\n", node->reg[T_REG], LHS->reg[T_REG]);
                
                if(LHS->dataType == FLOAT_TYPE){
                    freeTFloatReg(LHS->reg[T_REG]);
                }
                else if(LHS->reg[T_REG] == node->reg[T_REG]){
                    freeTIntReg(LHS->reg[T_REG]);
                }    
                if(RHS->dataType == FLOAT_TYPE){
                    freeTFloatReg(RHS->reg[T_REG]);
                }
                else if(RHS->reg[T_REG] == node->reg[T_REG]){
                    freeTIntReg(RHS->reg[T_REG]);
                }
                break;
            }    
        }

    }
}
        
void genVariableRValue(AST_NODE *node){
    SymbolTableEntry *entry = node->semantic_value.identifierSemanticValue.symbolTableEntry;
    DATA_TYPE dataType = getDataTypeByEntry(entry);
    if(dataType == FLOAT_TYPE){
        node->regType = T_REG;
        node->reg[T_REG] = allocateTFloatReg();
    }
    else{
        node->regType = T_REG;
        node->reg[T_REG] = allocateTIntReg();
    }

    IDENTIFIER_KIND kind = node->semantic_value.identifierSemanticValue.kind;
    if(kind == NORMAL_ID){
        if(entry->scope > 0){
            //printf("Local Variable reference\n");
            //printf("data type is %d\n", dataType);
            //printf("Reg is %d\n", node->reg[T_REG]);
            if(dataType == INT_TYPE){
                fprintf(outputFile, "\tlw\tt%d,%d(s0)\n", node->reg[T_REG], entry->offset);
            }
            else if(dataType == FLOAT_TYPE){
                fprintf(outputFile, "\tflw\tft%d,%d(s0)\n", node->reg[T_REG], entry->offset);
            }
        }
        else{
            //printf("Global variable reference\n");
            if(dataType == INT_TYPE){
                fprintf(outputFile, "\tlw\tt%d,_%s\n", node->reg[T_REG], getIdByNode(node));
            }
            else if(dataType == FLOAT_TYPE){
                int tmp = allocateTIntReg();
                fprintf(outputFile, "\tflw\tft%d,_%s,t%d\n", node->reg[T_REG], getIdByNode(node), tmp);
                freeTIntReg(tmp);
            }
        }
    }
    else if(kind == ARRAY_ID){
        /* TODO */
    }
}
