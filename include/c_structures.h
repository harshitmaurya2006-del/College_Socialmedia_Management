#ifndef C_STRUCTURES_H
#define C_STRUCTURES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char studentID[20];
    char name[60];
    char branch[30];
    int semester;
} CStudent;

typedef struct {
    char postID[20];
    char title[100];
    char content[240];
    char category[40];
    char date[20];
    char authorID[20];
} CPost;

typedef struct {
    char eventID[20];
    char eventName[100];
    char venue[80];
    char date[20];
    char description[240];
} CEvent;

typedef struct StudentNode {
    CStudent data;
    struct StudentNode *next;
} StudentNode;

typedef struct PostNode {
    CPost data;
    struct PostNode *next;
} PostNode;

typedef struct EventNode {
    CEvent data;
    struct EventNode *next;
} EventNode;

StudentNode *student_create_node(CStudent data);
void student_append(StudentNode **head, CStudent data);
StudentNode *student_find(StudentNode *head, const char *id);
int student_remove(StudentNode **head, const char *id);
void student_free_all(StudentNode **head);

PostNode *post_create_node(CPost data);
void post_append(PostNode **head, CPost data);
PostNode *post_find(PostNode *head, const char *id);
int post_remove(PostNode **head, const char *id);
void post_free_all(PostNode **head);

EventNode *event_create_node(CEvent data);
void event_append(EventNode **head, CEvent data);
EventNode *event_find(EventNode *head, const char *id);
int event_remove(EventNode **head, const char *id);
void event_free_all(EventNode **head);

typedef struct {
    char action[160];
    int top;
    int capacity;
    char **items;
} ActionStack;

void stack_init(ActionStack *stack, int capacity);
void stack_push(ActionStack *stack, const char *action);
int stack_pop(ActionStack *stack, char *output, int output_size);
void stack_free(ActionStack *stack);

typedef struct {
    char recordType[20];
    char recordID[20];
} ApprovalItem;

typedef struct ApprovalNode {
    ApprovalItem data;
    struct ApprovalNode *next;
} ApprovalNode;

typedef struct {
    ApprovalNode *front;
    ApprovalNode *rear;
} ApprovalQueue;

void queue_init(ApprovalQueue *queue);
void queue_enqueue(ApprovalQueue *queue, ApprovalItem item);
int queue_dequeue(ApprovalQueue *queue, ApprovalItem *output);
int queue_is_empty(const ApprovalQueue *queue);
void queue_free(ApprovalQueue *queue);

#ifdef __cplusplus
}
#endif

#endif
