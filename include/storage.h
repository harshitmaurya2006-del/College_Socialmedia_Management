#ifndef STORAGE_H
#define STORAGE_H

#include "ds.h"

#ifdef __cplusplus
extern "C" {
#endif

int users_load(const char *filename, UserNode **head);
int users_save(const char *filename, const UserNode *head);
int posts_load(const char *filename, PostNode **head);
int posts_save(const char *filename, const PostNode *head);
int events_load(const char *filename, EventNode **head);
int events_save(const char *filename, const EventNode *head);
int pending_load(const char *filename, PendingQueue *queue);
int pending_save(const char *filename, const PendingQueue *queue);

#ifdef __cplusplus
}
#endif

#endif
