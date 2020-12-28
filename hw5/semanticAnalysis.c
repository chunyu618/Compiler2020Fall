#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "symbolTable.h"
// This file is for reference only, you are not required to follow the implementation. //
// You only need to check for errors stated in the hw4 document. //
int g_anyErrorOccur = 0;
int g_suppressError = 0;
AST_NODE *g_currentFunctionDecl = NULL;

char *DATA_TYPE_string[] = {"int", "float", "void"};
char *NODE_TYPE_string[] = {
    "PROGRAM_NODE",
    "DECLARATION_NODE",
    "IDENTIFIER_NODE",
    "PARAM_LIST_NODE",
    "NUL_NODE",
    "BLOCK_NODE",
    "VARIABLE_DECL_LIST_NODE",
    "STMT_LIST_NODE",
    "STMT_NODE",
    "EXPR_NODE",
    "CONST_VALUE_NODE", //ex:1, 2, "constant string"
    "NONEMPTY_ASSIGN_EXPR_LIST_NODE",
    "NONEMPTY_RELOP_EXPR_LIST_NODE"
};

extern SymbolAttribute readAttr;
extern SymbolAttribute freadAttr;
extern SymbolAttribute writeAttr;

DATA_TYPE getBiggerType(DATA_TYPE dataType1, DATA_TYPE dataType2);
void processProgramNode(AST_NODE *programNode);
void processVariableDeclListNode(AST_NODE *declListNode);
void processDeclarationNode(AST_NODE* declarationNode);
void declareIdList(AST_NODE* typeNode, SymbolAttributeKind isVariableOrTypeAttribute, int ignoreArrayFirstDimSize);
void declareFunction(AST_NODE* returnTypeNode);
int processDeclDimList(AST_NODE* variableDeclDimList, TypeDescriptor* typeDescriptor, int ignoreFirstDimSize);
DATA_TYPE processTypeNode(AST_NODE* typeNode);
void processBlockNode(AST_NODE* blockNode);
void processStmtNode(AST_NODE* stmtNode);
void processGeneralNode(AST_NODE *node);
void checkAssignOrExpr(AST_NODE* assignOrExprRelatedNode);
void checkWhileStmt(AST_NODE* whileNode);
void checkForStmt(AST_NODE* forNode);
void checkAssignmentStmt(AST_NODE* assignmentNode);
void checkIfStmt(AST_NODE* ifNode);
void checkWriteFunction(AST_NODE* functionCallNode);
DATA_TYPE checkFunctionCall(AST_NODE* functionCallNode);
DATA_TYPE processExprRelatedNode(AST_NODE* exprRelatedNode);
void checkParameterPassing(Parameter* parameterList, AST_NODE* functionNameNode);
void checkReturnStmt(AST_NODE* returnNode);
DATA_TYPE processExprNode(AST_NODE* exprNode);
DATA_TYPE processVariableLValue(AST_NODE* idNode);
void processVariableRValue(AST_NODE* idNode);
DATA_TYPE processConstValueNode(AST_NODE* constValueNode);
DATA_TYPE getExprOrConstValue(AST_NODE* exprOrConstNode, int* iValue, float* fValue);
void evaluateExprValue(AST_NODE* exprNode, DATA_TYPE dataType);

static __inline__ char *getIdByNode(AST_NODE *node){
    if(node->nodeType == IDENTIFIER_NODE){
        return node->semantic_value.identifierSemanticValue.identifierName;
    }
    return NULL;
}

typedef enum ErrorMsgKind
{
    SYMBOL_IS_NOT_TYPE,
    SYMBOL_REDECLARE,
    SYMBOL_UNDECLARED,
    NOT_FUNCTION_NAME,
    TRY_TO_INIT_ARRAY,
    EXCESSIVE_ARRAY_DIM_DECLARATION,
    RETURN_ARRAY,
    VOID_VARIABLE,
    TYPEDEF_VOID_ARRAY,
    TYPE_REDECLARE,
    PARAMETER_TYPE_UNMATCH,
    TOO_FEW_ARGUMENTS,
    TOO_MANY_ARGUMENTS,
    RETURN_TYPE_UNMATCH,
    INCOMPATIBLE_ARRAY_DIMENSION,
    NOT_ASSIGNABLE,
    NOT_ARRAY,
    IS_TYPE_NOT_VARIABLE,
    IS_FUNCTION_NOT_VARIABLE,
    STRING_OPERATION,
    VOID_OPERATION,
    ARRAY_SIZE_NOT_INT,
    ARRAY_SIZE_NEGATIVE,
    ARRAY_SUBSCRIPT_NOT_INT,
    PASS_ARRAY_TO_SCALAR,
    PASS_SCALAR_TO_ARRAY
} ErrorMsgKind;

void printErrorMsgSpecial(AST_NODE* node, char *name2, ErrorMsgKind errorMsgKind)
{
    g_anyErrorOccur = 1;
    printf("Error found in line %d\n", node->linenumber);
    switch(errorMsgKind){
        case TYPE_REDECLARE:
            printf("typedef redefinition with different types (%s vs %s).\n", name2, DATA_TYPE_string[processTypeNode(node)]);
            break;
        case VOID_OPERATION:
            printf("passing 'void' to parameter of incompatible type '%s'.\n", name2);
            break;     
        default:
            break;
    } 
    
    
}
void printWarnMsg(AST_NODE *node, ErrorMsgKind warnMsgKind){
    
    printf("Warning found in line %d.\n", node->linenumber);
    switch(warnMsgKind){
        case RETURN_TYPE_UNMATCH:
            printf("implicit conversion turns floating-point number into integer: '%s' to '%s'\n", DATA_TYPE_string[1], DATA_TYPE_string[0]);
            break; 
        
    }
}

