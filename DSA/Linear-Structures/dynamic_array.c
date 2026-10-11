#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"

// Takes in a static array returns a
// dynamic vector
VectN new_VectN(Item* arr, int n){
	Item* data = malloc(sizeof(Item)*2*n);
	// copy over data
	for(int i = 0; i < n; i++){
		data[i] = arr[i];
	}
	// creating new Dynamic vector
	VectN newVectN = {
		.size = n,
		.capacity = n*2,
		.data = data
	};
	return newVectN;
}


// prints the entire array
void printVectN(VectN nums){
	for (int i=0; i<nums.size; i++){
		printf("%i\n", nums.data[i]);
	}
	printf("\n");
}

// Creates an item from a given value and type and
// Stores it in the privided address
int new_Item(void* value, types type, Item* dest){
	if (type >= LAST_TYPE){
		return ERROR;
	}
	*dest = (Item){
		.type = type,
		.value = value
	};
	return SUCCESS;
}

// appends element at the end
void appendTo(VectN* List, void* e, types type){
	if (List->size >= List->capacity){
		Item* temp = List->data;
		Item* data = malloc(List->capacity*2);
		// copying over elements
		for (int i=0; i<List->size; i++){
			data[i] = List->data[i];
		}
		List->capacity *= 2;
		List->data = data;
		free(temp);
	}
	Item item;
	new_Item(e, type, &item);
	List->data[List->size] = item;
	List->size += 1;
}

// pops element at the provided index
int pop(VectN* List, int index, Item* element){
	if (index > List->size-1){
		return ERROR;
	}
	*element = List->data[index];
	if (index == List->size-1){
		List->size -= 1;
		return SUCCESS;
	}
	for (int i=index; i<List->size-1; i++){
		List->data[i] = List->data[i+1];
	}
	List->size -= 1;
	return SUCCESS;	
}