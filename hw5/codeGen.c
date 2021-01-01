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
int tFloatRegs[12] = {};
int constNumber = 0;
int labelCount = 0;
int ifWithReturn = 0;
static int localVarOffset;
DATA_TYPE functionReturnType;

static __inline__ DATA_TYPE getDataTypeByEntry(SymbolTableEntry* entry){return entry->attribute->attr.typeDescriptor->properties.dataType;}

static __inline__ void freeSIntReg(int reg){ sIntRegs[reg] = 0;}
static __inline__ void freeSFloatReg(int reg){ sFloatRegs[reg] = 0;}
static __inline__ void freeTIntReg(int reg){ tIntRegs[reg] = 0;}
static __inline__ void freeTFloatReg(int reg){ tFloatRegs[reg] = 0;}

int allocateSIntReg(){
    for(int i = 1; i < 12; i++){
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

void genFloatToInt(AST_NODE *node, REG_TYPE regType){
    int tmp;
    char old, new;
    if(regType == T_REG){
        tmp = allocateTIntReg();
        new = 't';
    }
    else if(regType == S_REG){
        tmp = allocateSIntReg();
        new = 's';
    }
    if(node->regType == T_REG){
    freeTFloatReg(node->reg[T_REG]);
    old = 't';
    }
    else if(node->regType == S_REG){
        freeSFloatReg(node->reg[T_REG]);
        old = 's';
    }
    fprintf(outputFile, "\tfcvt.w.s\t%c%d,f%c%d,rtz\n", new, tmp, old, node->reg[node->regType]);
    node->regType = (new == 't')? T_REG : S_REG;
    node->reg[node->regType] = tmp;
    return;
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
    node->regType = (new == 't')? T_REG : S_REG;
    node->reg[T_REG] = tmp;
    printf("tmp  is %d\n", tmp);
    return;
}

void freeIntReg(AST_NODE *node){
    if(node->regType == T_REG){
        freeTIntReg(node->reg[T_REG]);
        //printf("free Int %d\n", node->reg[T_REG]);
    }
    else if(node->regType == S_REG){
        freeSIntReg(node->reg[S_REG]);
        //printf("free Int %d\n", node->reg[S_REG]);
    }
}

void freeFloatReg(AST_NODE *node){
    if(node->regType == T_REG){
        freeTFloatReg(node->reg[T_REG]);
        //printf("free Float %d\n", node->reg[T_REG]);
    }
    else if(node->regType == S_REG){
        freeSFloatReg(node->reg[S_REG]);
        //printf("free Float %d\n", node->reg[T_REG]);
    }
}

int allocateIntReg(AST_NODE *node){
    if(node->regType == T_REG){
        return allocateTIntReg();
    } 
    else if(node->regType == S_REG){
        return allocateSIntReg();    
    }
}

int allocateFloatReg(AST_NODE *node){
    if(node->regType == T_REG){
        return allocateTFloatReg();
    } 
    else if(node->regType == S_REG){
        return allocateSFloatReg();    
    }
}

int genSaveCallerReg(){
    int count = 0;
    for(int i = 1 ; i < 12 ; i++){
        if(sIntRegs[i]){
            fprintf(outputFile, "\tsw\ts%d,%d(sp)\n", i, -8*count);
            //printf("\tsw\ts%d,%d(sp)\n", i, -8*count);
            count++;
        }
    }
    for(int i = 0 ; i < 12 ; i++){
        if(sFloatRegs[i]){
            fprintf(outputFile, "\tfsw\tfs%d,%d(sp)\n", i, -8*count);
            //printf("\tfsw\tfs%d,%d(sp)\n", i, -8*count);
            count++;
        }
    }
    return count;
}

void genRestoreCallerReg(){
    int count = 0;
    for(int i = 1 ; i < 12 ; i++){
        if(sIntRegs[i]){
            fprintf(outputFile, "\tlw\ts%d,%d(sp)\n", i, -8*count);
            count++;
        }
    }
    for(int i = 0 ; i < 12 ; i++){
        if(sFloatRegs[i]){
            fprintf(outputFile, "\tflw\tfs%d,%d(sp)\n", i, -8*count);
            count++;
        }
    }
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
void genStmt(AST_NODE *node);
void genFunctionCall(AST_NODE *node);
void genExprRelated(AST_NODE *node);
void genConstValue(AST_NODE *node);
void genLocalDeclarationList(AST_NODE *node);
void genLocalDeclaration(AST_NODE *node);
void genExpr(AST_NODE *node);
void genVariableRValue(AST_NODE *node);
void genAssignOrExpr(AST_NODE *node);
void genAssignmentStmt(AST_NODE *node);
void genWhileStmt(AST_NODE *node);
void genIfStmt(AST_NODE *node);
void genReturnStmt(AST_NODE *node);
void genArrayRef(AST_NODE *node);

void codeGeneration(AST_NODE *root){
    outputFile = fopen("output.s", "w+");
    genProgram(root);
    fclose(outputFile);
    return;
}

void genPrologue(){
    fprintf(outputFile, ".text\n");
    fprintf(outputFile, "_%s_Prologue:\n", functionName);
    fprintf(outputFile, "_start_%s:\n", functionName); /* Generation of function start label*/
    fprintf(outputFile, "\tsd\tra,0(sp)\n");           /* sd    ra,0(sp) */
    fprintf(outputFile, "\tsd\tfp,-8(sp)\n");          /* sd    fp,-8(sp)  */
    fprintf(outputFile, "\taddi\tfp,sp,-8\n");         /* addi  fp,sp,-8  */ 
    fprintf(outputFile, "\taddi\tsp,sp,-16\n");        /* addi  sp,sp,-16  */
    /* prepare frame */
    fprintf(outputFile, "\tla\tra,_frameSize_%s\n", functionName);
    fprintf(outputFile, "\tlw\tra,0(ra)\n");
    fprintf(outputFile, "\tsub\tsp,sp,ra\n");
    int size = 8;
    for(int i = 0; i < 7; i++){
        fprintf(outputFile, "\tsw\tt%d,%d(sp)\n", i, size);
        size += 8;
    }
    for(int i = 0; i < 12; i++){
        fprintf(outputFile, "\tfsw\tft%d,%d(sp)\n", i, size);
        size += 8;
    }
    /*
    for(int i = 0; i < 8; i++){
        fprintf(outputFile, "\tsw\ta%d,%d(sp)\n", i, size);
        size += 8;
    }
    for(int i = 0; i < 8; i++){
        fprintf(outputFile, "\tfsw\tfa%d,%d(sp)\n", i, size);
        size += 8;
    }
    */
}   

void genEpilogue(){ 
    fprintf(outputFile, ".text\n");
    fprintf(outputFile, "_%s_Epilogue:\n", functionName);
    /* Restore */
    int size = 8;
    for(int i = 0; i < 7; i++){
        fprintf(outputFile, "\tlw\tt%d,%d(sp)\n", i, size);
        size += 8;
    }
    for(int i = 0; i < 12; i++){
        fprintf(outputFile, "\tflw\tft%d,%d(sp)\n", i, size);
        size += 8;
    }
    /*
    for(int i = 0; i < 8; i++){
        fprintf(outputFile, "\tlw\ta%d,%d(sp)\n", i, size);
        size += 8;
    }
    for(int i = 0; i < 8; i++){
        fprintf(outputFile, "\tflw\tfa%d,%d(sp)\n", i, size);
        size += 8;
    }
    */
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
                        fprintf(outputFile, "_%s:  .word 0x00000000\n", getIdByNode(id));
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
                printf("Array type %s\n", id->semantic_value.identifierSemanticValue.identifierName);
                int size = 8;
                ArrayProperties property = id->semantic_value.identifierSemanticValue.symbolTableEntry->attribute->attr.typeDescriptor->properties.arrayProperties;
                for(int dim = 0; dim < property.dimension; dim++){
                    size *= property.sizeInEachDimension[dim];
                }
                fprintf(outputFile, "_%s:  .zero  %d\n", getIdByNode(id), size);
            }
        }
    }
}

