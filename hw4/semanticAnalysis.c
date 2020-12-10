#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "symbolTable.h"
// This file is for reference only, you are not required to follow the implementation. //
// You only need to check for errors stated in the hw4 document. //
int g_anyErrorOccur = 0;
AST_NODE *g_currentFunctionDecl = NULL;

char *DATA_TYPE_string[] = {"int", "float", "void"};

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
void checkParameterPassing(Parameter* formalParameter, AST_NODE* actualParameter);
void checkReturnStmt(AST_NODE* returnNode);
DATA_TYPE processExprNode(AST_NODE* exprNode);
DATA_TYPE processVariableLValue(AST_NODE* idNode);
void processVariableRValue(AST_NODE* idNode);
DATA_TYPE processConstValueNode(AST_NODE* constValueNode);
void getExprOrConstValue(AST_NODE* exprOrConstNode, int* iValue, float* fValue);
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
        default:
            break;
    } 
    
    
}


void printErrorMsg(AST_NODE* node, ErrorMsgKind errorMsgKind){
    SymbolTableEntry *entry;
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
        case TOO_FEW_ARGUMENTS:
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        case TOO_MANY_ARGUMENTS:
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        case RETURN_TYPE_UNMATCH:
            printf("Warning found in line%d.\n", node->linenumber);
            printf("implicit conversion turns floating-point number intot integer: '%s' to '%s'", DATA_TYPE_string[node->dataType], DATA_TYPE_string[node->dataType]);
            break;
        case NOT_ARRAY:
            printf("subscripted value is not an array, pointer, or vector.\n");
            break;
        case NOT_ASSIGNABLE:
            printf("array type '%s [%d]' is not assignable.\n", DATA_TYPE_string[node->dataType], node->semantic_value.identifierSemanticValue.symbolTableEntry->attribute->attr.typeDescriptor->properties.arrayProperties.dimension);
            break;
        case ARRAY_SIZE_NOT_INT:
            printf("array subscript is not an integer.\n");
            break;
        case PASS_ARRAY_TO_SCALAR:
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        case PASS_SCALAR_TO_ARRAY:
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        default:
            printf("Unhandled case in void printErrorMsg(AST_NODE* node, ERROR_MSG_KIND* errorMsgKind)\n");
    }
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
            declareFunction(declNode);
            break;
        default:
            printError("Invalid declaration type");
    }
}

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


