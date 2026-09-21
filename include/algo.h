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
void sort_users(User *items, int count, int ascending);
void sort_posts_by_id(Post *items, int count, int ascending);
void sort_posts_by_title(Post *items, int count, int ascending);
void sort_posts_by_date(Post *items, int count, int ascending);
void sort_events_by_id(Event *items, int count, int ascending);
void sort_events_by_name(Event *items, int count, int ascending);
void sort_events_by_date(Event *items, int count, int ascending);

#ifdef __cplusplus
}
#endif

#endif