void genFunctionDeclaration(AST_NODE *root){
    // Get function name and retur type
    functionName = getIdByNode(root->child->rightSibling);
    functionReturnType = processTypeNode(root->child);
    //printf("Function name is %s and return type is %d\n", functionName, functionReturnType);
    
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
    fprintf(outputFile, ".data\n");
    fprintf(outputFile, "_frameSize_%s:  .word  %d\n", functionName, -localVarOffset + 20*8);
    
    for(int i = 1; i < 12; i ++){
        sIntRegs[i] = 0;
    }
    for(int i = 0; i < 12; i++){
        sFloatRegs[i] = 0;
    }
}


void genBlockNode(AST_NODE *root){
    for(AST_NODE *child = root->child ; child != NULL ; child = child->rightSibling){
        genGeneralNode(child);
    }
}

void genGeneralNode(AST_NODE *node){
    printf("General Node type is %d\n", node->nodeType);
    switch(node->nodeType){
        case VARIABLE_DECL_LIST_NODE:
            genLocalDeclarationList(node);
            break;
        case STMT_LIST_NODE:
            genStatementList(node);
            break;
        case NONEMPTY_ASSIGN_EXPR_LIST_NODE:
            for(AST_NODE *child = node->child ; child != NULL ; child = child->rightSibling){
                genAssignOrExpr(child);
            }
            break;
        case NONEMPTY_RELOP_EXPR_LIST_NODE:
            for(AST_NODE *child = node->child ; child != NULL ; child = child->rightSibling){
                genExprRelated(child);
            } 
            break;
        case NUL_NODE:
        default:
            break;
    }
}

void genStatementList(AST_NODE *node){
    for(AST_NODE *stmt = node->child ; stmt != NULL ; stmt = stmt->rightSibling){
        genStmt(stmt);
    }
}

void genStmt(AST_NODE *node){
    printf("-----Stmt node type is %d\n",node->nodeType );
    if(node->nodeType == NUL_NODE){
        return;
    }
    else if(node->nodeType == BLOCK_NODE){
        genBlockNode(node);
    }
    else{
        printf("Stmt kind is %d\n", node->semantic_value.stmtSemanticValue.kind);
        switch(node->semantic_value.stmtSemanticValue.kind){
            case WHILE_STMT:
                genWhileStmt(node);
                break;
            case FOR_STMT:
                /* TODO */
                break;
            case ASSIGN_STMT:
                printf("-----genStmt\n");
                genAssignmentStmt(node);
                break;
            case IF_STMT:
                genIfStmt(node);
                break;
            case FUNCTION_CALL_STMT:
                genFunctionCall(node);
                break;
            case RETURN_STMT:
                genReturnStmt(node);
                break;
        }
    }
    printf("after genStmt\n");
}