void declareIdList(AST_NODE* declarationNode, SymbolAttributeKind isVariableOrTypeAttribute, int ignoreArrayFirstDimSize){
    //printf("Id List\n");
    if(isVariableOrTypeAttribute == VARIABLE_ATTRIBUTE){
        AST_NODE* typeNode = declarationNode->child;
        //if(typeNode == NULL) printf("lalala");
        DATA_TYPE dataType = processTypeNode(typeNode);
        printf("%d\n", dataType);
        if(dataType == ERROR_TYPE){
            printf("[DEBUG] data type is error type.\n");
            return;
        } 
        else if(dataType == VOID_TYPE){
            printf("[DEBUG] data type is void type.\n");
            return;
        }
        for(AST_NODE *id = typeNode->rightSibling; id != NULL; id = id->rightSibling){
            int count[32] = {};
            //printNode(id, 0, count);    
            AST_NODE *exprNode;
            TypeDescriptor *typeDesc;
            SymbolAttribute *symbolAttr = newAttribute(VARIABLE_ATTRIBUTE);
            DATA_TYPE exprType;
            switch(id->semantic_value.identifierSemanticValue.kind){
                case NORMAL_ID:
                    // Prepare symbol attribute
                    typeDesc = newTypeDesc(SCALAR_TYPE_DESCRIPTOR);
                    typeDesc->properties.dataType = dataType;
                    symbolAttr->attr.typeDescriptor = typeDesc;
                    
                    // Enter into symbol table
                    if(enterSymbol(getIdByNode(id), symbolAttr) == NULL){
                        printErrorMsg(id, SYMBOL_REDECLARE);
                    }
                    break;
                case WITH_INIT_ID:
                    // Check expression type
                    exprNode = id->child;
                    exprType = processExprRelatedNode(exprNode);
                    if(exprType == CONST_STRING_TYPE){
                        //printfErrorMsg(exprNode, )
                        printf("[DEBUG] expression type is const string.\n");
                        break;
                    }
                    else if (exprType == VOID_TYPE){
                        printf("[DEBIG] expression type is void.\n");
                        break;
                    }
                    else if(exprType == ERROR_TYPE){
                        printf("[DEBUG] expression type is error type.\n");
                        break;
                    }
                    
                    // Prepare symbol attribute
                    typeDesc = newTypeDesc(SCALAR_TYPE_DESCRIPTOR);
                    typeDesc->properties.dataType = dataType;
                    symbolAttr->attr.typeDescriptor = typeDesc;
                    
                    // Enter into symbol table
                     if(enterSymbol(getIdByNode(id), symbolAttr) == NULL){
                        printErrorMsg(id, SYMBOL_REDECLARE);
                    }
                    break;
                case ARRAY_ID:
                    // Prepare symbol attribute
                    typeDesc = newTypeDesc(ARRAY_TYPE_DESCRIPTOR);
                    typeDesc->properties.arrayProperties.elementType = dataType;
                    processDeclDimList(id->child, typeDesc, 0);
                    symbolAttr->attr.typeDescriptor = typeDesc;
                    
                    // Enter into symbol table
                    if(enterSymbol(getIdByNode(id), symbolAttr) == NULL){
                        printErrorMsg(id, SYMBOL_REDECLARE);
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
        if(dataType == ERROR_TYPE){
            printf("[DEBUG] Symbol is not a type.\n");
        }
        for(AST_NODE *id = typeNode->rightSibling; id != NULL; id = id->rightSibling){
            if(insertType(getIdByNode(id), dataType) == NULL){
                DATA_TYPE oldDataType = retrieveSymbol(getIdByNode(id))->attribute->attr.typeDescriptor->properties.dataType;
                printf("%d %d\n", dataType, oldDataType);
                if(dataType == oldDataType){
                    printErrorMsg(id, SYMBOL_REDECLARE);
                }
                else{
                    printErrorMsgSpecial(id, DATA_TYPE_string[dataType], TYPE_REDECLARE);
                }
            }
        }
    }
}

void checkAssignOrExpr(AST_NODE* assignOrExprRelatedNode)
{
}

void checkWhileStmt(AST_NODE* whileNode)
{
}


void checkForStmt(AST_NODE* forNode)
{
}


void checkAssignmentStmt(AST_NODE* assignmentNode)
{
}


void checkIfStmt(AST_NODE* ifNode)
{
}

void checkWriteFunction(AST_NODE* functionCallNode)
{
}

DATA_TYPE checkFunctionCall(AST_NODE* functionCallNode){
    AST_NODE *functionNameNode = functionCallNode->child;
    SymbolTableEntry *entry = retrieveSymbol(getIdByNode(functionNameNode));
    if(entry == NULL){
        printErrorMsg(functionNameNode, SYMBOL_UNDECLARED);
        return ERROR_TYPE;
    }

    /* To check whether ID is a function name*/
    if(entry->attribute->attributeKind != FUNCTION_SIGNATURE){
        printErrorMsg(functionNameNode, NOT_FUNCTION_NAME);
        return ERROR_TYPE;
    }
   
    /* Check parameters*/
    
    
}

void checkParameterPassing(Parameter* formalParameter, AST_NODE* actualParameter)
{
}

/* Process EXPR_NODE, CONST_VALUE_NODE, STMT_NODE, IDENTIFIER_NODE, NUL_NODE*/
DATA_TYPE processExprRelatedNode(AST_NODE* exprRelatedNode){
    AST_NODE *leftValueNode;
    switch(exprRelatedNode->nodeType){
        case EXPR_NODE:
            return processExprNode(exprRelatedNode);
        case CONST_VALUE_NODE:
            return processConstValueNode(exprRelatedNode);
        case STMT_NODE:
            switch(exprRelatedNode->semantic_value.stmtSemanticValue.kind){
                case FUNCTION_CALL_STMT:
                    return checkFunctionCall(exprRelatedNode);
                case ASSIGN_STMT:
                    leftValueNode = exprRelatedNode->child;
                    DATA_TYPE leftDataType = processExprRelatedNode(leftValueNode);
                    if(leftDataType == ERROR_TYPE || leftDataType == VOID_TYPE){
                        return ERROR_TYPE;
                    }
                    DATA_TYPE rightDataType = processExprRelatedNode(leftValueNode->rightSibling);
                    if(rightDataType != leftDataType) {
                        if(rightDataType == CONST_STRING_TYPE){
                            printf("[DEBUG] cannot be assigned as a string.\n");
                            return ERROR_TYPE;
                        }
                        else if(rightDataType == VOID_TYPE){
                            printf("[DEBUG] cannot be assigned as a void type.\n");
                            return ERROR_TYPE;
                        }
                        else if(rightDataType == ERROR_TYPE){
                            return ERROR_TYPE;
                        }
                    }
                    return leftDataType;
                default:
                    printf("[DEBUG] Broken AST.\n");
                    exit(1);
            }
        case IDENTIFIER_NODE:
            return processVariableLValue(exprRelatedNode);
        default:
            printf("[DEBUG] Unexpected ExprRelated Node.\n");
            return ERROR_TYPE;
    }
}

void getExprOrConstValue(AST_NODE* exprOrConstNode, int* iValue, float* fValue)
{
}

void evaluateExprValue(AST_NODE* exprNode, DATA_TYPE dataType){
}


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
            if(leftDataType == VOID_TYPE || rightDataType == VOID_TYPE){
                printf("[DEBUG] void operation.\n");
                return ERROR_TYPE;
            }
            if(leftDataType == CONST_STRING_TYPE || rightDataType == CONST_STRING_TYPE){
                printf("[DEBUG] constant string operation.\n");
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
            if(dataType != INT_TYPE || dataType != FLOAT_TYPE){
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
    exprNode->dataType = dataType;
    return dataType;
}


DATA_TYPE processVariableLValue(AST_NODE* idNode)
{
}

void processVariableRValue(AST_NODE* idNode)
{
}


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


void checkReturnStmt(AST_NODE* returnNode)
{
}


void processBlockNode(AST_NODE* blockNode)
{
}


void processStmtNode(AST_NODE* stmtNode)
{
}


void processGeneralNode(AST_NODE *node)
{
}

int processDeclDimList(AST_NODE* idNode, TypeDescriptor* typeDescriptor, int ignoreFirstDimSize){
}

void declareFunction(AST_NODE* declarationNode){
    /* Check if node is a function decalration node */
    if(declarationNode->nodeType != DECLARATION_NODE || declarationNode->semantic_value.declSemanticValue.kind != FUNCTION_DECL){
        printf("[DEBUG] not function declaration.\n");
        exit(1);
    }

    /* Initialize the function signature information */
    AST_NODE *ret_node, *name_node, *param_node, *block_node;
    retNode = declarationNode->child;
    nameNode = retNode->rightSibling;
    paramNode = nameNode->rightSibling;
    blockNode = paramNode->rightSibling;

    /* Check return type is already defined */
    FunctionSignature *func = (FunctionSignature*)malloc(sizeof(struct FunctionSignature));
    func->returnType = processTypeNode(retNode);
    if(func->returnType == ERROR_TYPE){
        printf("[DEBUG] undefined return type.\n");
        return;
    }

    /* Enter into the symbol table */
    SymbolAttribute *attr = newAttribute(FUNCTION_SIGNATURE);
    attr->attr.functionSignature = func;
    if(enterSymbol(getIdByNode(name_node), attr) == NULL){
        printErrorMsg(name_node, SYMBOL_REDECLARE);
        return;
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
            printf("[DEBUG] Type of parameter is not defined.\n");
        }
        else if(paramType == VOID_TYPE){
            printf("[DEBUG] Type of parameter is void.\n");
        }
        TypeDescriptor *desc;
        switch(paramIdNode->semantic_value.identifierSemanticValue.kind){
            case NORMAL_ID:
                desc = newTypeDesc(SCALAR_TYPE_DESCRIPTOR);
                desc->properties.dataType = paramType;
                attr = newAttribute(VARIABLE_ATTRIBUTE);
                attr->attr.typeDescriptor = desc;
                if(enterSymbol(getIdByNode(paramIdNode), attr) == NULL){
                    printErrorMsg(paramIdNode, SYMBOL_REDECLARE);
                    break;
                }
                newParam->type = desc;
                newParam->parameterName = getIdNode(paramIdNode);
                break;

            case ARRAY_ID:
                desc = newTypeDesc(ARRAY_TYPE_DESCRIPTOR);
                break;
        }
    }
}
