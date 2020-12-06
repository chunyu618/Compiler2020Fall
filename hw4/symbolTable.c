#include "symbolTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// This file is for reference only, you are not required to follow the implementation. //

int HASH(char * str) {
	int idx=0;
	while (*str){
		idx = idx << 1;
		idx+=*str;
		str++;
	}
	return (idx & (HASH_TABLE_SIZE-1));
}

//SymbolTable symbolTable;
TableStack tableStack;
SegNameSpace segNameSpace;

SymbolTableEntry* newSymbolTableEntry(int nestingLevel)
{
    SymbolTableEntry* symbolTableEntry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
    symbolTableEntry->nextInHashChain = NULL;
    symbolTableEntry->prevInHashChain = NULL;
    symbolTableEntry->nextInSameLevel = NULL;
    symbolTableEntry->sameNameInOuterLevel = NULL;
    symbolTableEntry->attribute = NULL;
    symbolTableEntry->name = NULL;
    symbolTableEntry->nestingLevel = nestingLevel;
    return symbolTableEntry;
}

void removeFromHashChain(int hashIndex, SymbolTableEntry* entry){
    
}

void enterIntoHashChain(int hashIndex, SymbolTableEntry* entry){
    SymbolTableEntry *head = currentTable()->hashTable[hashIndex];
    if(head == NULL){
        head = entry;
        return;
    }

    while(head->nextInHashChain != NULL){
        head = head->nextInHashChain;
    }
    head->nextInHashChain = entry;
    entry->prevInHashChain = head;
}

void initializeSymbolTableStack(){
    initializeSegNameSpace();
    tableStack.currentScope = 0;
    for(int i = 0; i < HASH_TABLE_SIZE; i++){
        tableStack.entry[0].hashTable[i] = NULL;
    }
}

void symbolTableEnd(){
}

SymbolTableEntry* retrieveSymbol(char* symbolName){
}

SymbolTableEntry* enterSymbol(char* symbolName, SymbolAttribute* attribute){
    
}

//remove the symbol from the current scope
void removeSymbol(char* symbolName)
{
}

int declaredLocally(char* symbolName)
{
}

void openScope()
{
}

void closeScope()
{
}

void printTable(){
    for(int i = 0; i <= tableStack.currentScope; i++){
        printf("----------In scope %d----------\n", i);
        SymbolTable entry = tableStack.entry[i];
        for(int j = 0; j < TABLE_STACK_SIZE; j++){
            printf("-----In hash value %d-----\n", j);
            SymbolTableEntry *tmp = entry.hashTable[i];
            while(tmp != NULL){
                printf("Symbol Name %s\n", tmp->name);
            }
            printf("NULL\n");
        }
    }
}

void initializeSegNameSpace(){
    segNameSpace.segIndex = 0;
    newSegment();
}

void newSegment(){
    if(segNameSpace.segIndex >= SEGMENT_NUM){
        printError("Exceeds Maximum Number of Segments");
    }

    segNameSpace.segIndex += 1;
    segNameSpace.segments[segNameSpace.segIndex - 1] = (char*)malloc(sizeof(char) * (SEGMENT_SIZE + 1));
    memset(segNameSpace.segments[segNameSpace.segIndex - 1], 0, sizeof(char) * (SEGMENT_SIZE + 1));
    segNameSpace.currentOffset = 0;
}

void printError(char *errMsg){
    printf("%d\n", errMsg);
    exit(1);
}

__inline__ SymbolTable* currentTable(){
    return &tableStack.entry[tableStack.currentScope];
}
