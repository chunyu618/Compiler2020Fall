#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "symbolTable.h"
// This file is for reference only, you are not required to follow the implementation. //
// You only need to check for errors stated in the hw4 document. //
int g_anyErrorOccur = 0;

char *DATA_TYPE_string[] = {"int", "float", "void"};

DATA_TYPE getBiggerType(DATA_TYPE dataType1, DATA_TYPE dataType2);
void processProgramNode(AST_NODE *programNode);
void processVariableDeclListNode(AST_NODE *declListNode);
void processDeclarationNode(AST_NODE* declarationNode);
void declareIdList(AST_NODE* typeNode, SymbolAttributeKind isVariableOrTypeAttribute, int ignoreArrayFirstDimSize);
void declareFunction(AST_NODE* returnTypeNode);
void processDeclDimList(AST_NODE* variableDeclDimList, TypeDescriptor* typeDescriptor, int ignoreFirstDimSize);
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
void checkFunctionCall(AST_NODE* functionCallNode);
void processExprRelatedNode(AST_NODE* exprRelatedNode);
void checkParameterPassing(Parameter* formalParameter, AST_NODE* actualParameter);
void checkReturnStmt(AST_NODE* returnNode);
void processExprNode(AST_NODE* exprNode);
void processVariableLValue(AST_NODE* idNode);
void processVariableRValue(AST_NODE* idNode);
void processConstValueNode(AST_NODE* constValueNode);
void getExprOrConstValue(AST_NODE* exprOrConstNode, int* iValue, float* fValue);
void evaluateExprValue(AST_NODE* exprNode);

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

void printErrorMsgSpecial(AST_NODE* node, char* name2, ErrorMsgKind errorMsgKind)
{
    g_anyErrorOccur = 1;
    printf("Error found in line %d\n", node->linenumber);
    
    switch(errorMsgKind)
    {
        case SYMBOL_REDECLARE:
            printf("Error found in line%d.\n", node->linenumber);
            printf("redefinition of '%s'.\n", getIdByNode(node));
            break;
        case SYMBOL_UNDECLARED:
            printf("Error found in line%d.\n", node->linenumber);
            printf("use of undeclared idnetifier '%s'.\n", getIdByNode(node));
            break;
        case TOO_FEW_ARGUMENTS:
            printf("Error found in line%d.\n", node->linenumber);
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        case TOO_MANY_ARGUMENTS:
            printf("Error found in line%d.\n", node->linenumber);
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        case RETURN_TYPE_UNMATCH:
            printf("Warning found in line%d.\n", node->linenumber);
            printf("implicit conversion turns floating-point number intot integer: '%s' to '%s'", DATA_TYPE_string[node->dataType], DATA_TYPE_string[node->dataType]);
            break;
        case NOT_ARRAY:
            printf("Error found in line%d.\n", node->linenumber);
            printf("subscripted value is not an array, pointer, or vector.\n");
            break;
        case NOT_ASSIGNABLE:
            printf("Error found in line%d.\n", node->linenumber);
            printf("array type '%s [%d]' is not assignable.\n", DATA_TYPE_string[node->dataType], node->semantic_value.identifierSemanticValue.symbolTableEntry->attribute->attr.typeDescriptor->properties.arrayProperties.dimension);
            break;
        case ARRAY_SIZE_NOT_INT:
            printf("Error found in line%d.\n", node->linenumber);
            printf("array subscript is not an integer.\n");
            break;
        case PASS_ARRAY_TO_SCALAR:
            printf("Error found in line%d.\n", node->linenumber);
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        case PASS_SCALAR_TO_ARRAY:
            printf("Error found in line%d.\n", node->linenumber);
            printf("no matching function for call '%s'.\n", getIdByNode(node));
            break;
        default:
            printf("Unhandled case in void printErrorMsg(AST_NODE* node, ERROR_MSG_KIND* errorMsgKind)\n");
    }
    
}


void printErrorMsg(AST_NODE* node, ErrorMsgKind errorMsgKind)
{
    g_anyErrorOccur = 1;
    printf("Error found in line %d\n", node->linenumber);
    /*
    switch(errorMsgKind)
    {
        printf("Unhandled case in void printErrorMsg(AST_NODE* node, ERROR_MSG_KIND* errorMsgKind)\n");
        break;
    }
    */
}

void semanticAnalysis(AST_NODE *root)
{
    //printTable();
    printf("----------Print Over----------\n");
    int levelCount[32] = {};
    printNode(root, 0, levelCount);
    //processProgramNode(root);
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
    if(isVariableOrTypeAttribute == VARIABLE_ATTRIBUTE){
    
    }
    else if(isVariableOrTypeAttribute == TYPE_ATTRIBUTE){
    
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

void checkFunctionCall(AST_NODE* functionCallNode)
{
}

void checkParameterPassing(Parameter* formalParameter, AST_NODE* actualParameter)
{
}


void processExprRelatedNode(AST_NODE* exprRelatedNode)
{
}

void getExprOrConstValue(AST_NODE* exprOrConstNode, int* iValue, float* fValue)
{
}

void evaluateExprValue(AST_NODE* exprNode)
{
}


void processExprNode(AST_NODE* exprNode)
{
}


void processVariableLValue(AST_NODE* idNode)
{
}

void processVariableRValue(AST_NODE* idNode)
{
}


void processConstValueNode(AST_NODE* constValueNode)
{
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

void processDeclDimList(AST_NODE* idNode, TypeDescriptor* typeDescriptor, int ignoreFirstDimSize)
{
}


void declareFunction(AST_NODE* declarationNode)
{
}
