#include "ds.h"
#include <stdlib.h>

/* Creates one user node from a record.
 * Parameters: the record to copy.
 * Returns: a node or NULL on allocation failure; O(1) time and space. */
UserNode *user_create_node(User value)
{
    UserNode *node = (UserNode *)malloc(sizeof(UserNode));
    if (node != NULL) {
        node->data = value;
        node->next = NULL;
    }
    return node;
}

/* Appends a user at the list tail.
 * Parameters: list head and record; returns 1 on success, 0 on failure.
 * Time is O(n), and extra space is O(1). */
int user_append(UserNode **head, User value)
{
    UserNode *node;
    UserNode *current;
    if (head == NULL) return 0;
    node = user_create_node(value);
    if (node == NULL) return 0;
    if (*head == NULL) { *head = node; return 1; }
    current = *head;
    while (current->next != NULL) current = current->next;
    current->next = node; /* Link the new node after the old tail. */
    return 1;
}

/* Finds a user by integer ID.
 * Parameters: list head and ID; returns the matching node or NULL.
 * Time is O(n), and extra space is O(1). */
UserNode *user_find(UserNode *head, int id)
{
    while (head != NULL) {
        if (head->data.id == id) return head;
        head = head->next;
    }
    return NULL;
}

/* Removes the first user with the given ID.
 * Parameters: list head and ID; returns 1 if removed, otherwise 0.
 * Time is O(n), and extra space is O(1). */
