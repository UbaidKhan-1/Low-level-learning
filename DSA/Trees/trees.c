#include <stdio.h>
#include <stdlib.h>
#include "../Utils/trees.h"
#include "../Utils/dynamicArray.h"


// converts an array into a binary tree.
// To create a BST pass in a sorted array
BinaryNode *createBinaryTree(int arr[], int n)
{
	return createBinaryTree_r(arr, 0, n - 1, NULL);
}
BinaryNode *createBinaryTree_r(int arr[], int start, int end, BinaryNode *parent)
{
	if (start > end)
	{
		return NULL;
	}
	int mid = (start + end) / 2;
	int val = arr[mid];

	BinaryNode *node = malloc(sizeof(BinaryNode));

	node->parent = parent;
	node->val = val;

	node->left = createBinaryTree_r(arr, start, mid - 1, node);
	node->right = createBinaryTree_r(arr, mid + 1, end, node);
	return node;
}

void freeBinaryTree(BinaryNode* root)
{
	if (root == NULL){
		return;
	}
	freeBinaryTree(root->left);
	freeBinaryTree(root->right);
	free(root);
}

void printTree(BinaryNode* node){
	if (node == NULL){
		return;
	}
	printf(" %i\n", node->val);
	printTree(node->left);
	printTree(node->right);
}


VectN dfs(BinaryNode* node, int value, int level){
	if (node == NULL){
		VectN path = {
			.size = 0,
			.data = NULL,
			.capacity = 0
		};
		return path;	
	}
	if (node->val == value){
		int* values = malloc(level+1);
		values[level] = node->val;
		VectN path = {
			.size = level+1,
			.data = values,
			.capacity = level+1
		};
		return path;
	}
	VectN left = dfs(node->left, value, level + 1);
	if (left.data != NULL){
		left.data[level] = node->val;
		return left;
	}
	VectN right = dfs(node->right, value, level + 1);
	if (right.data != NULL){
		right.data[level] = node->val;
		return right;
	}
	VectN path = {
		.size = 0,
		.data = NULL,
		.capacity = 0
	};
	return path;	
}
VectN findPath_DFS(BinaryNode* node, int value){
	return dfs(node, value, 0);
}


int main(){
	int arr[] = {1, 2, 3, 4, 5};
	BinaryNode* root = createBinaryTree(arr, 5);
	printf(" dfs traversal:- \n");
	printTree(root);
	printf("\n");
	
	// finding path to a specific value
	VectN path = findPath_DFS(root, 5);
	printf("path to 5:  ");
	if (path.data == NULL){
		printf("| No Path |\n");
	}
	for (int i=0; i<path.size; i++){
		printf("%i", path.data[i]);
		if (i < path.size - 1){
			printf(" -> ");
		}
	}
	printf("\n");
}