void printErrorMsg(AST_NODE* node, ErrorMsgKind errorMsgKind){
    SymbolTableEntry *entry;
    if(g_suppressError){
        return;
    }
    g_suppressError = 1;
    int dimension, dim;
    char tmp[128] = {};
    int index = 0;
    g_anyErrorOccur = 1;
    printf("Error found in line %d\n", node->linenumber);
    switch(errorMsgKind){
        case NOT_FUNCTION_NAME:
            entry = retrieveSymbol(getIdByNode(node));
            printf("called object type '%s' is not a function or function pointer.\n", DATA_TYPE_string[entry->attribute->attr.typeDescriptor->properties.dataType]);
            break;
        case SYMBOL_REDECLARE:
            printf("redefinition of '%s'.\n", getIdByNode(node));
            break;
        case SYMBOL_UNDECLARED:
            printf("use of undeclared idnetifier '%s'.\n", getIdByNode(node));
            break;
        case SYMBOL_IS_NOT_TYPE:
            printf("unknown type name '%s'\n", getIdByNode(node));
            break;
        case VOID_VARIABLE:
            printf("variable has incomplete type 'void'\n");
            break;
        case TOO_FEW_ARGUMENTS:
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        case TOO_MANY_ARGUMENTS:
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        case NOT_ARRAY:
            printf("subscripted value is not an array, pointer, or vector.\n");
            break;
        case NOT_ASSIGNABLE:         
            dimension = processDeclDimList(node->child, NULL, 0);
            index = 0;
            entry = retrieveSymbol(getIdByNode(node));
            for(int i = dimension; i < entry->attribute->attr.typeDescriptor->properties.arrayProperties.dimension; i++){
                dim = entry->attribute->attr.typeDescriptor->properties.arrayProperties.sizeInEachDimension[i];
                sprintf(&tmp[index], "[%d]", dim);
                index = strlen(tmp);
            }
            printf("array type '%s %s' is not assignable.\n", DATA_TYPE_string[entry->attribute->attr.typeDescriptor->properties.arrayProperties.elementType], tmp);
            break;
        case ARRAY_SIZE_NOT_INT:
            printf("array subscript is not an integer.\n");
            break;
        case ARRAY_SIZE_NEGATIVE:
            printf("'%s' declared as an array with a negative size.\n", getIdByNode(node));
            break;
        case PASS_ARRAY_TO_SCALAR:
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        case PASS_SCALAR_TO_ARRAY:
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        case STRING_OPERATION:
            printf("Constant string operation.\n");
            break;
        case VOID_OPERATION:
            printf("Void type operation.\n");
            break;
        case IS_FUNCTION_NOT_VARIABLE:
            printf("'%s' is a function name not a variable.\n", getIdByNode(node));
            break;
        case IS_TYPE_NOT_VARIABLE:
            printf("'%s' is a type name not a variable.\n", getIdByNode(node));
            break;
        case RETURN_TYPE_UNMATCH:
            printf("non-void function '%s' should return a value.\n", getIdByNode(g_currentFunctionDecl->child->rightSibling));
            break;
        default:
            printf("Unhandled case in void printErrorMsg(AST_NODE* node, ERROR_MSG_KIND* errorMsgKind)\n");
    }
    g_suppressError = 0;
}

SymbolAttribute *newAttribute(SymbolAttributeKind kind){
    SymbolAttribute *newAttr = (SymbolAttribute*)malloc(sizeof(SymbolAttribute));
    newAttr->attributeKind = kind;
    return newAttr;
}

TypeDescriptor *newTypeDesc(TypeDescriptorKind kind){
    TypeDescriptor *newDesc = (TypeDescriptor*)malloc(sizeof(TypeDescriptor));
    newDesc->kind = kind;
    return newDesc;
}

SymbolTableEntry *insertType(char *name, DATA_TYPE type){
    SymbolAttribute *attr = newAttribute(TYPE_ATTRIBUTE);
    TypeDescriptor *desc = newTypeDesc(SCALAR_TYPE_DESCRIPTOR);
    desc->properties.dataType = type;
    attr->attr.typeDescriptor = desc;
    return enterSymbol(name, attr);
}

void semanticAnalysis(AST_NODE *root)
{
    //printf("----------Print Over----------\n");
    int levelCount[32] = {};
    //printNode(root, 0, levelCount);
    insertType(SYMBOL_TABLE_INT_NAME, INT_TYPE);
    insertType(SYMBOL_TABLE_FLOAT_NAME, FLOAT_TYPE);
    insertType(SYMBOL_TABLE_VOID_NAME, VOID_TYPE);
    enterSymbol(SYMBOL_TABLE_SYS_LIB_READ, &readAttr);
    enterSymbol(SYMBOL_TABLE_SYS_LIB_FREAD, &freadAttr);
    enterSymbol(SYMBOL_TABLE_SYS_LIB_WRITE, &writeAttr);
    processProgramNode(root); 
    //printTable();
}


DATA_TYPE getBiggerType(DATA_TYPE dataType1, DATA_TYPE dataType2)
{
    if(dataType1 == FLOAT_TYPE || dataType2 == FLOAT_TYPE) {
        return FLOAT_TYPE;
    } else {
        return INT_TYPE;
    }
}


void processProgramNode(AST_NODE *programNode){
    for(AST_NODE *node = programNode->child; node != NULL; node = node->rightSibling){
        switch(node->nodeType){
            case VARIABLE_DECL_LIST_NODE:
                processVariableDeclListNode(node);
                break;
            case DECLARATION_NODE:
                processDeclarationNode(node);
                break;
            default:
                printError("Invalid program");
        }
    }
    printf("process over\n");
}

void processVariableDeclListNode(AST_NODE *declListNode){
    AST_NODE *declNode = declListNode->child;
    while(declNode != NULL){
        // process declaration.
        processDeclarationNode(declNode);
        declNode = declNode->rightSibling;
    }
}

/* Process all kinds of declaration and call corresponding declare function */
void processDeclarationNode(AST_NODE* declNode){
    if(declNode->nodeType != DECLARATION_NODE){
        char tmp[32] = {};
        sprintf(tmp, "Unexpected node type %d\n", declNode->nodeType);
        printError(tmp);
    }
    int declKind = declNode->semantic_value.declSemanticValue.kind;
    switch(declKind){
        case VARIABLE_DECL:
            // variable declaration
            declareIdList(declNode, VARIABLE_ATTRIBUTE, 0);
            break;
        case TYPE_DECL:
            // type declaration
            declareIdList(declNode, TYPE_ATTRIBUTE, 0);
            break;
        case FUNCTION_DECL:
            // function declaration
            declareFunction(declNode);
            break;
        default:
            printError("Invalid declaration type");
    }
}

/* Check if the type is declared */
DATA_TYPE processTypeNode(AST_NODE *typeNode){
    char *name = getIdByNode(typeNode);
    //printf("Type Name is %s\n", name);
    SymbolTableEntry* entry = retrieveSymbol(name);
    if(entry != NULL){
        if(entry->attribute->attributeKind != TYPE_ATTRIBUTE){
            return ERROR_TYPE;
        }
        else{
            typeNode->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
            return entry->attribute->attr.typeDescriptor->properties.dataType;
        }
    }
    return ERROR_TYPE;
}

