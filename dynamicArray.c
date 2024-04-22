#include <stdlib.h>
#include <string.h>

#include "dynamicArray.h"

void initArray(Array *a, size_t initialSize){
	a->array = malloc(initialSize * sizeof(char*));
	a->altName = malloc(initialSize * sizeof(char*));
	a->altUsed = 0;
	a->altSize = initialSize;
	a->used = 0;
	a->size = initialSize;
};

//Stores the orginal names of files from main.c into array
void insertArray(Array *a, char element[]){
	a->array[a->used] = malloc(strlen(element)+1);
	strcpy(a->array[a->used], element);
	a->used++;

	if(a->used == a->size){
		a->size *= 2;
		a->array = realloc(a->array, a->size * sizeof(char*));
	}
};

//in main.c will shorten Name of files with length of 20 chars this will store the alternative name into altName
void insertAltName(Array *a, char element[]){
	a->altName[a->altUsed] = malloc(strlen(element)+1);
	strcpy(a->altName[a->altUsed], element);
	a->altUsed++;

	if(a->altUsed == a->altSize){
		a->altSize *= 2;
		a->altName = realloc(a->altName, a->altSize * sizeof(char*));
	}
}

void freeArray(Array *a){
	for(int i =0 ; i < a->used; i++){
		free(a->array[i]);
	}

	for(int i = 0; i < a->altUsed; i++){
		free(a->altName[i]);
	}
	a->used = a->size = 0;
	a->altUsed = a->altSize = 0;
};

