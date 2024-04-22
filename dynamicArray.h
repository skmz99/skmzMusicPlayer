#ifndef _DYNAMICARRAY_H
#define _DYNAMICARRAY_H

#include <stddef.h>
// This dynamicArray is to save the original title/folder of songs and save an alternative name with a 
// limit of 26 characters to fit into the 1.3 oled screen
typedef struct{
	char **array;
	char **altName;
	size_t altUsed;
	size_t altSize;
	size_t used;
	size_t size;
}Array;

void initArray(Array *a, size_t initialSize);
void insertArray(Array *a, char element[]);
void insertAltName(Array *a, char element[]);
void freeArray(Array *a);
#endif
