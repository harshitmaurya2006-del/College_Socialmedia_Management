#ifndef DS_H
#define DS_H

#include "records.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UserNode { UserRec data; struct UserNode *next; } UserNode;
typedef struct PostNode { PostRec data; struct PostNode *next; } PostNode;
typedef struct EventNode { EventRec data; struct EventNode *next; } EventNode;

typedef struct { PostNode *front; PostNode *rear; int size; } PostQueue;
typedef struct PendingNode { PendingRec data; struct PendingNode *next; } PendingNode;
typedef struct { PendingNode *front; PendingNode *rear; int size; } PendingQueue;
typedef struct { PendingRec *items; int top; int capacity; } PendingStack;

UserNode *user_create_node(UserRec value);
int user_append(UserNode **head, UserRec value);
UserNode *user_find(UserNode *head, int id);
int user_remove(UserNode **head, int id);
UserRec *user_to_array(const UserNode *head, int *outCount);
void user_free_all(UserNode **head);

PostNode *post_create_node(PostRec value);
int post_append(PostNode **head, PostRec value);
PostNode *post_find(PostNode *head, int id);
int post_remove(PostNode **head, int id);
PostRec *post_to_array(const PostNode *head, int *outCount);
void post_free_all(PostNode **head);

EventNode *event_create_node(EventRec value);
int event_append(EventNode **head, EventRec value);
EventNode *event_find(EventNode *head, int id);
int event_remove(EventNode **head, int id);
EventRec *event_to_array(const EventNode *head, int *outCount);
void event_free_all(EventNode **head);

void queue_init(PostQueue *queue);
int queue_enqueue(PostQueue *queue, PostRec value);
int queue_dequeue(PostQueue *queue, PostRec *outValue);
PostRec *queue_to_array(const PostQueue *queue, int *outCount);
void queue_free_all(PostQueue *queue);

void pending_queue_init(PendingQueue *queue);
int pending_enqueue(PendingQueue *queue, PendingRec value);
int pending_dequeue(PendingQueue *queue, PendingRec *outValue);
PendingRec *pending_queue_to_array(const PendingQueue *queue, int *outCount);
void pending_queue_free_all(PendingQueue *queue);

void stack_init(PendingStack *stack, int capacity);
int stack_push(PendingStack *stack, PendingRec value);
int stack_pop(PendingStack *stack, PendingRec *outValue);
PendingRec *stack_to_array(const PendingStack *stack, int *outCount);
void stack_free_all(PendingStack *stack);

#ifdef __cplusplus
}
#endif

#endif
