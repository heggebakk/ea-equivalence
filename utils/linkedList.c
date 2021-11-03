#include "linkedList.h"

void displayLinkedList(struct Node *head) {
    struct Node *current = head;
    if (head == NULL) {
        printf("Linked list is empty. \n");
        return;
    }
    printf("Nodes of linked list: ");
    while (current != NULL) {
        printf("%zu ", current->data);
        current = current->next;
    }
    printf("\n");
}

size_t countNodes(struct Node *head) {
    size_t count = 0;
    struct Node *current = head;

    while (current != NULL) {
        count += 1;
        current = current->next;
    }

    return count;
}