/* Declare identifier list of variable or type */
void declareIdList(AST_NODE* declarationNode, SymbolAttributeKind isVariableOrTypeAttribute, int ignoreArrayFirstDimSize){
    //printf("Id List\n");
    if(isVariableOrTypeAttribute == VARIABLE_ATTRIBUTE){
        AST_NODE* typeNode = declarationNode->child;
        //if(typeNode == NULL) printf("lalala");
        DATA_TYPE dataType = processTypeNode(typeNode);
        //printf("data type is %s\n", DATA_TYPE_string[dataType]);
        if(dataType == ERROR_TYPE){
            printErrorMsg(typeNode, SYMBOL_IS_NOT_TYPE);
            //printf("[DEBUG] data type is error type.\n");
            return;
        } 
        else if(dataType == VOID_TYPE){
            // This may not happen
            printErrorMsg(typeNode, VOID_VARIABLE);
            //printf("[DEBUG] data type is void type.\n");
            return;
        }
        for(AST_NODE *id = typeNode->rightSibling; id != NULL; id = id->rightSibling){
            int count[32] = {};
            //printNode(id, 0, count);    
            AST_NODE *exprNode;
            TypeDescriptor *typeDesc;
            SymbolAttribute *symbolAttr = newAttribute(VARIABLE_ATTRIBUTE);
            DATA_TYPE exprType;
            int tmp;
            SymbolTableEntry *entry;
            switch(id->semantic_value.identifierSemanticValue.kind){
                case NORMAL_ID:
                    // Prepare symbol attribute
                    typeDesc = newTypeDesc(SCALAR_TYPE_DESCRIPTOR);
                    typeDesc->properties.dataType = dataType;
                    symbolAttr->attr.typeDescriptor = typeDesc;
                    
                    // Enter into symbol table
                    entry = enterSymbol(getIdByNode(id), symbolAttr); 
                    if(entry == NULL){
                        printErrorMsg(id, SYMBOL_REDECLARE);
                    }
                    else{
                        id->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
                    }
                    break;
                case WITH_INIT_ID:
                    // Check expression type
                    exprNode = id->child;
                    exprType = processExprRelatedNode(exprNode);
                    if(exprType == CONST_STRING_TYPE){
                        printErrorMsg(exprNode, STRING_OPERATION);
                        //printf("[DEBUG] expression type is const string.\n");
                        break;
                    }
                    else if (exprType == VOID_TYPE){
                        printErrorMsg(exprNode, VOID_OPERATION);
                        //printf("[DEBUG] expression type is void.\n");
                        break;
                    }
                    else if(exprType == ERROR_TYPE){
                        
                        //printf("[DEBUG] expression type is error type.\n");
                        break;
                    }
                    
                    // Prepare symbol attribute
                    typeDesc = newTypeDesc(SCALAR_TYPE_DESCRIPTOR);
                    typeDesc->properties.dataType = dataType;
                    symbolAttr->attr.typeDescriptor = typeDesc;
                    
                    // Enter into symbol table
                    entry = enterSymbol(getIdByNode(id), symbolAttr); 
                    if(entry == NULL){
                        printErrorMsg(id, SYMBOL_REDECLARE);
                    }
                    else{
                        id->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
                    }
                    break;
                case ARRAY_ID:
                    // Prepare symbol attribute
                    typeDesc = newTypeDesc(ARRAY_TYPE_DESCRIPTOR);
                    typeDesc->properties.arrayProperties.elementType = dataType;
                    processDeclDimList(id->child, typeDesc, 0);
                    symbolAttr->attr.typeDescriptor = typeDesc;
                    /*
                    printf("Variable %s dim %d\n", getIdByNode(id), typeDesc->properties.arrayProperties.dimension);
                    for(int i = 0; i < typeDesc->properties.arrayProperties.dimension; i++){
                        printf("%d\n", i);
                        printf("dim %d size %d\n", i, typeDesc->properties.arrayProperties.sizeInEachDimension[i]);
                    }
                    */
                    // Enter into symbol table
                    entry = enterSymbol(getIdByNode(id), symbolAttr); 
                    if(entry == NULL){
                        printErrorMsg(id, SYMBOL_REDECLARE);
                    }                  
                    else{
                        id->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
                    }
                    break;
                default:
                    printf("[DEBUG] unexpected kind of variable.\n");
                    break;
            }
        }
    }
    else if(isVariableOrTypeAttribute == TYPE_ATTRIBUTE){
        AST_NODE *typeNode = declarationNode->child;
        DATA_TYPE dataType = processTypeNode(typeNode);
        //printf("dataType is %s\n", DATA_TYPE_string[dataType]);
        if(dataType == ERROR_TYPE){
            printErrorMsg(typeNode, SYMBOL_IS_NOT_TYPE);
            //printf("[DEBUG] Symbol is not a type.\n");
        }
        for(AST_NODE *id = typeNode->rightSibling; id != NULL; id = id->rightSibling){
            SymbolTableEntry *entry = insertType(getIdByNode(id), dataType);
            if(entry == NULL){
                DATA_TYPE oldDataType = retrieveSymbol(getIdByNode(id))->attribute->attr.typeDescriptor->properties.dataType;
                //printf("%d %d\n", dataType, oldDataType);
                if(dataType == oldDataType){
                    printErrorMsg(id, SYMBOL_REDECLARE);
                }
                else{
                    printErrorMsgSpecial(id, DATA_TYPE_string[dataType], TYPE_REDECLARE);
                }
            }
            else{
                id->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
            }
        }
    }
}

void checkAssignOrExpr(AST_NODE* assignOrExprRelatedNode){
    //printf("id name is %s\n", getIdByNode(assignOrExprRelatedNode));
    //printf("In check Assign Or Expr node type is %s\n", NODE_TYPE_string[assignOrExprRelatedNode->nodeType]);
    switch(assignOrExprRelatedNode->nodeType){
        case IDENTIFIER_NODE:
            processVariableLValue(assignOrExprRelatedNode);
            break;
        case STMT_NODE:
            processStmtNode(assignOrExprRelatedNode);
            break;
        case EXPR_NODE:
            processExprRelatedNode(assignOrExprRelatedNode);
            break;
        default:
            printf("[DEBUG] Invalid node in for loop.\n");
            break;
    }
    return;
}

void checkWhileStmt(AST_NODE* whileNode){
    processExprRelatedNode(whileNode->child);
    processStmtNode(whileNode->child->rightSibling);
}


void checkForStmt(AST_NODE* forNode){
    AST_NODE *current = forNode->child;
    if(current->nodeType == NONEMPTY_ASSIGN_EXPR_LIST_NODE){
        for(AST_NODE *part = current->child; part != NULL; part = part->rightSibling){
            checkAssignOrExpr(part);
        }
    }
    current = current->rightSibling;
    if(current->nodeType == NONEMPTY_RELOP_EXPR_LIST_NODE){
        for(AST_NODE *part = current->child; part != NULL; part = part->rightSibling){
            checkAssignOrExpr(part);
        }
    }
    current = current->rightSibling;
    if(current->nodeType == NONEMPTY_ASSIGN_EXPR_LIST_NODE){
        for(AST_NODE *part = current->child; part != NULL; part = part->rightSibling){
            checkAssignOrExpr(part);
        }
    }
    current = current->rightSibling;
    processStmtNode(current);
}


void checkAssignmentStmt(AST_NODE* assignmentNode){
    
}


void checkIfStmt(AST_NODE* ifNode){
    AST_NODE *current = ifNode->child;
    processExprRelatedNode(current);
    current = current->rightSibling;
    processStmtNode(current);
    current = current->rightSibling;
    /* else */
    if(current->nodeType != NUL_NODE){
        processStmtNode(current);
    }
}

void checkWriteFunction(AST_NODE* functionCallNode){
    //printf("Function name is %s\n", getIdByNode(functionCallNode));
    if(functionCallNode->rightSibling->nodeType == NUL_NODE){
        printErrorMsg(functionCallNode, TOO_FEW_ARGUMENTS);
        return;
    }
    AST_NODE *argument = functionCallNode->rightSibling->child;
    if(argument->rightSibling != NULL){
        printErrorMsg(functionCallNode, TOO_MANY_ARGUMENTS);
        return;
    }
    DATA_TYPE argumentType = processExprRelatedNode(argument);
    //printf("argument type is %d\n", argumentType);
    functionCallNode->rightSibling->child->dataType = argumentType; 
    //printf("Address is %d\n", functionCallNode->rightSibling->child->dataType);
    if(argumentType == VOID_TYPE){
        //printErrorMsg(functionCallNode, );
        //printf("[DEBUG] Void operation.\n");
    }
}

