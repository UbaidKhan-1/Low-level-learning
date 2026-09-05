#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../Utils/array_sorting.h"


//------------ Main -------------

int main(int argc, char *argv[])
{
	int nums[] = {42, -17, 85, 0, -17, 93, -5, 61, 12, -88};
	int n = sizeof(nums) / sizeof(nums[0]); // Array length is 10
	
	printf("un-sorted array: ");
	print_Array(nums, n);
	
	mergeSort(nums, n);
	
	printf("sorted array: ");
	print_Array(nums, n);
}

//_____________________________________

// Functions

void Bubble_Sort(int nums[], int n){
	bool hasbeensorted = false;
	// Bubbling process needs to repeat
	// n-1 times (At most)
	for (int i=0; i<n-1; i++){
		hasbeensorted = true;
		for (int i=0; i<n-1; i++){ 
			// if in wrong order
			if (nums[i] > nums[i+1]){
				// Swap them
				int num = nums[i];
				nums[i] = nums[i+1];
				nums[i+1] = num;
				hasbeensorted = false; 
				//if even one swap was
				//needed in a cycle, it is
				//assumed that list isnt
				//sorted yet
			}
		}
		if (hasbeensorted){
			break;
		}
	}
}

void Selection_sort(int nums[], int n){
	int start = 0;
	//At most n numbers need to be swapped
	//Thus this loop
	while (start < n){
		// Finding the smallest value
		float smallest_value = INT_MAX;
		int smallest_index;
		for (int i=start; i<n; i++){
			if (nums[i] < smallest_value){
				smallest_value = nums[i];
				smallest_index = i;
			}
		}
		// Swapping with the first unsorted value
		nums[smallest_index] = nums[start];
		nums[start] = smallest_value;
		start += 1;	
	}	
}

int* mergeSort_H(int nums[], int n, int start, int end){
	if (start == end){
		int* arr = malloc(sizeof(int));
		arr[0] = nums[start];
		return arr;
	}
	int mid = (start+end)/2 + 1;
	int ln = mid-start;
	int rn = end-mid+1;
	int* left = mergeSort_H(nums, ln, start, mid-1);
	int* right = mergeSort_H(nums, rn, mid, end);
	
	int* merged = malloc(sizeof(int)*n);
	int mergedindex = 0;
	int lp = 0;
	int rp = 0;
	
	while (lp < ln && rp < rn){
		if (left[lp] <= right[rp]){
			merged[mergedindex]=left[lp];
			mergedindex += 1;
			lp += 1;
		}
		else{
			merged[mergedindex] = right[rp];
			mergedindex += 1;
			rp += 1;
		}
	}
	while(lp<ln){
		merged[mergedindex] = left[lp];
		mergedindex += 1;
		lp += 1;
	}
	
	while(rp<rn){
		merged[mergedindex] = right[rp];
		mergedindex += 1;
		rp += 1;
	}
	
	free(left);
	free(right);
	return merged;
	
}
void mergeSort(int* nums, int n){
	int* sorted=mergeSort_H(nums, n, 0, n-1);	for (int i=0; i<n; i++){
		nums[i] = sorted[i];
	}
	free(sorted);
}

void print_Array(int nums[], int n){
	printf("[");
	for(int i=0; i<n; i++){
		if (i<n-1){
			printf("%i,", nums[i]);
		} else{
			printf("%i", nums[i]);
		}
	}
	printf("]\n");
}