void genFunctionCall(AST_NODE *node){
    char *functionName = getIdByNode(node->child);
    //printf("Function Name is %s\n", functionName);
    //printf("Function name address %d\n", node->child->rightSibling->child->dataType);
    AST_NODE *relop_expr = node->child->rightSibling;
    FunctionSignature *signature = node->child->semantic_value.identifierSemanticValue.symbolTableEntry->attribute->attr.functionSignature;
    if(strcmp(functionName, "write") == 0){
        genExprRelated(relop_expr->child);
        printf("----------------------------------expr type is %d\n", relop_expr->child->dataType);
        //C_type constType = relop_expr->child->semantic_value.const1->const_type;
        relop_expr->regType = relop_expr->child->regType;
        relop_expr->reg[relop_expr->regType] = relop_expr->child->reg[relop_expr->child->regType];
        DATA_TYPE dataType = relop_expr->child->dataType;
        char r = (relop_expr->regType == T_REG)? 't' : 's';
        if(dataType == INT_TYPE){
            fprintf(outputFile, "\tmv\ta0,%c%d\n", r, relop_expr->reg[relop_expr->regType]);
            freeIntReg(relop_expr);
            fprintf(outputFile, "\tcall\t_write_int\n");
        }
        else if(dataType == FLOAT_TYPE){
            fprintf(outputFile, "\tfmv.s\tfa0,f%c%d\n", r, relop_expr->reg[relop_expr->regType]);
            freeFloatReg(relop_expr);
            fprintf(outputFile, "\tcall\t_write_float\n");
        }
        else if(dataType == CONST_STRING_TYPE){
            fprintf(outputFile, "\tmv\ta0,%c%d\n", r, relop_expr->child->reg[relop_expr->regType]);
            freeIntReg(relop_expr);
            fprintf(outputFile, "\tcall\t_write_str\n");
        }
    }
    else if(strcmp(functionName, "read") == 0){
        fprintf(outputFile, "\tcall\t_read_int\n");
        node->regType = S_REG;
        node->reg[S_REG] = allocateSIntReg();
        fprintf(outputFile, "\tmv\ts%d,a0\n", node->reg[S_REG]);
    }
    else if(strcmp(functionName, "fread") == 0){
        fprintf(outputFile, "\tcall\t_read_float\n");
        node->regType = S_REG;
        node->reg[S_REG] = allocateSFloatReg();
        fprintf(outputFile, "\tfmv.s\tfs%d,fa0\n", node->reg[S_REG]);
    }
    else if(signature->parametersCount == 0){
        DATA_TYPE returnType = signature->returnType;
        if(returnType == INT_TYPE){
            node->regType = S_REG;
            node->reg[S_REG] = allocateSIntReg();
        }
        else if(returnType == FLOAT_TYPE){
            node->regType = S_REG;
            node->reg[S_REG] = allocateSFloatReg();
        }
        int savedNumber = genSaveCallerReg();
        if(savedNumber > 0){
            fprintf(outputFile, "\taddi\tsp,sp,%d\n", savedNumber*(-8));
        }
        fprintf(outputFile, "\tjal\t_%s_Prologue\n", functionName);
        if(savedNumber > 0){
            fprintf(outputFile, "\taddi\tsp,sp,%d\n", savedNumber*8);
            genRestoreCallerReg();
        }
        if(returnType == INT_TYPE){
            fprintf(outputFile, "\tmv\ts%d,a0\n", node->reg[S_REG]);    
        }
        else if(returnType == FLOAT_TYPE){
            fprintf(outputFile, "\tfmv.s\tfs%d,fa0\n", node->reg[S_REG]);
        }
    }
    else if(signature->parametersCount > 0){

    }
}

void genExprRelated(AST_NODE *node){
    //printf("gen Expr Related\n");
    printf("ID name %s, type %d\n", getIdByNode(node), node->nodeType);
    switch(node->nodeType){
        case EXPR_NODE:
            genExpr(node);
            break;
        case STMT_NODE:
            genFunctionCall(node);
            break;
        case IDENTIFIER_NODE:
            /* TODO */
            printf("In ID\n");
            genVariableRValue(node);
            break;
        case CONST_VALUE_NODE:
            genConstValue(node);
            break;
        default:
            break;
    }
    printf("After genExprRelated\n");
}