/* Check if the function call is legal */
DATA_TYPE checkFunctionCall(AST_NODE* functionCallNode){
    AST_NODE *functionNameNode = functionCallNode->child;
    //printf("Function name is %s\n", getIdByNode(functionNameNode));
    SymbolTableEntry *entry = retrieveSymbol(getIdByNode(functionNameNode));
    if(entry == NULL){
        printErrorMsg(functionNameNode, SYMBOL_UNDECLARED);
        return ERROR_TYPE;
    }
    else{
        functionNameNode->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
    }

    /* To check whether ID is a function name*/
    if(entry->attribute->attributeKind != FUNCTION_SIGNATURE){
        printErrorMsg(functionNameNode, NOT_FUNCTION_NAME);
        return ERROR_TYPE;
    }
   
    /* Check parameters*/
    FunctionSignature *funcSign = entry->attribute->attr.functionSignature;
    if(strncmp(getIdByNode(functionNameNode), "write", 6) == 0){
        checkWriteFunction(functionNameNode);
        return VOID_TYPE;
    }
    /* Give parameters while function doesn't need any parameter */
    if(funcSign->parametersCount == 0){
        if(functionNameNode->rightSibling->nodeType != NUL_NODE){
            printErrorMsg(functionNameNode, TOO_MANY_ARGUMENTS);
            return ERROR_TYPE;
        }
    }
    else{
        checkParameterPassing(funcSign->parameterList, functionNameNode);
    }
    return funcSign->returnType;
}

void checkParameterPassing(Parameter* parameterList, AST_NODE* functionNameNode){
    /* Get the first function parameter declaration node */
    AST_NODE *argument = functionNameNode->rightSibling->child;
    while(parameterList != NULL){
        if(argument == NULL){
            printErrorMsg(functionNameNode, TOO_FEW_ARGUMENTS);
            break;
        }
        switch(argument->nodeType){
            case CONST_VALUE_NODE:
            case IDENTIFIER_NODE:
            case EXPR_NODE:
            case STMT_NODE:
                if(parameterList->type->kind == SCALAR_TYPE_DESCRIPTOR){
                    if(argument->nodeType == IDENTIFIER_NODE){
                        SymbolTableEntry *entry = retrieveSymbol(getIdByNode(argument));
                        if(entry == NULL){
                            printErrorMsg(argument, SYMBOL_UNDECLARED);
                            argument = argument->rightSibling;
                            break;
                        }
                        else if(entry->attribute->attributeKind == VARIABLE_ATTRIBUTE){
                            argument->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
                            if(entry->attribute->attr.typeDescriptor->kind == ARRAY_TYPE_DESCRIPTOR){
                                int argumentDim = entry->attribute->attr.typeDescriptor->properties.arrayProperties.dimension;
                                int referenceDim = 0;
                                if(argument->child != NULL){
                                    referenceDim = processDeclDimList(argument->child, NULL, 0);
                                }
                                if(argumentDim > referenceDim){
                                    printErrorMsg(functionNameNode, PASS_ARRAY_TO_SCALAR);
                                    //printf("[DEBUG] array passed to scalar parameter.\n");
                                    g_suppressError = 1;
                                }
                                else if(argumentDim < referenceDim){
                                    printErrorMsg(argument, NOT_ARRAY);
                                    //printf("[DEBUG] incompatable array dimension.\n");
                                    g_suppressError = 1;
                                }
                            }    
                        }
                    }
                    DATA_TYPE argumentType = processExprRelatedNode(argument);
                    if(argumentType == VOID_TYPE){
                        printErrorMsgSpecial(argument, DATA_TYPE_string[parameterList->type->properties.dataType], VOID_OPERATION);
                        //printf("[DEBUG] Unexpected void type.\n");
                    }
                    else if(argumentType == CONST_STRING_TYPE){
                        printErrorMsg(argument, STRING_OPERATION);
                        //printf("[DEBUG] Unexpected const string type.\n");
                    }
                }
                else if(parameterList->type->kind == ARRAY_TYPE_DESCRIPTOR){
                    //printf("augument node type is %s\n", NODE_TYPE_string[argument->nodeType]);
                    if(argument->nodeType == IDENTIFIER_NODE){
                        SymbolTableEntry *entry = retrieveSymbol(getIdByNode(argument));
                        if(entry == NULL){
                            printErrorMsg(argument, SYMBOL_UNDECLARED);
                            argument = argument->rightSibling;
                            break;
                        }
                        else if(entry->attribute->attributeKind == VARIABLE_ATTRIBUTE){
                            argument->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
                            if(entry->attribute->attr.typeDescriptor->kind == ARRAY_TYPE_DESCRIPTOR){
                                int argumentDim = entry->attribute->attr.typeDescriptor->properties.arrayProperties.dimension;
                                int parameterDim = parameterList->type->properties.arrayProperties.dimension;
                                int referenceDim = 0;
                                if(argument->child != NULL){
                                    referenceDim = processDeclDimList(argument->child, NULL, 0);
                                }
                                //printf("argumentDim is %d, referenceDim is %d\n", argumentDim, referenceDim);
                                
                                if(argumentDim == referenceDim){
                                    printErrorMsg(functionNameNode, PASS_SCALAR_TO_ARRAY);
                                    //printf("[DEBUG] Array passed to scalar parameter.\n");
                                }
                                
                                if(argumentDim < referenceDim){
                                    printErrorMsg(argument, NOT_ARRAY);
                                }
                                else if(argumentDim < referenceDim || argumentDim - referenceDim != parameterDim){
                                    printErrorMsg(argument, NOT_ASSIGNABLE);
                                    //printf("[DEBUG] Incompatable array dimension.\n");
                                }
                                g_suppressError = 1;
                            }
                        }
                    }
                    DATA_TYPE argumentType = processExprRelatedNode(argument);
                    if(argumentType == VOID_TYPE){
                        //printf("[DEBUG] Unexpected void type \n");
                    }
                    else if(argumentType == CONST_STRING_TYPE){
                        printErrorMsg(argument, STRING_OPERATION);
                        //printf("[DEBUG] Unexpected const string type \n");
                    }
                    else if(argumentType != ERROR_TYPE){
                        printErrorMsg(argument, PASS_SCALAR_TO_ARRAY);
                        //printf("[DEBUG] Array passed to scalar parameter.\n");
                    }
                }
                argument = argument->rightSibling;
                break;
            default:
                printf("[DEBUG] Broken AST.\n");
                exit(1);
        }
        g_suppressError = 0;
        parameterList = parameterList->next;
    }
    /* Get one more parameter out of expected */
    if(argument != NULL){
        printErrorMsg(functionNameNode, TOO_MANY_ARGUMENTS);
    }
}

