#include "symbolTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// This file is for reference only, you are not required to follow the implementation. //

int HASH(char *str) {
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

SymbolTableEntry* newSymbolTableEntry(int scope)
{
    SymbolTableEntry* symbolTableEntry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
    symbolTableEntry->nextInHashChain = NULL;
    symbolTableEntry->prevInHashChain = NULL;
    symbolTableEntry->nextInSameLevel = NULL;
    symbolTableEntry->sameNameInOuterLevel = NULL;
    symbolTableEntry->attribute = NULL;
    symbolTableEntry->segIndex = 0;
    symbolTableEntry->nameLength = 0;
    symbolTableEntry->scope = scope;
    return symbolTableEntry;
}

void removeFromHashChain(int hashIndex, SymbolTableEntry* entry){
    
}

int enterNameSpace(char *symbolName){
    int len = strlen(symbolName);
    if(len > SEGMENT_SIZE){
        printError("Maximum namespace size reached.");
    }
    for(int i = 0 ; i < segNameSpace.segIndex ; i++){
        char *tmp = strstr(segNameSpace.segments[i], symbolName);
        if(tmp != NULL){
            return makeIndex(tmp - segNameSpace.segments[i], i);
        }
    }
    if(segNameSpace.currentOffset + len > SEGMENT_SIZE){
        newSegment();
    }
    strncpy(currentEmpty(), symbolName, len);
    segNameSpace.currentOffset += len;
    return makeIndex(segNameSpace.currentOffset - len, segNameSpace.segIndex - 1);
}

void enterIntoHashChain(int hashIndex, SymbolTableEntry* entry){
    SymbolTableEntry **head = &(currentTable()->hashTable[hashIndex]);
    if(*head == NULL){
        *head = entry;
        printf("--------%d\n", currentTable()->hashTable[hashIndex]->nameLength);
        return;
    }
   
    while((*head)->nextInHashChain != NULL){
        *head = (*head)->nextInHashChain;
    }
    (*head)->nextInHashChain = entry;
    entry->prevInHashChain = *head;
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
    // Calculate Hash Value
    int hashValue = HASH(symbolName);
    // Get Entry from Table of Lager scope to smaller scope
    int currentScope = tableStack.currentScope;
    for(int i = currentScope; i >= 0; i--){
        SymbolTable table = tableStack.entry[i];
        SymbolTableEntry *entry = table.hashTable[hashValue];
        while(entry != NULL){
            if(strlen(symbolName) == entry->nameLength && strncmp(symbolName, getName(entry->segIndex), entry->nameLength) == 0){
                return entry;
            }
            entry = entry->nextInHashChain;
        }
    }
    return NULL;
}

SymbolTableEntry* enterSymbol(char* symbolName, SymbolAttribute* attribute){
    // Calculate hash value
    int hashValue = HASH(symbolName);

    // Check if redeclared
    SymbolTableEntry* tmp = retrieveSymbol(symbolName);
    if(tmp != NULL && tmp->scope == tableStack.currentScope){
        return NULL;
    }
    
    // Initialize a new table entry
    SymbolTableEntry *newEntry = newSymbolTableEntry(tableStack.currentScope);
    newEntry->segIndex = enterNameSpace(symbolName);
    newEntry->nameLength = strlen(symbolName);
    newEntry->attribute = attribute;

    // enter into hash table
    enterIntoHashChain(hashValue, newEntry);
    
    return newEntry;
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
        for(int j = 0; j < HASH_TABLE_SIZE; j++){
            printf("-----In hash value %d-----\n", j);
            SymbolTableEntry *tmp = entry.hashTable[j];
            while(tmp != NULL){
                char name[32] = {};
                strncpy(name, getName(tmp->segIndex), tmp->nameLength);
                name[tmp->nameLength] = '\0';
                printf("Symbol Name %s\n", name);
                tmp = tmp->nextInHashChain;
            }
            //printf("NULL\n");
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

__inline__ char *currentEmpty(){
    return segNameSpace.segments[segNameSpace.segIndex - 1] + segNameSpace.currentOffset;
}

__inline__ int makeIndex(int offset, int segment){
    return offset + SEGMENT_SIZE * segment;
}

__inline__ SymbolTable* currentTable(){
    return &tableStack.entry[tableStack.currentScope];
}

__inline__ char *getName(int index){
    return segNameSpace.segments[index / SEGMENT_SIZE] + index % SEGMENT_SIZE;
}