void genConstValue(AST_NODE *node){
    printf("Const value data type is %d\n", node->dataType);
    C_type constType = node->semantic_value.const1->const_type; 
    if(constType == INTEGERC){
        node->regType = T_REG;
        node->reg[T_REG] = allocateTIntReg();
        //printf("REG is %d\n", node->reg[T_REG]);
        //printf("\tli\tt%d,%d\n", node->reg[T_REG], node->semantic_value.const1->const_u.intval);
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
                char r = (relop->regType == T_REG)? 't': 's';
                if(dataType != relop->dataType){
                    if(dataType == INT_TYPE){
                        genFloatToInt(relop, relop->regType);
                    }
                    else if(dataType == FLOAT_TYPE){
                        genIntToFloat(relop, relop->regType);
                    }
                }
                if(dataType == INT_TYPE){
                    fprintf(outputFile ,"\tsd\t%c%d,%d(s0)\n", r, relop->reg[relop->regType], entry->offset);
                    freeIntReg(relop);
                }
                else if(dataType == FLOAT_TYPE){
                    fprintf(outputFile ,"\tfsd\tf%c%d,%d(s0)\n", r, relop->reg[relop->regType], entry->offset);
                    freeFloatReg(relop);
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
            fprintf(outputFile, "\tli\tt%d,%d\n", node->reg[T_REG], val);
        }
        else if(node->dataType == FLOAT_TYPE){
            node->regType = T_REG;
            node->reg[T_REG] = allocateTFloatReg();
            int tmpReg = allocateTIntReg();
            float fVal = node->semantic_value.exprSemanticValue.constEvalValue.fValue;
            fprintf(outputFile, ".data\n");
            fprintf(outputFile, "_CONSTANT_%d: .word  %#010x\n", constNumber, *(unsigned int*)&fVal);
            fprintf(outputFile, ".text\n");
            fprintf(outputFile, "\tflw\tft%d,_CONSTANT_%d,t%d\n", node->reg[T_REG], constNumber, tmpReg);
            freeTIntReg(tmpReg);
            constNumber++;
        }
        return;
    }
    
    EXPRSemanticValue exprSemanticValue = node->semantic_value.exprSemanticValue;
    if(exprSemanticValue.kind == UNARY_OPERATION){
        genExprRelated(node->child);
        node->regType = T_REG;
        node->reg[node->regType] = node->child->reg[node->child->regType];
        if(exprSemanticValue.op.unaryOp == UNARY_OP_NEGATIVE){
            if(node->dataType == INT_TYPE){
                fprintf(outputFile, "\tneg\tt%d,t%d\n", node->reg[node->regType], node->reg[node->regType]);
            }
            else if(node->dataType == FLOAT_TYPE){
                fprintf(outputFile, "\tfneg.s\tft%d,ft%d\n", node->reg[node->regType], node->reg[node->regType]);
            }
        }
        else if(exprSemanticValue.op.unaryOp == UNARY_OP_LOGICAL_NEGATION){
            if(node->child->dataType == FLOAT_TYPE){
                int tmp = allocateTIntReg();
                fprintf(outputFile, "\tfcvt.w.s\tt%d,ft%d,rtz\n", tmp, node->child->reg[node->child->regType]);
                node->reg[node->regType] = tmp;
                node->dataType = INT_TYPE;
                freeTFloatReg(node->child->reg[node->child->regType]);
            }
            int reg = node->reg[node->regType];
            fprintf(outputFile, "\tseqz\tt%d,t%d\n", reg, reg);        
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
        
        //printf("Node dataType is %d\n", node->dataType);
        //printf("LHS is %s type %d, RHS is %s type %d\n", getIdByNode(LHS), LHS->dataType, getIdByNode(RHS), RHS->dataType);
        DATA_TYPE logical_op_type;
        if(LHS->dataType != RHS->dataType){
            printf("Different!!\n");
            logical_op_type = FLOAT_TYPE; 
            BINARY_OPERATOR operation = node->semantic_value.exprSemanticValue.op.binaryOp;
            switch(operation){
                case BINARY_OP_ADD:
                case BINARY_OP_SUB:
                case BINARY_OP_MUL:
                case BINARY_OP_DIV:    
                    if(LHS->dataType == INT_TYPE){
                        genIntToFloat(LHS, T_REG);
                        LHS->dataType = FLOAT_TYPE;
                        node->regType = LHS->regType;
                        node->reg[node->regType] = LHS->reg[LHS->regType];
                    }
                    else if(RHS->dataType == INT_TYPE){
                        genIntToFloat(RHS, T_REG);
                        RHS->dataType = FLOAT_TYPE;
                        node->regType = LHS->regType;
                        node->reg[node->regType] = LHS->reg[T_REG];
                    }
                    break;
                case BINARY_OP_EQ:
                case BINARY_OP_GE:
                case BINARY_OP_LE:
                case BINARY_OP_NE:
                case BINARY_OP_GT:
                case BINARY_OP_LT:
                    if(LHS->dataType == INT_TYPE){
                        genIntToFloat(LHS, T_REG);
                        LHS->dataType = FLOAT_TYPE;
                        node->regType = LHS->regType;
                        node->reg[node->regType] = allocateIntReg(node);
                    }
                    else if(RHS->dataType == INT_TYPE){
                        genIntToFloat(RHS, T_REG);
                        RHS->dataType = FLOAT_TYPE;
                        node->regType = LHS->regType;
                        node->reg[node->regType] = allocateIntReg(node);
                    }
                    break;
                case BINARY_OP_AND:
                case BINARY_OP_OR:
                    if(LHS->dataType == INT_TYPE){
                        node->regType = LHS->regType;
                        node->reg[node->regType] = LHS->reg[LHS->regType];
                    }
                    else{
                        node->regType = LHS->regType;
                        node->reg[node->regType] = RHS->reg[LHS->regType];
                    }
                    break;
                default:
                    break;
            }
        }
        else if(LHS->dataType == RHS->dataType && LHS->dataType == FLOAT_TYPE){
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
                    node->regType = LHS->regType;
                    node->reg[node->regType] = allocateIntReg(node);
                    break;
                default:
                    node->regType = LHS->regType;
                    node->reg[node->regType] = LHS->reg[LHS->regType];
                    break;
            }
        }
        else if(LHS->dataType == RHS->dataType){
            logical_op_type = INT_TYPE;
            node->regType = LHS->regType;
            node->reg[node->regType] = LHS->reg[LHS->regType];
        }
        
        char l, r, n;
        l = (LHS->regType == T_REG)? 't' : 's';
        r = (RHS->regType == T_REG)? 't' : 's';
        n = (node->regType == T_REG)? 't' : 's';
        int lReg, rReg, nReg;
        lReg = LHS->regType;
        rReg = RHS->regType;
        nReg = node->regType;
        switch(node->semantic_value.exprSemanticValue.op.binaryOp){
            case BINARY_OP_ADD:
            case BINARY_OP_SUB:
            case BINARY_OP_MUL:
            case BINARY_OP_DIV:
                if(node->dataType == INT_TYPE){
                    if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_ADD){
                        fprintf(outputFile, "\tadd\t%c%d,%c%d,%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_SUB){
                        fprintf(outputFile, "\tsub\t%c%d,%c%d,%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_MUL){
                        fprintf(outputFile, "\tmul\t%c%d,%c%d,%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_DIV){
                        fprintf(outputFile, "\tdiv\t%c%d,%c%d,%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                    freeIntReg(RHS);
                    //printf("free %c REG number %d\n" , (RHS->regType == T_REG)? 't': 's', RHS->reg[RHS->regType]);
                }    
                else if(node->dataType == FLOAT_TYPE){
                    if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_ADD){
                        fprintf(outputFile, "\tfadd.s\tf%c%d,f%c%d,f%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_SUB){
                        fprintf(outputFile, "\tfsub.s\tf%c%d,f%c%d,f%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_MUL){
                        fprintf(outputFile, "\tfmul.s\tf%c%d,f%c%d,f%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_DIV){
                        fprintf(outputFile, "\tfdiv.s\tf%c%d,f%c%d,f%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                    freeFloatReg(RHS);
                    //printf("free %c REG number %d\n" , (RHS->regType == T_REG)? 't': 's', RHS->reg[RHS->regType]);
                }    
                break;
            case BINARY_OP_EQ:
            case BINARY_OP_GE:
            case BINARY_OP_LE:
            case BINARY_OP_NE:
            case BINARY_OP_GT:
            case BINARY_OP_LT:
                if(logical_op_type == INT_TYPE){
                    if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_EQ){
                        fprintf(outputFile, "\txor\t%c%d,%c%d,%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                        fprintf(outputFile, "\tseqz\t%c%d,%c%d\n", n, node->reg[nReg], l, LHS->reg[lReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_GE){
                        fprintf(outputFile, "\tslt\t%c%d,%c%d,%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                        fprintf(outputFile, "\txori\t%c%d,%c%d,1\n", n, node->reg[nReg], l, LHS->reg[lReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_LE){
                        fprintf(outputFile, "\tslt\t%c%d,%c%d,%c%d\n", r, RHS->reg[rReg], r, RHS->reg[rReg], l, LHS->reg[lReg]);
                        fprintf(outputFile, "\txori\t%c%d,%c%d,1\n", n, node->reg[nReg], r, RHS->reg[rReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_NE){
                        fprintf(outputFile, "\txor\t%c%d,%c%d,%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                        fprintf(outputFile, "\tsnez\t%c%d,%c%d\n", n, node->reg[nReg], l, LHS->reg[lReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_GT){
                        fprintf(outputFile, "\tslt\t%c%d,%c%d,%c%d\n", l, LHS->reg[lReg], r, RHS->reg[rReg], l, LHS->reg[lReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_LT){
                        fprintf(outputFile, "\tslt\t%c%d,%c%d,%c%d\n", l, LHS->reg[lReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                }    
                else if(logical_op_type == FLOAT_TYPE){
                    if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_EQ){
                        fprintf(outputFile, "\tfeq.s\t%c%d,f%c%d,f%c%d\n", n, node->reg[nReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_GE){
                        fprintf(outputFile, "\tfle.s\t%c%d,f%c%d,f%c%d\n", n, node->reg[nReg], r, RHS->reg[rReg], l, LHS->reg[lReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_LE){
                        fprintf(outputFile, "\tfle.s\t%c%d,f%c%d,f%c%d\n", n, node->reg[nReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_NE){
                        fprintf(outputFile, "\tfeq.s\t%c%d,f%c%d,f%c%d\n", n, node->reg[nReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                        fprintf(outputFile, "\txori\t%c%d,%c%d,1\n", n, node->reg[nReg], n, node->reg[nReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_GT){
                        fprintf(outputFile, "\tflt.s\t%c%d,f%c%d,f%c%d\n", n, node->reg[nReg], r, RHS->reg[rReg], l, LHS->reg[lReg]);
                    }
                    else if(node->semantic_value.exprSemanticValue.op.binaryOp == BINARY_OP_LT){
                        fprintf(outputFile, "\tflt.s\t%c%d,f%c%d,f%c%d\n", n, node->reg[nReg], l, LHS->reg[lReg], r, RHS->reg[rReg]);
                    }
                }

                if(LHS->dataType == FLOAT_TYPE){
                    freeFloatReg(LHS);
                }
                if(RHS->dataType == INT_TYPE){
                    freeIntReg(RHS);
                }
                else {
                    freeFloatReg(RHS);
                }
                break;

            case BINARY_OP_AND:{
                int short_circuit_label = labelCount;
                labelCount++;
                int end_label = labelCount;
                labelCount++;
                fprintf(outputFile, "\tbeq\t%c%d,x0,_LABEL_%d\n", l, LHS->reg[lReg], short_circuit_label);
                
                /* RHS */
                fprintf(outputFile, "\tj\t_LABEL_%d\n", end_label);
                fprintf(outputFile, "_LABEL_%d:\n", end_label);
                genExprRelated(RHS);
                r = (RHS->regType == T_REG)? 't' : 's';
                rReg = RHS->regType;
                fprintf(outputFile, "\tsnez\t%c%d,%c%d\n", l, LHS->reg[lReg],  r, RHS->reg[rReg]);
                fprintf(outputFile, "\tj\t_LABEL_%d\n", short_circuit_label);
                fprintf(outputFile, "_LABEL_%d:\n", short_circuit_label);
                fprintf(outputFile, "\tandi\t%c%d,%c%d,1\n", n, node->reg[nReg], l, LHS->reg[lReg]);
                
                if(LHS->dataType == FLOAT_TYPE){
                    freeTFloatReg(LHS->reg[lReg]);
                }
                else if(LHS->reg[lReg] == node->reg[nReg]){
                    freeTIntReg(LHS->reg[lReg]);
                }    
                if(RHS->dataType == FLOAT_TYPE){
                    freeTFloatReg(RHS->reg[rReg]);
                }
                else if(RHS->reg[rReg] == node->reg[nReg]){
                    freeTIntReg(RHS->reg[rReg]);
                }
                break;
            }
            
            case BINARY_OP_OR:{
                int short_circuit_label = labelCount;
                labelCount++;
                int end_label = labelCount;
                labelCount++;
                fprintf(outputFile, "\tbne\t%c%d,x0,_LABEL_%d\n", l, LHS->reg[lReg], short_circuit_label);
                
                /* RHS */
                fprintf(outputFile, "\tj\t_LABEL_%d\n", end_label);
                fprintf(outputFile, "_LABEL_%d:\n", end_label);
                genExprRelated(RHS);
                r = (RHS->regType == T_REG)? 't' : 's';
                rReg = RHS->regType;
                fprintf(outputFile, "\tsnez\t%c%d,%c%d\n", l, LHS->reg[lReg], r, RHS->reg[rReg]);
                fprintf(outputFile, "\tj\t_LABEL_%d\n", short_circuit_label);
                fprintf(outputFile, "_LABEL_%d:\n", short_circuit_label);
                fprintf(outputFile, "\tandi\t%c%d,%c%d,1\n", n, node->reg[nReg], l, LHS->reg[lReg]);
                
                if(LHS->dataType == FLOAT_TYPE){
                    freeTFloatReg(LHS->reg[lReg]);
                }
                else if(LHS->reg[lReg] == node->reg[nReg]){
                    freeTIntReg(LHS->reg[lReg]);
                }    
                if(RHS->dataType == FLOAT_TYPE){
                    freeTFloatReg(RHS->reg[rReg]);
                }
                else if(RHS->reg[rReg] == node->reg[nReg]){
                    freeTIntReg(RHS->reg[rReg]);
                }
                break;
            }    
        }

    }
}
        
void genVariableRValue(AST_NODE *node){
    printf("-----------------------------------Id is %s\n", getIdByNode(node));
    SymbolTableEntry *entry = node->semantic_value.identifierSemanticValue.symbolTableEntry;
    DATA_TYPE dataType = node->dataType;
    if(entry == NULL){
        printf("Entry is NULL\n");
    }
    printf("Variable datvaType is %d\n", dataType);
    if(dataType == FLOAT_TYPE){
        node->regType = T_REG;
        node->reg[T_REG] = allocateTFloatReg();
    }
    else{
        node->regType = T_REG;
        node->reg[T_REG] = allocateTIntReg();
    }

    IDENTIFIER_KIND kind = node->semantic_value.identifierSemanticValue.kind;
    printf("kind is %d\n", kind);
    if(kind == NORMAL_ID){
        printf("-------------------------------scope is %d\n", entry->scope);
        if(entry->scope > 0){
            //printf("Local Variable reference\n");
            //printf("data type is %d\n", dataType);
            //printf("Reg is %d\n", node->reg[T_REG]);
            if(dataType == INT_TYPE){
                fprintf(outputFile, "\tlw\tt%d,%d(s0)\n", node->reg[node->regType], entry->offset);
            }
            else if(dataType == FLOAT_TYPE){
                fprintf(outputFile, "\tflw\tft%d,%d(s0)\n", node->reg[node->regType], entry->offset);
            }
            else{
                printf("Error dataType %d\n", dataType);
                exit(0);
            }
        }
        else{
            //printf("Global variable reference\n");
            if(dataType == INT_TYPE){
                fprintf(outputFile, "\tlw\tt%d,_%s\n", node->reg[node->regType], getIdByNode(node));
            }
            else if(dataType == FLOAT_TYPE){
                int tmp = allocateTIntReg();
                fprintf(outputFile, "\tflw\tft%d,_%s,t%d\n", node->reg[node->regType], getIdByNode(node), tmp);
                freeTIntReg(tmp);
            }
        }
    }
    else if(kind == ARRAY_ID){
        genArrayRef(node);
        char n = (node->regType == T_REG)? 't' : 's';
        char d = (node->child->regType == T_REG)? 't' : 's';
        if(dataType == INT_TYPE){
            fprintf(outputFile, "\tlw\t%c%d,0(%c%d)\n", n, node->reg[node->regType], d, node->child->reg[node->regType]);
            //printf("\tlw\t%c%d,0(%c%d)\n", n, node->reg[node->regType], d, node->child->reg[node->regType]);
            freeIntReg(node->child);    
        }
        else if(dataType == FLOAT_TYPE){
            fprintf(outputFile, "\tflw\tf%c%d,0(%c%d)\n", n, node->reg[node->regType], d, node->child->reg[node->regType]);
            freeIntReg(node->child);    
        }

    }
}

void genAssignOrExpr(AST_NODE *node){
    printf("node type is %d\n", node->nodeType);
    if(node->nodeType == STMT_NODE){
        STMT_KIND kind = node->semantic_value.stmtSemanticValue.kind;
        if(kind == ASSIGN_STMT){
            genAssignmentStmt(node);
        }
        else if(kind == FUNCTION_CALL_STMT){
            genFunctionCall(node);
        }
    }
    else{
        genExprRelated(node);
    }
}

void genAssignmentStmt(AST_NODE *node){
    AST_NODE *LHS = node->child, *RHS = node->child->rightSibling;
    genExprRelated(RHS);
    printf("Assign LHS is %d, RHS is %d\n", LHS->dataType, RHS->dataType);
    if(LHS->dataType != RHS->dataType){
        if(LHS->dataType == INT_TYPE){
            genFloatToInt(RHS, T_REG); 
        }
        else if(LHS->dataType == FLOAT_TYPE){
            genIntToFloat(RHS, T_REG);
        }
    }
    char l, r;
    l = (LHS->regType == T_REG)? 't' : 's';
    r = (RHS->regType == T_REG)? 't' : 's';
    //printf("Assign LHS is %d, RHS is %d\n", LHS->dataType, RHS->dataType);
    IDENTIFIER_KIND LHSKind = LHS->semantic_value.identifierSemanticValue.kind;
    SymbolTableEntry *LHSEntry = LHS->semantic_value.identifierSemanticValue.symbolTableEntry;
    printf("id kind is %d\n", LHSKind);
    if(LHSKind == NORMAL_ID){
        if(LHSEntry->scope > 0){
            //printf("Scope is %d\n", LHSEntry->scope);
            if(LHS->dataType == INT_TYPE){
                //printf("In LHS\n");
                fprintf(outputFile, "\tsw\t%c%d,%d(s0)\n", r, RHS->reg[RHS->regType], LHSEntry->offset);
                freeTIntReg(RHS->reg[RHS->regType]);
            }
            else if(LHS->dataType == FLOAT_TYPE){
                fprintf(outputFile, "\tfsw\tf%c%d,%d(s0)\n", r, RHS->reg[RHS->regType], LHSEntry->offset);
                freeTFloatReg(RHS->reg[RHS->regType]);
            }
        }
        else if(LHSEntry->scope == 0){
            int tmp = allocateTIntReg();
            if(LHS->dataType == INT_TYPE){
                fprintf(outputFile, "\tsw\t%c%d,_%s,t%d\n", r, RHS->reg[RHS->regType], getIdByNode(LHS), tmp);
            }
            else if(LHS->dataType == FLOAT_TYPE){
                fprintf(outputFile, "\tfsw\tf%c%d,_%s,t%d\n", r, RHS->reg[RHS->regType], getIdByNode(LHS), tmp);
            }
            freeTIntReg(tmp);
        }
    }
    else if(LHSKind == ARRAY_ID){
        genArrayRef(LHS);
        char n = (RHS->regType == T_REG)? 't' : 's';
        char d = (LHS->child->regType == T_REG)? 't' : 's';
        if(LHS->dataType == INT_TYPE){
            fprintf(outputFile, "\tsw\t%c%d,0(%c%d)\n", n, RHS->reg[RHS->regType], d, LHS->child->reg[LHS->child->regType]);
            freeIntReg(RHS);    
            freeIntReg(LHS->child);    
        }
        else if(LHS->dataType == FLOAT_TYPE){
            fprintf(outputFile, "\tfsw\tf%c%d,0(%c%d)\n", n, RHS->reg[RHS->regType], d, LHS->child->reg[LHS->child->regType]);
            freeFloatReg(RHS);    
            freeIntReg(LHS->child);    
        }
    }

    if(RHS->dataType == INT_TYPE){
        freeIntReg(RHS);
    }
    else if(RHS->dataType == FLOAT_TYPE){
        freeFloatReg(RHS);
    }
    printf("After Assignment\n");
}

void genWhileStmt(AST_NODE *node){
    printf("------------------------Start generate While stmt.\n");
    AST_NODE *condition = node->child;
    AST_NODE *block = condition->rightSibling;
    
    int startLabel = labelCount;
    labelCount++;
    int endLabel = labelCount;
    labelCount++;

    fprintf(outputFile,"_LABEL_%d:\n", startLabel);
    printf("----before\n");
    genExprRelated(condition);
    printf("----after\n");
    if(condition->dataType == INT_TYPE){
        fprintf(outputFile, "\tbeqz\tt%d,_LABEL_%d\n", condition->reg[condition->regType], endLabel);
        freeTIntReg(condition->reg[condition->regType]);
    }
    /* Float Type problem not solved */
    else if(condition->dataType == FLOAT_TYPE){
        fprintf(outputFile, "\tbeqz\tft%d,_LABEL_%d\n", condition->reg[condition->regType], endLabel);
        freeTFloatReg(condition->reg[condition->regType]);
    }
    genStmt(block);
    fprintf(outputFile,"\tj\t_LABEL_%d\n", startLabel);
    fprintf(outputFile, "_LABEL_%d:\n", endLabel);
}

void genIfStmt(AST_NODE *node){
    AST_NODE *condition = node->child;
    AST_NODE *ifBlock = condition->rightSibling;
    AST_NODE *elseBlock = ifBlock->rightSibling;
    
    genExprRelated(condition);
    int endLabel, elseLabel;
    if(elseBlock == NULL){
        endLabel = labelCount;
        labelCount++;
        if(condition->dataType == INT_TYPE){
            fprintf(outputFile, "\tbeqz\tt%d,_LABEL_%d\n", condition->reg[condition->regType], endLabel);
            freeTIntReg(condition->reg[condition->regType]);
        }
        /* Float Type problem not solved */
        else if(condition->dataType == FLOAT_TYPE){
            fprintf(outputFile, "\tbeqz\tft%d,_LABEL_%d\n", condition->reg[condition->regType], endLabel);
            freeTFloatReg(condition->reg[condition->regType]);
        }
        genStmt(ifBlock);
        fprintf(outputFile, "_LABEL_%d:\n", endLabel);
        return;
    }
    //printf("else block is not NULL\n");   
    elseLabel = labelCount;
    labelCount++;
    endLabel = labelCount;
    labelCount++;
    if(condition->dataType == INT_TYPE){
        fprintf(outputFile, "\tbeqz\tt%d,_LABEL_%d\n", condition->reg[condition->regType], elseLabel);
        freeTIntReg(condition->reg[condition->regType]);    
    }
    /* Float Type problem not solved */
    else if(condition->dataType == FLOAT_TYPE){
        fprintf(outputFile, "\tbeqz\tft%d,_LABEL_%d\n", condition->reg[condition->regType], elseLabel);
        freeTFloatReg(condition->reg[condition->regType]);
    }
    ifWithReturn = 0;    
    genStmt(ifBlock);
    if(ifWithReturn == 0){
        fprintf(outputFile, "\tj\t_LABEL_%d\n", endLabel);
    }
    fprintf(outputFile, "_LABEL_%d:\n", elseLabel);
    ifWithReturn = 0;    
    genStmt(elseBlock);
     
    if(ifWithReturn == 0){
        fprintf(outputFile, "\tj\t_LABEL_%d\n", endLabel);
    }
    fprintf(outputFile, "_LABEL_%d:\n", endLabel);
    ifWithReturn = 0;    
}

void genReturnStmt(AST_NODE *node){
    ifWithReturn = 1;
    if(functionReturnType == VOID_TYPE){
        fprintf(outputFile, "\tj\t_%s_Epilogue\n", functionName);
        return;
    }
    AST_NODE *returnExpr = node->child;
    genExprRelated(returnExpr);
    if(functionReturnType != returnExpr->dataType){
        if(functionReturnType == INT_TYPE){
            genFloatToInt(returnExpr, returnExpr->regType);    
        }
        else if(functionReturnType == FLOAT_TYPE){
            genIntToFloat(returnExpr, returnExpr->regType);
        }
    }
    char r = (returnExpr->regType == T_REG)? 't' : 's';
    if(functionReturnType == INT_TYPE){
        fprintf(outputFile, "\tmv\ta0,%c%d\n", r, returnExpr->reg[returnExpr->regType]);
        freeIntReg(returnExpr);
    }
    else if(functionReturnType == FLOAT_TYPE){
        fprintf(outputFile, "\tfmv.s\tfa0,f%c%d\n", r, returnExpr->reg[returnExpr->regType]);
        freeFloatReg(returnExpr);
    }
    fprintf(outputFile, "\tj\t_%s_Epilogue\n", functionName);
}

void genArrayRef(AST_NODE *node){
    AST_NODE *dimList = node->child;

    if(dimList != NULL){
        int addr = allocateTIntReg();
        printf("arr ref addr is %d\n", addr);
        genExprRelated(dimList);
        char d = (dimList->regType == T_REG)? 't' : 's';
        printf("After\n"); 
        printf("--------------------------------dimList reg type is %c, number is %d\n", d, dimList->reg[dimList->regType]);
        int dim = 1;
        ArrayProperties property = node->semantic_value.identifierSemanticValue.symbolTableEntry->attribute->attr.typeDescriptor->properties.arrayProperties;
        for(AST_NODE *dimension = dimList->rightSibling; dimension != NULL; dimension = dimension->rightSibling){
            int sizeReg = allocateTIntReg();
            fprintf(outputFile, "\taddi\tt%d,zero,%d\n", sizeReg, property.sizeInEachDimension[dim - 1]);
            genExprRelated(dimension);
            char tmp = (dimension->regType == T_REG)? 't' : 's';
            /* offset = offset * dimSize[n - 1] + dimValue[n] */
            fprintf(outputFile, "\tmul\t%c%d,%c%d,t%d\n", d, dimList->reg[dimList->regType], d, dimList->reg[dimList->regType], sizeReg);
            fprintf(outputFile, "\tadd\t,%c%d,%c%d,%c%d\n", d, dimList->reg[dimList->regType], d, dimList->reg[dimList->regType], tmp, dimension->reg[dimension->regType]);
            freeTIntReg(sizeReg);
            freeIntReg(dimension);
            dim++;
        }
        
        int scope = node->semantic_value.identifierSemanticValue.symbolTableEntry->scope;
        if(scope > 0){
            fprintf(outputFile, "\taddi\tt%d,s0,%d\n", addr, node->semantic_value.identifierSemanticValue.symbolTableEntry->offset);
        }
        else if(scope == 0){
            fprintf(outputFile, "\tla\tt%d,_%s\n", addr, getIdByNode(node));
        }
        
        /* addr = base + offset * 8 */
        fprintf(outputFile, "\tslli\t%c%d,%c%d,3\n", d, dimList->reg[dimList->regType], d, dimList->reg[dimList->regType]);
        fprintf(outputFile, "\tadd\t%c%d,%c%d,t%d\n", d, dimList->reg[dimList->regType], d, dimList->reg[dimList->regType], addr);
        freeTIntReg(addr);
        //node->regType = dimList->regType;
        //node->reg[node->regType] = dimList->reg[dimList->regType];
        
    }
}