/* Process EXPR_NODE, CONST_VALUE_NODE, STMT_NODE, IDENTIFIER_NODE, NUL_NODE*/
DATA_TYPE processExprRelatedNode(AST_NODE* exprRelatedNode){
    AST_NODE *leftValueNode;
    //printf("node type is %s\n", NODE_TYPE_string[exprRelatedNode->nodeType]);
    switch(exprRelatedNode->nodeType){
        case EXPR_NODE:
            return (exprRelatedNode->dataType = processExprNode(exprRelatedNode));
        case CONST_VALUE_NODE:
            return (exprRelatedNode->dataType = processConstValueNode(exprRelatedNode));
        case STMT_NODE:
            switch(exprRelatedNode->semantic_value.stmtSemanticValue.kind){
                case FUNCTION_CALL_STMT:
                    return (exprRelatedNode->dataType = checkFunctionCall(exprRelatedNode));
                case ASSIGN_STMT:
                    leftValueNode = exprRelatedNode->child;
                    DATA_TYPE leftDataType = processExprRelatedNode(leftValueNode);
                    if(leftDataType == ERROR_TYPE || leftDataType == VOID_TYPE){
                        return ERROR_TYPE;
                    }
                    DATA_TYPE rightDataType = processExprRelatedNode(leftValueNode->rightSibling);
                    if(rightDataType != leftDataType) {
                        if(rightDataType == CONST_STRING_TYPE){
                            printErrorMsg(exprRelatedNode, STRING_OPERATION);
                            //printf("[DEBUG] cannot be assigned as a string.\n");
                            return ERROR_TYPE;
                        }
                        else if(rightDataType == VOID_TYPE){
                            printErrorMsg(exprRelatedNode, VOID_OPERATION);
                            //printf("[DEBUG] cannot be assigned as a void type.\n");
                            return ERROR_TYPE;
                        }
                        else if(rightDataType == ERROR_TYPE){
                            return ERROR_TYPE;
                        }
                    }

                    return (exprRelatedNode->dataType = leftDataType);
                default:
                    printf("[DEBUG] Broken AST.\n");
                    exit(1);
            }
        case IDENTIFIER_NODE:
            return (exprRelatedNode->dataType = processVariableLValue(exprRelatedNode));
        default:
            printf("[DEBUG] Unexpected ExprRelated Node.\n");
            return ERROR_TYPE;
    }
    return ERROR_TYPE;
}

DATA_TYPE getExprOrConstValue(AST_NODE* exprOrConstNode, int* iValue, float* fValue){
    if(exprOrConstNode->nodeType == CONST_VALUE_NODE){
        switch(exprOrConstNode->semantic_value.const1->const_type){
            case INTEGERC:
                *iValue = exprOrConstNode->semantic_value.const1->const_u.intval;
                exprOrConstNode->dataType = INT_TYPE;
                return INT_TYPE;
            case FLOATC:
                exprOrConstNode->dataType = FLOAT_TYPE;
                *fValue = exprOrConstNode->semantic_value.const1->const_u.fval;
                return FLOAT_TYPE;
            default:
                return ERROR_TYPE;
        }
    }
    else if(exprOrConstNode->nodeType == EXPR_NODE && exprOrConstNode->semantic_value.exprSemanticValue.isConstEval == 1){
        switch(exprOrConstNode->dataType){
            case INTEGERC:
                *iValue = exprOrConstNode->semantic_value.exprSemanticValue.constEvalValue.iValue;
                exprOrConstNode->dataType = INT_TYPE;
                return INT_TYPE;
            case FLOATC:
                *fValue = exprOrConstNode->semantic_value.exprSemanticValue.constEvalValue.fValue;
                exprOrConstNode->dataType = FLOAT_TYPE;
                return FLOAT_TYPE;
            default:
                return ERROR_TYPE;
        }
    }
    else{
        return ERROR_TYPE;
    }
}

