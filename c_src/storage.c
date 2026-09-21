#include "storage.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 1024
#define MAX_FIELDS 8

/* Removes line terminators from a read buffer.
 * Parameters: mutable line; returns nothing.
 * Time is O(n), and extra space is O(1). */
static void strip_line_end(char *line)
{
    size_t length;
    if (line == NULL) return;
    length = strlen(line);
    while (length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r')) line[--length] = '\0';
}

/* Splits fields without losing empty fields between delimiters.
 * Parameters: line, output field array and capacity; returns field count.
 * Time is O(n), and extra space is O(1) besides output pointers. */
static int split_fields(char *line, char **fields, int capacity)
{
    int count = 0;
    char *start = line;
    char *current;
    if (line == NULL || fields == NULL || capacity <= 0) return 0;
    while (count < capacity) {
        fields[count++] = start;
        current = strchr(start, '|');
        if (current == NULL) break;
        *current = '\0';
        start = current + 1;
        if (*start == '\0' && count < capacity) fields[count++] = start;
        if (current[1] == '\0') break;
    }
    return count;
}

/* Parses a complete integer field and rejects trailing characters.
 * Parameters: text and output integer; returns 1 if valid, otherwise 0.
 * Time and extra space are O(1). */
static int parse_int(const char *text, int *value)
{
    char *end;
    long number;
    if (text == NULL || value == NULL || *text == '\0') return 0;
    errno = 0;
    number = strtol(text, &end, 10);
    if (errno == ERANGE || end == text || *end != '\0' || number < -2147483647L - 1L || number > 2147483647L) return 0;
    *value = (int)number;
    return 1;
}

/* Copies a text field safely into a fixed-size destination.
 * Parameters: destination, capacity and source; returns nothing.
 * Time is O(n), and extra space is O(1). */
static void copy_text(char *destination, size_t capacity, const char *source)
{
    if (capacity == 0) return;
    if (source == NULL) source = "";
    strncpy(destination, source, capacity - 1U);
    destination[capacity - 1U] = '\0';
}

/* Writes a field while replacing format-breaking characters with spaces.
 * Parameters: stream and text field; returns nothing.
 * Time is O(n), and extra space is O(1). */
static void write_safe_field(FILE *file, const char *text)
{
    const char *current;
    if (text == NULL) return;
    for (current = text; *current != '\0'; ++current) {
        if (*current == '|' || *current == '\n' || *current == '\r') fputc(' ', file);
        else fputc((unsigned char)*current, file);
    }
}

/* Loads users from id|name|role|password|year lines.
 * Parameters: filename and list head; returns records loaded, or -1 on I/O/error.
 * Time is O(L*n), and extra space is O(1) besides list nodes. */
int users_load(const char *filename, UserNode **head)
{
    FILE *file;
    char line[LINE_SIZE];
    char *fields[MAX_FIELDS];
    int loaded = 0;
    int count;
    User value;
    if (filename == NULL || head == NULL) return -1;
    file = fopen(filename, "r");
    if (file == NULL) return errno == ENOENT ? 0 : -1;
    while (fgets(line, sizeof(line), file) != NULL) {
        strip_line_end(line);
        count = split_fields(line, fields, MAX_FIELDS);
        if ((count != 5 && count != 7) || !parse_int(fields[0], &value.id) || !parse_int(fields[2], &value.role) || !parse_int(fields[4], &value.year)) continue;
        copy_text(value.name, sizeof(value.name), fields[1]);
        copy_text(value.password, sizeof(value.password), fields[3]);
        if (count == 7) {
            copy_text(value.branch, sizeof(value.branch), fields[5]);
            copy_text(value.department, sizeof(value.department), fields[6]);
        }
        if (!user_append(head, value)) { fclose(file); return -1; }
        ++loaded;
    }
    if (ferror(file)) { fclose(file); return -1; }
    fclose(file);
    return loaded;
}

/* Saves users as id|name|role|password|year lines.
 * Parameters: filename and list head; returns 1 on success, 0 on failure.
 * Time is O(n), and extra space is O(1). */
int users_save(const char *filename, const UserNode *head)
{
    FILE *file;
    if (filename == NULL) return 0;
    file = fopen(filename, "w");
    if (file == NULL) return 0;
    while (head != NULL) {
        if (fprintf(file, "%d|", head->data.id) < 0) { fclose(file); return 0; }
        write_safe_field(file, head->data.name);
        if (fprintf(file, "|%d|", head->data.role) < 0) { fclose(file); return 0; }
        write_safe_field(file, head->data.password);
        if (fprintf(file, "|%d|", head->data.year) < 0) { fclose(file); return 0; }
        write_safe_field(file, head->data.branch);
        fputc('|', file);
        write_safe_field(file, head->data.department);
        if (fputc('\n', file) == EOF) { fclose(file); return 0; }
        head = head->next;
    }
    if (fclose(file) != 0) return 0;
    return 1;
}

/* Loads posts from id|title|content|category|date|author_id lines.
 * Parameters: filename and list head; returns records loaded, or -1 on error.
 * Time is O(L*n), and extra space is O(1) besides list nodes. */
int posts_load(const char *filename, PostNode **head)
{
    FILE *file;
    char line[LINE_SIZE];
    char *fields[MAX_FIELDS];
    int loaded = 0;
    int count;
    Post value;
    if (filename == NULL || head == NULL) return -1;
    file = fopen(filename, "r");
    if (file == NULL) return errno == ENOENT ? 0 : -1;
    while (fgets(line, sizeof(line), file) != NULL) {
        strip_line_end(line);
        count = split_fields(line, fields, MAX_FIELDS);
        if (count != 6 || !parse_int(fields[0], &value.id) || !parse_int(fields[5], &value.author_id)) continue;
        copy_text(value.title, sizeof(value.title), fields[1]);
        copy_text(value.content, sizeof(value.content), fields[2]);
        copy_text(value.category, sizeof(value.category), fields[3]);
        copy_text(value.date, sizeof(value.date), fields[4]);
        if (!post_append(head, value)) { fclose(file); return -1; }
        ++loaded;
    }
    if (ferror(file)) { fclose(file); return -1; }
    fclose(file);
    return loaded;
}

/* Saves posts as id|title|content|category|date|author_id lines.
 * Parameters: filename and list head; returns 1 on success, 0 on failure.
 * Time is O(n), and extra space is O(1). */
int posts_save(const char *filename, const PostNode *head)
{
    FILE *file;
    if (filename == NULL) return 0;
    file = fopen(filename, "w");
    if (file == NULL) return 0;
    while (head != NULL) {
        if (fprintf(file, "%d|", head->data.id) < 0) { fclose(file); return 0; }
        write_safe_field(file, head->data.title); fputc('|', file);
        write_safe_field(file, head->data.content); fputc('|', file);
        write_safe_field(file, head->data.category); fputc('|', file);
        write_safe_field(file, head->data.date);
        if (fprintf(file, "|%d\n", head->data.author_id) < 0) { fclose(file); return 0; }
        head = head->next;
    }
    if (fclose(file) != 0) return 0;
    return 1;
}

/* Loads events from id|name|venue|date|description lines.
 * Parameters: filename and list head; returns records loaded, or -1 on error.
 * Time is O(L*n), and extra space is O(1) besides list nodes. */
int events_load(const char *filename, EventNode **head)
{
    FILE *file;
    char line[LINE_SIZE];
    char *fields[MAX_FIELDS];
    int loaded = 0;
    int count;
    Event value;
    if (filename == NULL || head == NULL) return -1;
    file = fopen(filename, "r");
    if (file == NULL) return errno == ENOENT ? 0 : -1;
    while (fgets(line, sizeof(line), file) != NULL) {
        strip_line_end(line);
        count = split_fields(line, fields, MAX_FIELDS);
        if ((count != 5 && count != 6) || !parse_int(fields[0], &value.id)) continue;
        copy_text(value.name, sizeof(value.name), fields[1]);
        copy_text(value.venue, sizeof(value.venue), fields[2]);
        copy_text(value.date, sizeof(value.date), fields[3]);
        copy_text(value.description, sizeof(value.description), fields[4]);
        if (count == 6 && !parse_int(fields[5], &value.organizer_id)) continue;
        if (!event_append(head, value)) { fclose(file); return -1; }
        ++loaded;
    }
    if (ferror(file)) { fclose(file); return -1; }
    fclose(file);
    return loaded;
}

/* Saves events as id|name|venue|date|description lines.
 * Parameters: filename and list head; returns 1 on success, 0 on failure.
 * Time is O(n), and extra space is O(1). */
int events_save(const char *filename, const EventNode *head)
{
    FILE *file;
    if (filename == NULL) return 0;
    file = fopen(filename, "w");
    if (file == NULL) return 0;
    while (head != NULL) {
        if (fprintf(file, "%d|", head->data.id) < 0) { fclose(file); return 0; }
        write_safe_field(file, head->data.name); fputc('|', file);
        write_safe_field(file, head->data.venue); fputc('|', file);
        write_safe_field(file, head->data.date); fputc('|', file);
        write_safe_field(file, head->data.description); fputc('|', file);
        if (fprintf(file, "%d\n", head->data.organizer_id) < 0) { fclose(file); return 0; }
        head = head->next;
    }
    if (fclose(file) != 0) return 0;
    return 1;
}

/* Loads pending POST or EVENT records while preserving FIFO order.
 * Parameters: filename and queue; returns records loaded, or -1 on error.
 * Time is O(L*n), and extra space is O(1) besides queue nodes. */
int pending_load(const char *filename, PendingQueue *queue)
{
    FILE *file;
    char line[LINE_SIZE];
    char *fields[MAX_FIELDS];
    int loaded = 0;
    int count;
    PendingRec value;
    if (filename == NULL || queue == NULL) return -1;
    file = fopen(filename, "r");
    if (file == NULL) return errno == ENOENT ? 0 : -1;
    while (fgets(line, sizeof(line), file) != NULL) {
        memset(&value, 0, sizeof(value));
        strip_line_end(line);
        count = split_fields(line, fields, MAX_FIELDS);
        if (count < 3 || !parse_int(fields[1], &value.submitted_by)) continue;
        copy_text(value.type, sizeof(value.type), fields[0]);
        if (strcmp(value.type, "POST") == 0 && count == 8 && parse_int(fields[2], &value.post.id) && parse_int(fields[7], &value.post.author_id)) {
            copy_text(value.post.title, sizeof(value.post.title), fields[3]);
            copy_text(value.post.content, sizeof(value.post.content), fields[4]);
            copy_text(value.post.category, sizeof(value.post.category), fields[5]);
            copy_text(value.post.date, sizeof(value.post.date), fields[6]);
            if (!pending_enqueue(queue, value)) { fclose(file); return -1; }
            ++loaded;
        } else if (strcmp(value.type, "EVENT") == 0 && (count == 7 || count == 8) && parse_int(fields[2], &value.event.id)) {
            copy_text(value.event.name, sizeof(value.event.name), fields[3]);
            copy_text(value.event.venue, sizeof(value.event.venue), fields[4]);
            copy_text(value.event.date, sizeof(value.event.date), fields[5]);
            copy_text(value.event.description, sizeof(value.event.description), fields[6]);
            if (count == 8 && !parse_int(fields[7], &value.event.organizer_id)) continue;
            if (!pending_enqueue(queue, value)) { fclose(file); return -1; }
            ++loaded;
        }
    }
    if (ferror(file)) { fclose(file); return -1; }
    fclose(file);
    return loaded;
}

/* Saves pending records using POST or EVENT tagged lines.
 * Parameters: filename and FIFO queue; returns 1 on success, 0 on failure.
 * Time is O(n), and extra space is O(1). */
int pending_save(const char *filename, const PendingQueue *queue)
{
    FILE *file;
    const PendingNode *current;
    const PendingRec *value;
    if (filename == NULL || queue == NULL) return 0;
    file = fopen(filename, "w");
    if (file == NULL) return 0;
    for (current = queue->front; current != NULL; current = current->next) {
        value = &current->data;
        if (strcmp(value->type, "POST") == 0) {
            if (fprintf(file, "POST|%d|%d|", value->submitted_by, value->post.id) < 0) { fclose(file); return 0; }
            write_safe_field(file, value->post.title); fputc('|', file);
            write_safe_field(file, value->post.content); fputc('|', file);
            write_safe_field(file, value->post.category); fputc('|', file);
            write_safe_field(file, value->post.date);
            if (fprintf(file, "|%d\n", value->post.author_id) < 0) { fclose(file); return 0; }
        } else if (strcmp(value->type, "EVENT") == 0) {
            if (fprintf(file, "EVENT|%d|%d|", value->submitted_by, value->event.id) < 0) { fclose(file); return 0; }
            write_safe_field(file, value->event.name); fputc('|', file);
            write_safe_field(file, value->event.venue); fputc('|', file);
            write_safe_field(file, value->event.date); fputc('|', file);
            write_safe_field(file, value->event.description); fputc('|', file);
            if (fprintf(file, "%d\n", value->event.organizer_id) < 0) { fclose(file); return 0; }
        }
    }
    if (fclose(file) != 0) return 0;
    return 1;
}
