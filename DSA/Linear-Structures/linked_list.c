#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"


int main() {
    struct Node *head;
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8};
    head = makeLinkedList(nums,8, 0);
    trvrse(head);
    return 0;
}

struct Node* makeLinkedList(int list[], int length, int i){
    if ( length == 0 ){
        struct Node *node = malloc(sizeof(struct Node));
        node -> val = list[i];
        node -> next = NULL;
        return node;
    };
    
    if (i == length - 1){
        struct Node *node = malloc(sizeof(struct Node));
        node->val = list[i];
        node->next = NULL;
        return node;
    };
    
    struct Node *node = malloc(sizeof(struct Node));
    struct Node *ptrToNext;
    ptrToNext = makeLinkedList(list, length, i+1);
    node->val = list[i];
    node->next = ptrToNext;
    return node;
};

int trvrse(struct Node *head){
    struct Node current = *head;
    while (1==1){
        printf("%d->", current.val);
        if (current.next == NULL){
            break;
        };
        current = *current.next;
    };
    printf("null\n");
    return 0;
};