void evaluateExprValue(AST_NODE* exprNode, DATA_TYPE dataType){
    if(exprNode->semantic_value.exprSemanticValue.kind == BINARY_OPERATION){
        AST_NODE *leftExpr = exprNode->child;
        AST_NODE *rightExpr = leftExpr->rightSibling;

        int leftInt, rightInt;
        float leftFloat, rightFloat;

        DATA_TYPE leftDataType = getExprOrConstValue(leftExpr, &leftInt, &leftFloat);
        DATA_TYPE rightDataType = getExprOrConstValue(rightExpr, &rightInt, &rightFloat);
        if(dataType != INT_TYPE && dataType != FLOAT_TYPE){
            return;
        }
        if(leftDataType == ERROR_TYPE || rightDataType == ERROR_TYPE || leftDataType == VOID_TYPE || rightDataType == VOID_TYPE){
            return;
        }
        if(leftDataType == INT_TYPE && rightDataType == INT_TYPE){
            int val;
            switch(exprNode->semantic_value.exprSemanticValue.op.binaryOp){
                case BINARY_OP_ADD:
                    val = leftInt + rightInt;
                    break;
                case BINARY_OP_SUB:
                    val = leftInt - rightInt;
                    break;
                case BINARY_OP_MUL:
                    val = leftInt * rightInt;
                    break;
                case BINARY_OP_DIV:
                    val = leftInt / rightInt;
                    break;
                case BINARY_OP_EQ:
                    val = (leftInt == rightInt);
                    break;
                case BINARY_OP_GE:
                    val = (leftInt >= rightInt);
                    break;
                case BINARY_OP_LE:
                    val = (leftInt <= rightInt);
                    break;
                case BINARY_OP_NE:
                    val = (leftInt != rightInt);
                    break;
                case BINARY_OP_GT:
                    val = (leftInt > rightInt);
                    break;
                case BINARY_OP_LT:
                    val = (leftInt < rightInt);
                    break;
                case BINARY_OP_AND:
                    val = (leftInt && rightInt);
                    break;
                case BINARY_OP_OR:
                    val = (leftInt || rightInt);
                    break;
            }
            exprNode->semantic_value.exprSemanticValue.isConstEval = 1;
            if(dataType == INT_TYPE){
                exprNode->semantic_value.exprSemanticValue.constEvalValue.iValue = val;
            }
            else if(dataType == FLOAT_TYPE){
                exprNode->semantic_value.exprSemanticValue.constEvalValue.fValue = (float)val;
            }
        }
        else if(dataType == FLOAT_TYPE || rightDataType == FLOAT_TYPE){
            if(leftDataType == INT_TYPE){
                leftFloat = (float)leftInt;
            }
            if(rightDataType == INT_TYPE){
                rightFloat = (float)rightInt;
            }
            float fval = 0;
            int ival = 0;
            switch(exprNode->semantic_value.exprSemanticValue.op.binaryOp){
                case BINARY_OP_ADD:
                    fval = leftFloat + rightFloat;
                    break;
                case BINARY_OP_SUB:
                    fval = leftFloat - rightFloat;
                    break;
                case BINARY_OP_MUL:
                    fval = leftFloat * rightFloat;
                    break;
                case BINARY_OP_DIV:
                    fval = leftFloat / rightFloat;
                    break;
                case BINARY_OP_EQ:
                    ival = (leftFloat == rightFloat);
                    break;
                case BINARY_OP_GE:
                    ival = (leftFloat >= rightFloat);
                    break;
                case BINARY_OP_LE:
                    ival = (leftFloat <= rightFloat);
                    break;
                case BINARY_OP_NE:
                    ival = (leftFloat != rightFloat);
                    break;
                case BINARY_OP_GT:
                    ival = (leftFloat > rightFloat);
                    break;
                case BINARY_OP_LT:
                    ival = (leftFloat < rightFloat);
                    break;
                case BINARY_OP_AND:
                    ival = (leftFloat && rightFloat);
                    break;
                case BINARY_OP_OR:
                    ival = (leftFloat || rightFloat);
                    break;  
            }
            exprNode->semantic_value.exprSemanticValue.isConstEval = 1;
            switch(exprNode->semantic_value.exprSemanticValue.op.binaryOp){
                case BINARY_OP_ADD:
                case BINARY_OP_SUB:
                case BINARY_OP_MUL:
                case BINARY_OP_DIV:
                    if(dataType == INT_TYPE){
                        exprNode->semantic_value.exprSemanticValue.constEvalValue.iValue = (int)fval;

                    }
                    else if(dataType == FLOAT_TYPE){
                        exprNode->semantic_value.exprSemanticValue.constEvalValue.fValue = fval;
                    }
                    break;
                case BINARY_OP_EQ:    
                case BINARY_OP_GE:    
                case BINARY_OP_LE:    
                case BINARY_OP_NE:    
                case BINARY_OP_GT:    
                case BINARY_OP_LT:    
                case BINARY_OP_AND:    
                case BINARY_OP_OR:
                    if(dataType == INT_TYPE){
                        exprNode->semantic_value.exprSemanticValue.constEvalValue.iValue = ival;
                    }
                    else if(dataType == FLOAT_TYPE){
                        exprNode->semantic_value.exprSemanticValue.constEvalValue.fValue = (float)ival;
                    }
                    break;
            }
        }
    }
    else if(exprNode->semantic_value.exprSemanticValue.kind == UNARY_OPERATION){
        AST_NODE *leftExpr = exprNode->child;
        int leftInt;
        float leftFloat;
        DATA_TYPE leftDataType = getExprOrConstValue(leftExpr, &leftInt, &leftFloat);
        if(leftDataType == ERROR_TYPE){
            return;
        }
        if(leftDataType == INT_TYPE){
            int val;
            switch(exprNode->semantic_value.exprSemanticValue.op.unaryOp){
                case UNARY_OP_POSITIVE:
                    val = leftInt;
                    break;
                case UNARY_OP_NEGATIVE:
                    val = -leftInt;
                    break;
                case UNARY_OP_LOGICAL_NEGATION:
                    val = !(leftInt);
                    break;
            }
            exprNode->semantic_value.exprSemanticValue.isConstEval = 1;
            if(dataType == INT_TYPE){
                exprNode->semantic_value.exprSemanticValue.constEvalValue.iValue = val;
            }
            else if(dataType == FLOAT_TYPE){
                exprNode->semantic_value.exprSemanticValue.constEvalValue.fValue = (float)val;
            }
        }
        else if(leftDataType == FLOAT_TYPE){
            int ival = 0;
            float fval = .0;
            switch(exprNode->semantic_value.exprSemanticValue.op.unaryOp){
                case UNARY_OP_POSITIVE:
                    fval = leftFloat;
                    break;
                case UNARY_OP_NEGATIVE:
                    fval = -leftFloat;
                    break;
                case UNARY_OP_LOGICAL_NEGATION:
                    ival = !(leftFloat);
                    break;
            }
            switch(exprNode->semantic_value.exprSemanticValue.op.unaryOp){
                case UNARY_OP_POSITIVE:
                case UNARY_OP_NEGATIVE:
                    if(dataType == INT_TYPE){
                        exprNode->semantic_value.exprSemanticValue.constEvalValue.iValue = (int)fval;
                    }
                    else if(dataType == FLOAT_TYPE){
                        exprNode->semantic_value.exprSemanticValue.constEvalValue.fValue = fval;
                    }
                    break;
                case UNARY_OP_LOGICAL_NEGATION:
                    if(dataType == INT_TYPE){
                        exprNode->semantic_value.exprSemanticValue.constEvalValue.iValue = ival;
                    }
                    else if(dataType == FLOAT_TYPE){
                        exprNode->semantic_value.exprSemanticValue.constEvalValue.fValue = (float)ival;
                    }
                    break;
            }
            exprNode->semantic_value.exprSemanticValue.isConstEval = 1;
        }
    }
}

/* Process expression */
DATA_TYPE processExprNode(AST_NODE* exprNode){
    if(exprNode->nodeType != EXPR_NODE){
        printf("[DEBUG] wrong node type in processExprNode.\n");
        exit(1);
    }

    DATA_TYPE dataType, leftDataType, rightDataType;
    switch(exprNode->semantic_value.exprSemanticValue.kind){
        case BINARY_OPERATION:
            leftDataType = processExprRelatedNode(exprNode->child);
            rightDataType = processExprRelatedNode(exprNode->child->rightSibling);
            //printf("left is %d, right is %d\n", leftDataType, rightDataType);
            if(leftDataType == VOID_TYPE || rightDataType == VOID_TYPE){
                printErrorMsg(exprNode, VOID_OPERATION);
                //printf("[DEBUG] void operation.\n");
                return ERROR_TYPE;
            }
            if(leftDataType == CONST_STRING_TYPE || rightDataType == CONST_STRING_TYPE){
                printErrorMsg(exprNode, STRING_OPERATION);
                //printf("[DEBUG] constant string operation.\n");
                return ERROR_TYPE;
            }
            if(leftDataType == ERROR_TYPE || rightDataType == ERROR_TYPE){
                return ERROR_TYPE;
            }

            switch(exprNode->semantic_value.exprSemanticValue.op.binaryOp){
                /* expor */
                case BINARY_OP_ADD: 
                case BINARY_OP_SUB: 
                case BINARY_OP_MUL: 
                case BINARY_OP_DIV:
                    dataType = getBiggerType(leftDataType, rightDataType);
                    evaluateExprValue(exprNode, dataType);
                    break;

                /* relop */
                case BINARY_OP_OR:
                case BINARY_OP_AND:
                case BINARY_OP_EQ:
                case BINARY_OP_GE:
                case BINARY_OP_LE:
                case BINARY_OP_NE:
                case BINARY_OP_GT:
                case BINARY_OP_LT:
                    dataType = INT_TYPE;
                    evaluateExprValue(exprNode, dataType);        
                    break;
            }
            break;
        case UNARY_OPERATION:
            dataType = processExprRelatedNode(exprNode->child);
            if(dataType != INT_TYPE && dataType != FLOAT_TYPE){
                dataType = ERROR_TYPE;
                break;
            }
            if(exprNode->semantic_value.exprSemanticValue.op.unaryOp == UNARY_OP_LOGICAL_NEGATION){
                dataType = INT_TYPE;
            }
            evaluateExprValue(exprNode, dataType);
            break;
        default:
            printf("[DEBUG] Invalid operation.\n");
            exit(1);
    }
    //printf("Node name is %s and type is %d\n", getIdByNode(exprNode), dataType);
    exprNode->dataType = dataType;
    return dataType;
}


