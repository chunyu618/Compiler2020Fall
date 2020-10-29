#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include"header.h"

#define TABLE_SIZE	512

symtab * hash_table[TABLE_SIZE];
extern int linenumber;

int HASH(char * str){
	int idx=0;
	while(*str){
		idx = idx << 1;
		idx+=*str;
		str++;
	}	
	return (idx & (TABLE_SIZE-1));
}

/*returns the symbol table entry if found else NULL*/

symtab * lookup(char *name){
	int hash_key;
	symtab* symptr;
	if(!name)
		return NULL;
	hash_key=HASH(name);
	symptr=hash_table[hash_key];

	while(symptr){
		if(!(strcmp(name,symptr->lexeme)))
			return symptr;
		symptr=symptr->front;
	}
	return NULL;
}


void insertID(char *name){
	int hash_key;
	symtab* ptr;
	symtab* symptr=(symtab*)malloc(sizeof(symtab));	
	
	hash_key=HASH(name);
	ptr=hash_table[hash_key];
	
	if(ptr==NULL){
		/*first entry for this hash_key*/
		hash_table[hash_key]=symptr;
		symptr->front=NULL;
		symptr->back=symptr;
	}
	else{
		symptr->front=ptr;
		ptr->back=symptr;
		symptr->back=symptr;
		hash_table[hash_key]=symptr;	
	}
	
	strcpy(symptr->lexeme,name);
	symptr->line=linenumber;
	symptr->counter=1;
}

int compare(const void *a, const void *b){
    return strcmp((*(symtab**)a)->lexeme, (*(symtab**)b)->lexeme);
}

void printSymTab()
{
    symtab **symList = (symtab**)malloc(sizeof(symtab*) * TABLE_SIZE);
    printf("----- Symbol Table ---------\n");
    int numberOfID = 0;
    for(int i=0; i<TABLE_SIZE; i++){
        symtab* symptr = hash_table[i];
	    //printf("while\n");
        while (symptr != NULL){
            symList[numberOfID] = symptr; 
	        symptr=symptr->front;
            numberOfID++;
	    }
    }
    qsort(symList, numberOfID, sizeof(symtab*), compare);
    for(int i = 0; i < numberOfID; i++){
        printf("%s\t\t%d\n", symList[i]->lexeme, symList[i]->counter);
    }
}
