#include <stdio.h>
#include <stdlib.h>
#include "../Utils/dynamicArray.h"

// Usage
int main(int argc, char *argv[])
{
	int nums[] = {1, 2, 3, 4, 5};
	int n = sizeof(nums)/sizeof(int);
	VectN dynamic_nums = new_VectN(nums, n);
	printf("orignal: \n");
	printVectN(dynamic_nums);
	appendTo(&dynamic_nums, 6);
	printf("appended: 6\n");
	printVectN(dynamic_nums);
	printf("popped: 6\n");
	pop(&dynamic_nums, dynamic_nums.size-1);
	printVectN(dynamic_nums);
}


// Takes in a static array returns a
// dynamic vector
VectN new_VectN(int arr[], int n){
	int* data = malloc(2*n);
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


// appends element at the end
void appendTo(VectN* nums, int e){
	if (nums->size >= nums->capacity){
		int* data = malloc(nums->capacity*2);
		// copying over elements
		for (int i=0; i<nums->size; i++){
			data[i] = nums->data[i];
		}
		nums->capacity *= 2;
	}
	nums->data[nums->size] = e;
	nums->size += 1;
}

// pops element at the provided index
int pop(VectN* nums, int index, int* element){
	if (index > nums->size-1){
		return 1;
	}
	*element = nums->data[index];
	if (index == nums->size-1){
		nums->data[index] = 0;
		nums->size -= 1;
		return 0;
	}
	for (int i=index; i<nums->size-1; i++){
		nums->data[i] = nums->data[i+1];
	}
	nums->size -= 1;
	return 0;	
}