DATA_TYPE processVariableLValue(AST_NODE* idNode){
    //printf("process variable L value %s\n", getIdByNode(idNode));
    SymbolTableEntry *entry = retrieveSymbol(getIdByNode(idNode));
    if(entry == NULL){
        printErrorMsg(idNode, SYMBOL_UNDECLARED);
        return ERROR_TYPE;
    }
    else{
        idNode->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
    }

    SymbolAttribute *attribute = entry->attribute;
    if(attribute->attributeKind != VARIABLE_ATTRIBUTE){
        if(attribute->attributeKind == FUNCTION_SIGNATURE){
            //printf("----\n");
            //printf("[DEBUG] Function is not a variable.\n");
            printErrorMsg(idNode, IS_FUNCTION_NOT_VARIABLE);
        }
        else if(attribute->attributeKind == TYPE_ATTRIBUTE){
            //printf("[DEBUG] Type is not a variable.\n");
            printErrorMsg(idNode, IS_TYPE_NOT_VARIABLE);
        }
        return ERROR_TYPE;
    }

    TypeDescriptor *descriptor = attribute->attr.typeDescriptor;
    //printf("Descriptor->kind is %d\n", descriptor->kind);
    switch(descriptor->kind){
        case SCALAR_TYPE_DESCRIPTOR:
            if(idNode->semantic_value.identifierSemanticValue.kind == ARRAY_ID){
                printErrorMsg(idNode, NOT_ARRAY);
                return ERROR_TYPE;
            }
            idNode->dataType = descriptor->properties.dataType;
            return descriptor->properties.dataType;
        case ARRAY_TYPE_DESCRIPTOR:
            if(idNode->semantic_value.identifierSemanticValue.kind != ARRAY_ID){
                printErrorMsg(idNode, NOT_ASSIGNABLE);
                return ERROR_TYPE;
            }
            int referenceDim = processDeclDimList(idNode->child, NULL, 0);
            //printf("referenceDim is %d\n", referenceDim);
            if(referenceDim > descriptor->properties.arrayProperties.dimension){
                printErrorMsg(idNode, NOT_ARRAY);
                return ERROR_TYPE;
            }
            else if(referenceDim < descriptor->properties.arrayProperties.dimension){
                printErrorMsg(idNode, NOT_ASSIGNABLE);
                return ERROR_TYPE;
            }
            return descriptor->properties.arrayProperties.elementType;
        default:
            return ERROR_TYPE;
    }
}


void processVariableRValue(AST_NODE* idNode)
{
}

/* Process constant value included int, float, string */
DATA_TYPE processConstValueNode(AST_NODE* constValueNode){
    switch(constValueNode->semantic_value.const1->const_type){
        case INTEGERC:
            return INT_TYPE;
        case FLOATC:
            return FLOAT_TYPE;
        case STRINGC:
            return CONST_STRING_TYPE;
        default:
            return ERROR_TYPE;
    }
}


void checkReturnStmt(AST_NODE* returnNode){
    AST_NODE *functionNameNode = g_currentFunctionDecl->child->rightSibling;
    FunctionSignature *signature = retrieveSymbol(functionNameNode->semantic_value.identifierSemanticValue.identifierName)->attribute->attr.functionSignature;
    DATA_TYPE returnType;
    switch(returnNode->child->nodeType){
        case NUL_NODE:
            if(signature->returnType != VOID_TYPE){
                printErrorMsg(returnNode, RETURN_TYPE_UNMATCH);
                //printf("[DEBUG] Unmatch return type.\n");
            }
            break;
        case IDENTIFIER_NODE:
        case CONST_VALUE_NODE:
        case EXPR_NODE: 
            returnType = processExprRelatedNode(returnNode->child);
            //printf("return type is %d, signature is %d\n", returnType, signature->returnType);
            if(returnType != signature->returnType){
                if(returnType == FLOAT_TYPE && signature->returnType == INT_TYPE){
                    printWarnMsg(functionNameNode, RETURN_TYPE_UNMATCH);
                }
                else if(returnType != INT_TYPE || signature->returnType != FLOAT_TYPE){
                    //printErrorMsg(returnNode, RETURN_TYPE_UNMATCH);    
                    //printf("[DEBUG] Unmatch return type.\n");
                }
            }
            break;
        default:
            printf("[DEBUG] Broken AST.\n");
    }
}

/* Process a block aka {} */
void processBlockNode(AST_NODE* blockNode){
    if(blockNode->nodeType != BLOCK_NODE){
        printf("[DEBUG] Not a block.\n");
        exit(1);
    }
    for(AST_NODE *block = blockNode->child ; block != NULL ; block = block->rightSibling){
        switch(block->nodeType){
            case VARIABLE_DECL_LIST_NODE:
                processVariableDeclListNode(block);
                break;
            case STMT_LIST_NODE:
                for(AST_NODE *stmt = block->child ; stmt != NULL ; stmt = stmt->rightSibling){
                    processStmtNode(stmt);
                }
                break;
            default:
                printf("[DEBUG] Unexpected block type.\n");
                break;
        }
    }
}


void processStmtNode(AST_NODE* stmtNode){
    printf("stmt node type is %s\n", NODE_TYPE_string[stmtNode->nodeType]);
    switch(stmtNode->nodeType){
        case BLOCK_NODE:
            processBlockNode(stmtNode);
            return;
        case NUL_NODE:
            return;
        case STMT_NODE:
            //printf("%d\n", stmtNode->semantic_value.stmtSemanticValue.kind);
            switch(stmtNode->semantic_value.stmtSemanticValue.kind){
                case WHILE_STMT:
                    checkWhileStmt(stmtNode);
                    break;
                case FOR_STMT:
                    checkForStmt(stmtNode);
                    break;
                case ASSIGN_STMT:
                    processExprRelatedNode(stmtNode);
                    break;
                case IF_STMT:
                    checkIfStmt(stmtNode);
                    break;
                case FUNCTION_CALL_STMT:
                    checkFunctionCall(stmtNode);
                    break;
                case RETURN_STMT:
                    checkReturnStmt(stmtNode);
                    break;
                default:
                    printf("[DEBUG] unexpected statement type.\n");
                    break;
            }
            break;
        default:
            printf("[DEBUG] unexpected node type.\n");
            break;
    }
}


void processGeneralNode(AST_NODE *node)
{
}

