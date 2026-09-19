#include "algo.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Compares two strings without considering letter case.
 * Parameters: two strings; returns negative, zero or positive by order.
 * Time is O(n), and extra space is O(1). */
static int compare_ci(const char *left, const char *right)
{
    unsigned char left_char;
    unsigned char right_char;
    while (*left != '\0' && *right != '\0') {
        left_char = (unsigned char)tolower((unsigned char)*left);
        right_char = (unsigned char)tolower((unsigned char)*right);
        if (left_char != right_char) return (int)left_char - (int)right_char;
        ++left;
        ++right;
    }
    return (int)(unsigned char)*left - (int)(unsigned char)*right;
}

/* Checks whether part occurs in text without considering letter case.
 * Parameters: two NUL-terminated strings; returns 1 for a match, else 0.
 * Time is O(n*m) in the simple worst case, and extra space is O(1). */
int str_contains_ci(const char *text, const char *part)
{
    size_t i;
    size_t j;
    if (text == NULL || part == NULL) return 0;
    if (*part == '\0') return 1;
    for (i = 0; text[i] != '\0'; ++i) {
        for (j = 0; part[j] != '\0' && text[i + j] != '\0'; ++j) {
            if (tolower((unsigned char)text[i + j]) != tolower((unsigned char)part[j])) break;
        }
        if (part[j] == '\0') return 1;
    }
    return 0;
}

/* Validates an exact YYYY-MM-DD calendar date.
 * Parameters: date string; returns 1 when valid, otherwise 0.
 * Time and extra space are O(1). */
int date_is_valid(const char *date)
{
    int year;
    int month;
    int day;
    int days;
    int leap;
    int index;
    if (date == NULL || strlen(date) != 10 || date[4] != '-' || date[7] != '-') return 0;
    for (index = 0; index < 10; ++index) {
        if (index != 4 && index != 7 && !isdigit((unsigned char)date[index])) return 0;
    }
    year = (date[0] - '0') * 1000 + (date[1] - '0') * 100 + (date[2] - '0') * 10 + date[3] - '0';
    month = (date[5] - '0') * 10 + date[6] - '0';
    day = (date[8] - '0') * 10 + date[9] - '0';
    if (year < 1900 || year > 2100 || month < 1 || month > 12) return 0;
    days = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) days = 30;
    if (month == 2) {
        leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
        days = leap ? 29 : 28;
    }
    return day >= 1 && day <= days;
}

/* Returns matching post copies in list order.
 * Parameters: source list, keyword, output count; NULL means no matches/failure.
 * Time is O(n*m), and returned space is O(k) for k matches. */
PostNode *search_posts(PostNode *head, const char *keyword, int *outCount)
{
    PostNode *result = NULL;
    PostNode *tail = NULL;
    PostNode *node;
    int count = 0;
    if (outCount == NULL) return NULL;
    *outCount = 0;
    if (keyword == NULL) return NULL;
    while (head != NULL) {
        if (str_contains_ci(head->data.title, keyword) || str_contains_ci(head->data.content, keyword) || str_contains_ci(head->data.category, keyword)) {
            node = post_create_node(head->data);
            if (node == NULL) { post_free_all(&result); return NULL; }
            if (tail == NULL) result = node;
            else tail->next = node; /* Add the match after the result tail. */
            tail = node;
            ++count;
        }
        head = head->next;
    }
    *outCount = count;
    return result;
}

/* Returns matching event copies in list order.
 * Parameters: source list, keyword, output count; NULL means no matches/failure.
 * Time is O(n*m), and returned space is O(k) for k matches. */
EventNode *search_events(EventNode *head, const char *keyword, int *outCount)
{
    EventNode *result = NULL;
    EventNode *tail = NULL;
    EventNode *node;
    int count = 0;
    if (outCount == NULL) return NULL;
    *outCount = 0;
    if (keyword == NULL) return NULL;
    while (head != NULL) {
        if (str_contains_ci(head->data.name, keyword) || str_contains_ci(head->data.venue, keyword) || str_contains_ci(head->data.description, keyword)) {
            node = event_create_node(head->data);
            if (node == NULL) { event_free_all(&result); return NULL; }
            if (tail == NULL) result = node;
            else tail->next = node; /* Add the match after the result tail. */
            tail = node;
            ++count;
        }
        head = head->next;
    }
    *outCount = count;
    return result;
}

/* Performs iterative binary search on an ID-sorted post list.
 * Parameters: sorted head, node count and ID; returns a matching node or NULL.
 * Time is O(log n) comparisons but node access makes the list traversal O(n log n). */
PostNode *binary_search_post(PostNode *head, int count, int id)
{
    int low = 0;
    int high = count - 1;
    int middle;
    int index;
    PostNode *current;
    while (low <= high) {
        middle = low + (high - low) / 2;
        current = head;
        for (index = 0; index < middle && current != NULL; ++index) current = current->next;
        if (current == NULL) return NULL;
        if (current->data.id == id) return current;
        if (current->data.id < id) low = middle + 1;
        else high = middle - 1;
    }
    return NULL;
}

/* Performs iterative binary search on an ID-sorted event list.
 * Parameters: sorted head, node count and ID; returns a matching node or NULL.
 * Time is O(log n) comparisons but node access makes the list traversal O(n log n). */
