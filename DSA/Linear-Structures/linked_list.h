#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct Node{
    int val;
    struct Node *next;
};

struct Node* makeLinkedList(int list[], int length, int i);
int trvrse(struct Node *head);

#endif