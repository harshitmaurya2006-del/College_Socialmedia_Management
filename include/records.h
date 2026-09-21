#ifndef RECORDS_H
#define RECORDS_H

#define NAME_SIZE 64
#define TEXT_SIZE 256
#define CATEGORY_SIZE 32
#define DATE_SIZE 11
#define TYPE_SIZE 16

#define ROLE_STUDENT 1
#define ROLE_FACULTY 2
#define ROLE_ADMIN 3

typedef struct {
    int id;
    char name[NAME_SIZE];
    int role;
    char password[NAME_SIZE];
    int year;
} User;

typedef struct {
    int id;
    char title[NAME_SIZE];
    char content[TEXT_SIZE];
    char category[CATEGORY_SIZE];
    char date[DATE_SIZE];
    int author_id;
} Post;

typedef struct {
    int id;
    char name[NAME_SIZE];
    char venue[NAME_SIZE];
    char date[DATE_SIZE];
    char description[TEXT_SIZE];
    int organizer_id;
} Event;

typedef struct {
    char type[TYPE_SIZE];
    int submitted_by;
    Post post;
    Event event;
} PendingRec;

typedef User UserRec;
typedef Post PostRec;
typedef Event EventRec;
typedef PendingRec ActionRec;

#endif
