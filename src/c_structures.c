#include "c_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAKE_NODE(kind, type, value) \
    kind##Node *node = (kind##Node *)malloc(sizeof(kind##Node)); \
    if (node != NULL) { node->data = value; node->next = NULL; } \
    return node;

StudentNode *student_create_node(CStudent data) { MAKE_NODE(Student, CStudent, data) }
void student_append(StudentNode **head, CStudent data) {
    StudentNode *node = student_create_node(data); if (!node) return;
    if (!*head) { *head = node; return; }
    StudentNode *current = *head; while (current->next) current = current->next; current->next = node;
}
StudentNode *student_find(StudentNode *head, const char *id) {
    while (head) { if (strcmp(head->data.studentID, id) == 0) return head; head = head->next; } return NULL;
}
int student_remove(StudentNode **head, const char *id) {
    StudentNode *current = *head, *previous = NULL;
    while (current) { if (strcmp(current->data.studentID, id) == 0) { if (previous) previous->next = current->next; else *head = current->next; free(current); return 1; } previous = current; current = current->next; } return 0;
}
void student_free_all(StudentNode **head) { while (*head) { StudentNode *old = *head; *head = old->next; free(old); } }

PostNode *post_create_node(CPost data) { MAKE_NODE(Post, CPost, data) }
void post_append(PostNode **head, CPost data) {
    PostNode *node = post_create_node(data); if (!node) return;
    if (!*head) { *head = node; return; }
    PostNode *current = *head; while (current->next) current = current->next; current->next = node;
}
PostNode *post_find(PostNode *head, const char *id) {
    while (head) { if (strcmp(head->data.postID, id) == 0) return head; head = head->next; } return NULL;
}
int post_remove(PostNode **head, const char *id) {
    PostNode *current = *head, *previous = NULL;
    while (current) { if (strcmp(current->data.postID, id) == 0) { if (previous) previous->next = current->next; else *head = current->next; free(current); return 1; } previous = current; current = current->next; } return 0;
}
void post_free_all(PostNode **head) { while (*head) { PostNode *old = *head; *head = old->next; free(old); } }

EventNode *event_create_node(CEvent data) { MAKE_NODE(Event, CEvent, data) }
void event_append(EventNode **head, CEvent data) {
    EventNode *node = event_create_node(data); if (!node) return;
    if (!*head) { *head = node; return; }
    EventNode *current = *head; while (current->next) current = current->next; current->next = node;
}
EventNode *event_find(EventNode *head, const char *id) {
    while (head) { if (strcmp(head->data.eventID, id) == 0) return head; head = head->next; } return NULL;
}
int event_remove(EventNode **head, const char *id) {
    EventNode *current = *head, *previous = NULL;
    while (current) { if (strcmp(current->data.eventID, id) == 0) { if (previous) previous->next = current->next; else *head = current->next; free(current); return 1; } previous = current; current = current->next; } return 0;
}
void event_free_all(EventNode **head) { while (*head) { EventNode *old = *head; *head = old->next; free(old); } }

void stack_init(ActionStack *stack, int capacity) { stack->top = -1; stack->capacity = capacity; stack->items = (char **)calloc((size_t)capacity, sizeof(char *)); }
void stack_push(ActionStack *stack, const char *action) {
    if (stack->top + 1 >= stack->capacity) { free(stack->items[0]); memmove(stack->items, stack->items + 1, (size_t)(stack->capacity - 1) * sizeof(char *)); stack->top--; }
    stack->items[++stack->top] = (char *)malloc(strlen(action) + 1); if (stack->items[stack->top]) strcpy(stack->items[stack->top], action);
}
int stack_pop(ActionStack *stack, char *output, int output_size) {
    if (stack->top < 0) return 0; strncpy(output, stack->items[stack->top], (size_t)output_size - 1); output[output_size - 1] = '\0'; free(stack->items[stack->top--]); return 1;
}
void stack_free(ActionStack *stack) { while (stack->top >= 0) free(stack->items[stack->top--]); free(stack->items); stack->items = NULL; }

void queue_init(ApprovalQueue *queue) { queue->front = NULL; queue->rear = NULL; }
void queue_enqueue(ApprovalQueue *queue, ApprovalItem item) {
    ApprovalNode *node = (ApprovalNode *)malloc(sizeof(ApprovalNode)); if (!node) return; node->data = item; node->next = NULL;
    if (!queue->rear) queue->front = queue->rear = node; else { queue->rear->next = node; queue->rear = node; }
}
int queue_dequeue(ApprovalQueue *queue, ApprovalItem *output) {
    if (!queue->front) return 0; ApprovalNode *old = queue->front; *output = old->data; queue->front = old->next; if (!queue->front) queue->rear = NULL; free(old); return 1;
}
int queue_is_empty(const ApprovalQueue *queue) { return queue->front == NULL; }
void queue_free(ApprovalQueue *queue) { ApprovalItem item; while (queue_dequeue(queue, &item)) {} }
