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

void removeFromHashTrain(int hashIndex, SymbolTableEntry* entry)
{
}

void enterIntoHashTrain(int hashIndex, SymbolTableEntry* entry)
{
}

void initializeSymbolTableStack(){
    tableStack.currentScope = 0;
    for(int i = 0; i < HASH_TABLE_SIZE; i++){
        tableStack.entry[0].hashTable[i]->name = NULL;
    }
}

void symbolTableEnd()
{
}

SymbolTableEntry* retrieveSymbol(char* symbolName)
{
}

SymbolTableEntry* enterSymbol(char* symbolName, SymbolAttribute* attribute)
{
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
    for(int i = 0; i < tableStack.currentScope; i++){
        printf("----------In scope %d----------\n", i);
        SymbolTable entry = tableStack.entry[i];
        for(int j = 0; j < TABLE_STACK_SIZE; j++){
            printf("-----In hash value %d-----\n", j);
            SymbolTableEntry *tmp = entry.hashTable[i];
            while(tmp->name != NULL){
                printf("Symbol Name %s\n", tmp->name);
            }
            printf("NULL\n");
        }
    }
}

__inline__ SymbolTable* currentTable(){
    return &tableStack.entry[tableStack.currentScope];
}
