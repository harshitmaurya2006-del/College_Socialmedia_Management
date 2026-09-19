#include "search_sort.h"
#include <algorithm>
#include <cstring>
#include <vector>

static void swap_post(PostNode *a, PostNode *b) { CPost temp = a->data; a->data = b->data; b->data = temp; }
static void swap_event(EventNode *a, EventNode *b) { CEvent temp = a->data; a->data = b->data; b->data = temp; }
static void sort_posts(PostNode *head, int mode) {
    for (PostNode *i = head; i; i = i->next) for (PostNode *j = i->next; j; j = j->next) {
        int result = mode == 0 ? strcmp(i->data.postID, j->data.postID) : mode == 1 ? strcmp(i->data.title, j->data.title) : strcmp(i->data.date, j->data.date);
        if (result > 0) swap_post(i, j);
    }
}
static void sort_events(EventNode *head, int mode) {
    for (EventNode *i = head; i; i = i->next) for (EventNode *j = i->next; j; j = j->next) {
        int result = mode == 0 ? strcmp(i->data.eventID, j->data.eventID) : mode == 1 ? strcmp(i->data.eventName, j->data.eventName) : strcmp(i->data.date, j->data.date);
        if (result > 0) swap_event(i, j);
    }
}
void sort_posts_by_id(PostNode *head) { sort_posts(head, 0); }
void sort_posts_by_title(PostNode *head) { sort_posts(head, 1); }
void sort_posts_by_date(PostNode *head) { sort_posts(head, 2); }
void sort_events_by_id(EventNode *head) { sort_events(head, 0); }
void sort_events_by_name(EventNode *head) { sort_events(head, 1); }
void sort_events_by_date(EventNode *head) { sort_events(head, 2); }
int post_count(PostNode *head) { int count = 0; while (head) { ++count; head = head->next; } return count; }
int event_count(EventNode *head) { int count = 0; while (head) { ++count; head = head->next; } return count; }

PostNode *binary_search_post_by_id(PostNode *head, int count, const char *id) {
    std::vector<PostNode *> nodes; for (PostNode *p = head; p; p = p->next) nodes.push_back(p);
    int low = 0, high = count - 1; while (low <= high) { int middle = (low + high) / 2; int result = strcmp(nodes[middle]->data.postID, id); if (result == 0) return nodes[middle]; if (result < 0) low = middle + 1; else high = middle - 1; } return nullptr;
}
EventNode *binary_search_event_by_id(EventNode *head, int count, const char *id) {
    std::vector<EventNode *> nodes; for (EventNode *e = head; e; e = e->next) nodes.push_back(e);
    int low = 0, high = count - 1; while (low <= high) { int middle = (low + high) / 2; int result = strcmp(nodes[middle]->data.eventID, id); if (result == 0) return nodes[middle]; if (result < 0) low = middle + 1; else high = middle - 1; } return nullptr;
}