EventNode *binary_search_event(EventNode *head, int count, int id)
{
    int low = 0;
    int high = count - 1;
    int middle;
    int index;
    EventNode *current;
    while (low <= high) {
        middle = low + (high - low) / 2;
        current = head;
        for (index = 0; index < middle && current != NULL; ++index) current = current->next;
        if (current == NULL) return NULL;
        if (current->data.id == id) return current;
        if (current->data.id < id) low = middle + 1;
        else high = middle - 1;
    }
    return NULL;
}

/* Orders users by ID using insertion sort.
 * Parameters: array, count and direction; returns nothing.
 * Time is O(n^2), and extra space is O(1). */
void sort_users(User *items, int count, int ascending)
{
    int i;
    int j;
    User value;
    if (items == NULL) return;
    for (i = 1; i < count; ++i) {
        value = items[i];
        j = i - 1;
        while (j >= 0 && ((ascending && items[j].id > value.id) || (!ascending && items[j].id < value.id))) {
            items[j + 1] = items[j];
            --j;
        }
        items[j + 1] = value;
    }
}

/* Orders posts by ID, with ID ascending as the tie-breaker.
 * Parameters: array, count and direction; returns nothing.
 * Time is O(n^2), and extra space is O(1). */
void sort_posts_by_id(Post *items, int count, int ascending)
{
    int i;
    int j;
    Post value;
    if (items == NULL) return;
    for (i = 1; i < count; ++i) {
        value = items[i]; j = i - 1;
        while (j >= 0 && ((ascending && items[j].id > value.id) || (!ascending && items[j].id < value.id))) { items[j + 1] = items[j]; --j; }
        items[j + 1] = value;
    }
}

/* Orders posts by case-insensitive title, then ID ascending.
 * Parameters: array, count and direction; returns nothing.
 * Time is O(n^2), and extra space is O(1). */
void sort_posts_by_title(Post *items, int count, int ascending)
{
    int i;
    int j;
    int comparison;
    Post value;
    if (items == NULL) return;
    for (i = 1; i < count; ++i) {
        value = items[i]; j = i - 1;
        while (j >= 0) {
            comparison = compare_ci(items[j].title, value.title);
            if (comparison == 0) comparison = items[j].id - value.id;
            if (!((ascending && comparison > 0) || (!ascending && comparison < 0))) break;
            items[j + 1] = items[j]; --j;
        }
        items[j + 1] = value;
    }
}

/* Orders posts by YYYY-MM-DD text, then ID ascending.
 * Parameters: array, count and direction; returns nothing.
 * Time is O(n^2), and extra space is O(1); fixed-width dates sort chronologically as text. */
void sort_posts_by_date(Post *items, int count, int ascending)
{
    int i;
    int j;
    int comparison;
    Post value;
    if (items == NULL) return;
    for (i = 1; i < count; ++i) {
        value = items[i]; j = i - 1;
        while (j >= 0) {
            comparison = strcmp(items[j].date, value.date);
            if (comparison == 0) comparison = items[j].id - value.id;
            if (!((ascending && comparison > 0) || (!ascending && comparison < 0))) break;
            items[j + 1] = items[j]; --j;
        }
        items[j + 1] = value;
    }
}

/* Orders events by ID using insertion sort.
 * Parameters: array, count and direction; returns nothing.
 * Time is O(n^2), and extra space is O(1). */
void sort_events_by_id(Event *items, int count, int ascending)
{
    int i; int j; Event value;
    if (items == NULL) return;
    for (i = 1; i < count; ++i) { value = items[i]; j = i - 1; while (j >= 0 && ((ascending && items[j].id > value.id) || (!ascending && items[j].id < value.id))) { items[j + 1] = items[j]; --j; } items[j + 1] = value; }
}

/* Orders events by case-insensitive name, then ID ascending.
 * Parameters: array, count and direction; returns nothing.
 * Time is O(n^2), and extra space is O(1). */
void sort_events_by_name(Event *items, int count, int ascending)
{
    int i; int j; int comparison; Event value;
    if (items == NULL) return;
    for (i = 1; i < count; ++i) {
        value = items[i]; j = i - 1;
        while (j >= 0) { comparison = compare_ci(items[j].name, value.name); if (comparison == 0) comparison = items[j].id - value.id; if (!((ascending && comparison > 0) || (!ascending && comparison < 0))) break; items[j + 1] = items[j]; --j; }
        items[j + 1] = value;
    }
}

/* Orders events by date text, then ID ascending.
 * Parameters: array, count and direction; returns nothing.
 * Time is O(n^2), and extra space is O(1); YYYY-MM-DD text is chronological. */
void sort_events_by_date(Event *items, int count, int ascending)
{
    int i; int j; int comparison; Event value;
    if (items == NULL) return;
    for (i = 1; i < count; ++i) {
        value = items[i]; j = i - 1;
        while (j >= 0) { comparison = strcmp(items[j].date, value.date); if (comparison == 0) comparison = items[j].id - value.id; if (!((ascending && comparison > 0) || (!ascending && comparison < 0))) break; items[j + 1] = items[j]; --j; }
        items[j + 1] = value;
    }
}