int user_remove(UserNode **head, int id)
{
    UserNode *current;
    UserNode *previous = NULL;
    if (head == NULL) return 0;
    current = *head;
    while (current != NULL) {
        if (current->data.id == id) {
            if (previous == NULL) *head = current->next;
            else previous->next = current->next; /* Unlink the node. */
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

/* Copies users into a newly allocated array in list order.
 * Parameters: list head and output count; returns array or NULL.
 * Time is O(n), and returned space is O(n). */
User *user_to_array(const UserNode *head, int *outCount)
{
    int count = 0;
    int index = 0;
    User *array;
    const UserNode *current;
    if (outCount == NULL) return NULL;
    for (current = head; current != NULL; current = current->next) ++count;
    *outCount = count;
    if (count == 0) return NULL;
    array = (User *)malloc((size_t)count * sizeof(User));
    if (array == NULL) { *outCount = 0; return NULL; }
    for (current = head; current != NULL; current = current->next) array[index++] = current->data;
    return array;
}

/* Frees every user node and clears the head.
 * Parameters: address of the list head; returns nothing.
 * Time is O(n), and extra space is O(1). */
void user_free_all(UserNode **head)
{
    UserNode *old;
    if (head == NULL) return;
    while (*head != NULL) {
        old = *head;
        *head = old->next; /* Advance before freeing the old head. */
        free(old);
    }
}

/* Creates one post node from a record.
 * Parameters: the record to copy.
 * Returns: a node or NULL; O(1) time and space. */
PostNode *post_create_node(Post value)
{
    PostNode *node = (PostNode *)malloc(sizeof(PostNode));
    if (node != NULL) { node->data = value; node->next = NULL; }
    return node;
}

/* Appends a post at the list tail.
 * Parameters: list head and record; returns 1 on success, 0 on failure.
 * Time is O(n), and extra space is O(1). */
int post_append(PostNode **head, Post value)
{
    PostNode *node;
    PostNode *current;
    if (head == NULL) return 0;
    node = post_create_node(value);
    if (node == NULL) return 0;
    if (*head == NULL) { *head = node; return 1; }
    current = *head;
    while (current->next != NULL) current = current->next;
    current->next = node; /* Append after the tail. */
    return 1;
}

/* Finds a post by integer ID.
 * Parameters: list head and ID; returns the matching node or NULL.
 * Time is O(n), and extra space is O(1). */
PostNode *post_find(PostNode *head, int id)
{
    while (head != NULL) {
        if (head->data.id == id) return head;
        head = head->next;
    }
    return NULL;
}

/* Removes the first post with the given ID.
 * Parameters: list head and ID; returns 1 if removed, otherwise 0.
 * Time is O(n), and extra space is O(1). */
int post_remove(PostNode **head, int id)
{
    PostNode *current;
    PostNode *previous = NULL;
    if (head == NULL) return 0;
    current = *head;
    while (current != NULL) {
        if (current->data.id == id) {
            if (previous == NULL) *head = current->next;
            else previous->next = current->next; /* Unlink current. */
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

/* Copies posts into a newly allocated array in list order.
 * Parameters: list head and output count; returns array or NULL.
 * Time is O(n), and returned space is O(n). */
Post *post_to_array(const PostNode *head, int *outCount)
{
    int count = 0;
    int index = 0;
    Post *array;
    const PostNode *current;
    if (outCount == NULL) return NULL;
    for (current = head; current != NULL; current = current->next) ++count;
    *outCount = count;
    if (count == 0) return NULL;
    array = (Post *)malloc((size_t)count * sizeof(Post));
    if (array == NULL) { *outCount = 0; return NULL; }
    for (current = head; current != NULL; current = current->next) array[index++] = current->data;
    return array;
}

/* Frees every post node and clears the head.
 * Parameters: address of the list head; returns nothing.
 * Time is O(n), and extra space is O(1). */
void post_free_all(PostNode **head)
{
    PostNode *old;
    if (head == NULL) return;
    while (*head != NULL) {
        old = *head;
        *head = old->next; /* Move the head before freeing. */
        free(old);
    }
}

/* Creates one event node from a record.
 * Parameters: the record to copy.
 * Returns: a node or NULL; O(1) time and space. */
EventNode *event_create_node(Event value)
{
    EventNode *node = (EventNode *)malloc(sizeof(EventNode));
    if (node != NULL) { node->data = value; node->next = NULL; }
    return node;
}

/* Appends an event at the list tail.
 * Parameters: list head and record; returns 1 on success, 0 on failure.
 * Time is O(n), and extra space is O(1). */
int event_append(EventNode **head, Event value)
{
    EventNode *node;
    EventNode *current;
    if (head == NULL) return 0;
    node = event_create_node(value);
    if (node == NULL) return 0;
    if (*head == NULL) { *head = node; return 1; }
    current = *head;
    while (current->next != NULL) current = current->next;
    current->next = node; /* Append after the tail. */
    return 1;
}

/* Finds an event by integer ID.
 * Parameters: list head and ID; returns the matching node or NULL.
 * Time is O(n), and extra space is O(1). */
EventNode *event_find(EventNode *head, int id)
{
    while (head != NULL) {
        if (head->data.id == id) return head;
        head = head->next;
    }
    return NULL;
}

/* Removes the first event with the given ID.
 * Parameters: list head and ID; returns 1 if removed, otherwise 0.
 * Time is O(n), and extra space is O(1). */
int event_remove(EventNode **head, int id)
{
    EventNode *current;
    EventNode *previous = NULL;
    if (head == NULL) return 0;
    current = *head;
    while (current != NULL) {
        if (current->data.id == id) {
            if (previous == NULL) *head = current->next;
            else previous->next = current->next; /* Unlink current. */
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

/* Copies events into a newly allocated array in list order.
 * Parameters: list head and output count; returns array or NULL.
 * Time is O(n), and returned space is O(n). */
Event *event_to_array(const EventNode *head, int *outCount)
{
    int count = 0;
    int index = 0;
    Event *array;
    const EventNode *current;
    if (outCount == NULL) return NULL;
    for (current = head; current != NULL; current = current->next) ++count;
    *outCount = count;
    if (count == 0) return NULL;
    array = (Event *)malloc((size_t)count * sizeof(Event));
    if (array == NULL) { *outCount = 0; return NULL; }
    for (current = head; current != NULL; current = current->next) array[index++] = current->data;
    return array;
}

/* Frees every event node and clears the head.
 * Parameters: address of the list head; returns nothing.
 * Time is O(n), and extra space is O(1). */
void event_free_all(EventNode **head)
{
    EventNode *old;
    if (head == NULL) return;
    while (*head != NULL) {
        old = *head;
        *head = old->next; /* Move the head before freeing. */
        free(old);
    }
}

/* Initializes an empty post queue.
 * Parameters: queue to initialize; returns nothing.
 * Time and extra space are O(1). */
void queue_init(PostQueue *queue)
{
    if (queue != NULL) { queue->front = NULL; queue->rear = NULL; queue->size = 0; }
}

/* Enqueues a post at the rear.
 * Parameters: queue and post; returns 1 on success, 0 on allocation failure.
 * Time is O(1), and extra space is O(1). */
int queue_enqueue(PostQueue *queue, Post value)
{
    PostNode *node;
    if (queue == NULL) return 0;
    node = post_create_node(value);
    if (node == NULL) return 0;
    if (queue->rear == NULL) queue->front = node;
    else queue->rear->next = node; /* Link after the old rear. */
    queue->rear = node;
    ++queue->size;
    return 1;
}

/* Removes a post from the front.
 * Parameters: queue and output record; returns 1 if removed, otherwise 0.
 * Time is O(1), and extra space is O(1). */
int queue_dequeue(PostQueue *queue, Post *outValue)
{
    PostNode *old;
    if (queue == NULL || outValue == NULL || queue->front == NULL) return 0;
    old = queue->front;
    *outValue = old->data;
    queue->front = old->next; /* Advance the front link. */
    if (queue->front == NULL) queue->rear = NULL;
    free(old);
    --queue->size;
    return 1;
}

/* Copies queue records from front to rear.
 * Parameters: queue and output count; returns array or NULL.
 * Time is O(n), and returned space is O(n). */
Post *queue_to_array(const PostQueue *queue, int *outCount)
{
    int index = 0;
    Post *array;
    const PostNode *current;
    if (outCount == NULL) return NULL;
    *outCount = queue == NULL ? 0 : queue->size;
    if (queue == NULL || queue->size == 0) return NULL;
    array = (Post *)malloc((size_t)queue->size * sizeof(Post));
    if (array == NULL) { *outCount = 0; return NULL; }
    for (current = queue->front; current != NULL; current = current->next) array[index++] = current->data;
    return array;
}

/* Frees all post queue nodes and resets its links.
 * Parameters: queue to clear; returns nothing.
 * Time is O(n), and extra space is O(1). */
void queue_free_all(PostQueue *queue)
{
    Post value;
    if (queue == NULL) return;
    while (queue_dequeue(queue, &value)) { }
}

/* Initializes an empty pending-record queue.
 * Parameters: queue to initialize; returns nothing.
 * Time and extra space are O(1). */
void pending_queue_init(PendingQueue *queue)
{
    if (queue != NULL) { queue->front = NULL; queue->rear = NULL; queue->size = 0; }
}

/* Enqueues a pending record at the rear.
 * Parameters: queue and record; returns 1 on success, 0 on failure.
 * Time is O(1), and extra space is O(1). */
int pending_enqueue(PendingQueue *queue, PendingRec value)
{
    PendingNode *node;
    if (queue == NULL) return 0;
    node = (PendingNode *)malloc(sizeof(PendingNode));
    if (node == NULL) return 0;
    node->data = value;
    node->next = NULL;
    if (queue->rear == NULL) queue->front = node;
    else queue->rear->next = node; /* Link after the rear. */
    queue->rear = node;
    ++queue->size;
    return 1;
}

/* Removes one pending record from the front.
 * Parameters: queue and output record; returns 1 if removed, otherwise 0.
 * Time is O(1), and extra space is O(1). */
int pending_dequeue(PendingQueue *queue, PendingRec *outValue)
{
    PendingNode *old;
    if (queue == NULL || outValue == NULL || queue->front == NULL) return 0;
    old = queue->front;
    *outValue = old->data;
    queue->front = old->next; /* Advance the front pointer. */
    if (queue->front == NULL) queue->rear = NULL;
    free(old);
    --queue->size;
    return 1;
}

/* Copies pending records from front to rear.
 * Parameters: queue and output count; returns array or NULL.
 * Time is O(n), and returned space is O(n). */
PendingRec *pending_queue_to_array(const PendingQueue *queue, int *outCount)
{
    int index = 0;
    PendingRec *array;
    const PendingNode *current;
    if (outCount == NULL) return NULL;
    *outCount = queue == NULL ? 0 : queue->size;
    if (queue == NULL || queue->size == 0) return NULL;
    array = (PendingRec *)malloc((size_t)queue->size * sizeof(PendingRec));
    if (array == NULL) { *outCount = 0; return NULL; }
    for (current = queue->front; current != NULL; current = current->next) array[index++] = current->data;
    return array;
}

/* Frees every pending queue node and resets the queue.
 * Parameters: queue to clear; returns nothing.
 * Time is O(n), and extra space is O(1). */
void pending_queue_free_all(PendingQueue *queue)
{
    PendingRec value;
    if (queue == NULL) return;
    while (pending_dequeue(queue, &value)) { }
}

/* Initializes a fixed-capacity pending stack.
 * Parameters: stack and capacity; returns nothing.
 * Time is O(1), and allocated space is O(capacity). */
void stack_init(PendingStack *stack, int capacity)
{
    if (stack == NULL) return;
    stack->items = NULL;
    stack->top = -1;
    stack->capacity = capacity > 0 ? capacity : 0;
    if (stack->capacity > 0) stack->items = (PendingRec *)malloc((size_t)stack->capacity * sizeof(PendingRec));
    if (stack->items == NULL) stack->capacity = 0;
}

/* Pushes a record onto the stack.
 * Parameters: stack and record; returns 1 on success, 0 if full or invalid.
 * Time is O(1), and extra space is O(1). */
int stack_push(PendingStack *stack, PendingRec value)
{
    if (stack == NULL || stack->items == NULL || stack->top + 1 >= stack->capacity) return 0;
    ++stack->top;
    stack->items[stack->top] = value; /* Place the new top record. */
    return 1;
}

/* Pops the top stack record.
 * Parameters: stack and output record; returns 1 if popped, otherwise 0.
 * Time is O(1), and extra space is O(1). */
int stack_pop(PendingStack *stack, PendingRec *outValue)
{
    if (stack == NULL || outValue == NULL || stack->top < 0) return 0;
    *outValue = stack->items[stack->top];
    --stack->top;
    return 1;
}

/* Copies stack records from top to bottom.
 * Parameters: stack and output count; returns array or NULL.
 * Time is O(n), and returned space is O(n). */
PendingRec *stack_to_array(const PendingStack *stack, int *outCount)
{
    int index;
    PendingRec *array;
    if (outCount == NULL) return NULL;
    *outCount = (stack == NULL || stack->top < 0) ? 0 : stack->top + 1;
    if (stack == NULL || stack->top < 0) return NULL;
    array = (PendingRec *)malloc((size_t)(stack->top + 1) * sizeof(PendingRec));
    if (array == NULL) { *outCount = 0; return NULL; }
    for (index = 0; index <= stack->top; ++index) array[index] = stack->items[stack->top - index];
    return array;
}

/* Frees the stack array and resets its fields.
 * Parameters: stack to clear; returns nothing.
 * Time is O(1), and extra space is O(1). */
void stack_free_all(PendingStack *stack)
{
    if (stack == NULL) return;
    free(stack->items);
    stack->items = NULL;
    stack->top = -1;
    stack->capacity = 0;
}
