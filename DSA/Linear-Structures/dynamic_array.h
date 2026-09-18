#ifndef VECTOR_H
#define VECTOR_H

typedef struct vectN{
    int* data;
    int size;
    int capacity;
} VectN;

VectN new_VectN(int arr[], int n);
void printVectN(VectN nums);
void appendTo(VectN* nums, int e);
int pop(VectN* nums, int index);

#endif
