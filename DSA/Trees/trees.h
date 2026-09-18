#ifndef TREE_H
#define TREE_H

// binary tree node
typedef struct BinaryNode
{
	int val;
	struct BinaryNode *parent;
	struct BinaryNode *left;
	struct BinaryNode *right;
} BinaryNode;

// general tree node
typedef struct TreeNode{
	struct TreeNode *parent;
	struct TreeNode **children;
}TreeNode;


BinaryNode *createBinaryTree(int arr[],int n);
BinaryNode *createBinaryTree_r(int arr[], int start, int end, BinaryNode *parent);
void freeBinaryTree(BinaryNode *root);

#endif