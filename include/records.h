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

typedef struct UserRec {
    int id;
    char name[NAME_SIZE];
    int role;
    char password[NAME_SIZE];
    int year;
    char branch[NAME_SIZE];
    char department[NAME_SIZE];
} UserRec;
#ifndef __cplusplus
typedef UserRec User;
#endif

typedef struct PostRec {
    int id;
    char title[NAME_SIZE];
    char content[TEXT_SIZE];
    char category[CATEGORY_SIZE];
    char date[DATE_SIZE];
    int author_id;
} PostRec;
#ifndef __cplusplus
typedef PostRec Post;
#endif

typedef struct EventRec {
    int id;
    char name[NAME_SIZE];
    char venue[NAME_SIZE];
    char date[DATE_SIZE];
    char description[TEXT_SIZE];
    int organizer_id;
} EventRec;
#ifndef __cplusplus
typedef EventRec Event;
#endif

typedef struct {
    char type[TYPE_SIZE];
    int submitted_by;
    PostRec post;
    EventRec event;
} PendingRec;

typedef PendingRec ActionRec;

#endif