/* id Node is the const node inside [] of array*/
int processDeclDimList(AST_NODE* idNode, TypeDescriptor* typeDescriptor, int ignoreFirstDimSize){
    //printf("-----\n");
    int dimension = 0;
    while(idNode != NULL){
        if(idNode->nodeType == NUL_NODE){
            // There is no dimension in [].
            if(ignoreFirstDimSize){
                typeDescriptor->properties.arrayProperties.sizeInEachDimension[dimension] = -1;
            }
        }
        else{
            int size;
            AST_NODE *nameNode = NULL;
            SymbolTableEntry *entry = NULL;
            TypeDescriptor *desc = NULL;
            //printf("Variable %s node type %s\n", getIdByNode(idNode), NODE_TYPE_string[idNode->nodeType]);
            switch(idNode->nodeType){        
                case CONST_VALUE_NODE:
                    if(idNode->semantic_value.const1->const_type != INTEGERC){
                        nameNode = idNode->parent;
                        printErrorMsg(nameNode, ARRAY_SIZE_NOT_INT);        
                        size = 0;
                    }
                    else{
                        size = idNode->semantic_value.const1->const_u.intval;
                        //printf("size is %d\n", size);
                    }
                    break;
                case EXPR_NODE:
                    if(processExprRelatedNode(idNode) != INT_TYPE){
                        nameNode = idNode->parent;
                        printErrorMsg(nameNode, ARRAY_SIZE_NOT_INT);
                        size = 0;
                    }
                    else{
                        size = idNode->semantic_value.exprSemanticValue.constEvalValue.iValue;
                        //printf("size is %d\n", size);
                    }
                    break;
                case STMT_NODE:
                    entry = retrieveSymbol(getIdByNode(idNode->child));
                    if(entry->attribute->attr.functionSignature->returnType != INT_TYPE){
                        nameNode = idNode->parent;
                        printErrorMsg(nameNode, ARRAY_SIZE_NOT_INT);
                    }
                    break;
                case IDENTIFIER_NODE:
                    entry = retrieveSymbol(getIdByNode(idNode));
                    if(entry->attribute->attributeKind == TYPE_ATTRIBUTE){
                        //printErrorMsg()
                        //printf("[DEBUG] Id is type,  not variable.\n"); 
                    }
                    else{
                        desc = entry->attribute->attr.typeDescriptor;
                        switch(desc->kind){
                            case SCALAR_TYPE_DESCRIPTOR:
                                if(desc->properties.dataType != INT_TYPE){
                                    nameNode = idNode->parent;
                                    printErrorMsg(nameNode, ARRAY_SIZE_NOT_INT);
                                }
                                break;
                            case ARRAY_TYPE_DESCRIPTOR:
                                if(desc->properties.arrayProperties.elementType != INT_TYPE){
                                    nameNode = idNode->parent;
                                    printErrorMsg(nameNode, ARRAY_SIZE_NOT_INT);
                                }
                                break;
                        }
                    }   
                    break;
            }
            if(typeDescriptor != NULL){
                if(size < 0){
                    printErrorMsg(idNode->parent, ARRAY_SIZE_NEGATIVE);
                }
                else{
                    typeDescriptor->properties.arrayProperties.sizeInEachDimension[dimension] = size;
                }
            }
        }    
        dimension++;
        ignoreFirstDimSize = 0;
        idNode = idNode->rightSibling;
        if(dimension > MAX_ARRAY_DIMENSION){
            AST_NODE *nameNode = idNode->parent;
            printErrorMsg(nameNode, EXCESSIVE_ARRAY_DIM_DECLARATION);
            break;
        }   
    }
    if(typeDescriptor != NULL){
        typeDescriptor->properties.arrayProperties.dimension = dimension;
    }
    return dimension;
}

/* Declare a new function */
void declareFunction(AST_NODE* declarationNode){
    /* Check if node is a function decalration node */
    if(declarationNode->nodeType != DECLARATION_NODE || declarationNode->semantic_value.declSemanticValue.kind != FUNCTION_DECL){
        printErrorMsg(declarationNode, NOT_FUNCTION_NAME);
        //printf("[DEBUG] not function declaration.\n");
        //exit(1);
    }

    /* Initialize the function signature information */
    AST_NODE *retNode, *nameNode, *paramNode, *blockNode;
    retNode = declarationNode->child;
    nameNode = retNode->rightSibling;
    paramNode = nameNode->rightSibling;
    blockNode = paramNode->rightSibling;

    /* Check return type is already defined */
    FunctionSignature *func = (FunctionSignature*)malloc(sizeof(struct FunctionSignature));
    func->returnType = processTypeNode(retNode);
    if(func->returnType == ERROR_TYPE){
        printErrorMsg(retNode, SYMBOL_UNDECLARED);
        //printf("[DEBUG] undefined return type.\n");
        return;
    }

    /* Enter into the symbol table */
    SymbolAttribute *attr = newAttribute(FUNCTION_SIGNATURE);
    attr->attr.functionSignature = func;
    SymbolTableEntry *entry = enterSymbol(getIdByNode(nameNode), attr);
    if(entry == NULL){
        printErrorMsg(nameNode, SYMBOL_REDECLARE);
        return;
    }
    else{
        nameNode->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
    }

    openScope();
    g_currentFunctionDecl = declarationNode;

    /* Chaining parameters list  */
    func->parametersCount = 0;
    Parameter *newParam = NULL, *lastParam = NULL;
    for(AST_NODE *paramList = paramNode->child ; paramList != NULL ; paramList = paramList->rightSibling){
        func->parametersCount++;
        newParam = (Parameter*)malloc(sizeof(Parameter));

        AST_NODE *paramTypeNode = paramList->child;
        AST_NODE *paramIdNode = paramTypeNode->rightSibling;
        DATA_TYPE paramType = processTypeNode(paramTypeNode);
        if(paramType == ERROR_TYPE){
            printErrorMsg(paramTypeNode, SYMBOL_UNDECLARED);
            //printf("[DEBUG] Type of parameter is not defined.\n");
        }
        else if(paramType == VOID_TYPE){
            //printf("[DEBUG] Type of parameter is void.\n");
        }
        TypeDescriptor *desc;
        SymbolTableEntry *entry;
        switch(paramIdNode->semantic_value.identifierSemanticValue.kind){
            case NORMAL_ID:
                desc = newTypeDesc(SCALAR_TYPE_DESCRIPTOR);
                desc->properties.dataType = paramType;
                attr = newAttribute(VARIABLE_ATTRIBUTE);
                attr->attr.typeDescriptor = desc;
                entry = enterSymbol(getIdByNode(paramIdNode), attr);
                if(entry == NULL){
                    printErrorMsg(paramIdNode, SYMBOL_REDECLARE);
                    break;
                }
                else{
                    paramIdNode->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
                }
                newParam->type = desc;
                newParam->parameterName = getIdByNode(paramIdNode);
                break;

            case ARRAY_ID:
                desc = newTypeDesc(ARRAY_TYPE_DESCRIPTOR);
                desc->properties.arrayProperties.elementType = paramType;
                int tmp = processDeclDimList(paramIdNode->child, desc, 1);
                //printf("Array %s is of size %d\n", getIdByNode(paramIdNode), tmp);
                attr = newAttribute(VARIABLE_ATTRIBUTE);
                attr->attr.typeDescriptor = desc;

                entry = enterSymbol(getIdByNode(paramIdNode), attr);
                if(entry == NULL){
                    printErrorMsg(paramIdNode, SYMBOL_REDECLARE);
                    break;
                }
                else{
                    paramIdNode->semantic_value.identifierSemanticValue.symbolTableEntry = entry;
                }
                newParam->type = desc;
                newParam->parameterName = getIdByNode(paramIdNode);
                break;
            default:
                printf("[DEBUG] Unexpected type of variable.\n");
                break;
        }
        newParam->next = NULL;
        if(lastParam == NULL){
            func->parameterList = newParam;
        }
        else{
            lastParam->next = newParam;
        }
        lastParam = newParam;
    }
    processBlockNode(blockNode);
    closeScope();
    g_currentFunctionDecl = NULL;
    return;
}    
