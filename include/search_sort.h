#ifndef SEARCH_SORT_H
#define SEARCH_SORT_H

#include "c_structures.h"

void sort_posts_by_id(PostNode *head);
void sort_posts_by_title(PostNode *head);
void sort_posts_by_date(PostNode *head);
void sort_events_by_id(EventNode *head);
void sort_events_by_name(EventNode *head);
void sort_events_by_date(EventNode *head);
PostNode *binary_search_post_by_id(PostNode *head, int count, const char *id);
EventNode *binary_search_event_by_id(EventNode *head, int count, const char *id);
int post_count(PostNode *head);
int event_count(EventNode *head);

#endif
