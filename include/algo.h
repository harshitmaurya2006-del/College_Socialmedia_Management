#ifndef ALGO_H
#define ALGO_H

#include "ds.h"

#ifdef __cplusplus
extern "C" {
#endif

int str_contains_ci(const char *text, const char *part);
int date_is_valid(const char *date);
PostNode *search_posts(PostNode *head, const char *keyword, int *outCount);
EventNode *search_events(EventNode *head, const char *keyword, int *outCount);
PostNode *binary_search_post(PostNode *head, int count, int id);
EventNode *binary_search_event(EventNode *head, int count, int id);
void sort_users(UserRec *items, int count, int ascending);
void sort_posts_by_id(PostRec *items, int count, int ascending);
void sort_posts_by_title(PostRec *items, int count, int ascending);
void sort_posts_by_date(PostRec *items, int count, int ascending);
void sort_events_by_id(EventRec *items, int count, int ascending);
void sort_events_by_name(EventRec *items, int count, int ascending);
void sort_events_by_date(EventRec *items, int count, int ascending);

#ifdef __cplusplus
}
#endif

#endif
