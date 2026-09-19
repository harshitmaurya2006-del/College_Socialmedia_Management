#include "algo.h"
#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int total;
static int passed;

static void check(int condition, const char *name)
{
    ++total;
    if (condition) { ++passed; printf("PASS: %s\n", name); }
    else printf("FAIL: %s\n", name);
}

static User make_user(int id, const char *name, int role, const char *password, int year)
{
    User value;
    memset(&value, 0, sizeof(value));
    value.id = id; value.role = role; value.year = year;
    strncpy(value.name, name, sizeof(value.name) - 1U);
    strncpy(value.password, password, sizeof(value.password) - 1U);
    return value;
}

static Post make_post(int id, const char *title, const char *content, const char *category, const char *date, int author)
{
    Post value;
    memset(&value, 0, sizeof(value));
    value.id = id; value.author_id = author;
    strncpy(value.title, title, sizeof(value.title) - 1U);
    strncpy(value.content, content, sizeof(value.content) - 1U);
    strncpy(value.category, category, sizeof(value.category) - 1U);
    strncpy(value.date, date, sizeof(value.date) - 1U);
    return value;
}

static Event make_event(int id, const char *name, const char *venue, const char *date, const char *description)
{
    Event value;
    memset(&value, 0, sizeof(value));
    value.id = id;
    strncpy(value.name, name, sizeof(value.name) - 1U);
    strncpy(value.venue, venue, sizeof(value.venue) - 1U);
    strncpy(value.date, date, sizeof(value.date) - 1U);
    strncpy(value.description, description, sizeof(value.description) - 1U);
    return value;
}

static PendingRec make_pending_post(int submitted, Post post)
{
    PendingRec value;
    memset(&value, 0, sizeof(value));
    strncpy(value.type, "POST", sizeof(value.type) - 1U);
    value.submitted_by = submitted;
    value.post = post;
    return value;
}

static PendingRec make_pending_event(int submitted, Event event)
{
    PendingRec value;
    memset(&value, 0, sizeof(value));
    strncpy(value.type, "EVENT", sizeof(value.type) - 1U);
    value.submitted_by = submitted;
    value.event = event;
    return value;
}

static int date_tests(void)
{
    const char *good[] = {"1900-01-01", "2026-01-31", "2026-04-30", "2028-02-29", "2000-02-29", "2100-12-31", "2027-06-15", "1999-11-30", "2012-02-29", "2050-09-01"};
    const char *bad[] = {"1899-12-31", "2101-01-01", "2027-02-29", "2026-02-30", "2026-04-31", "2026-13-01", "2026-00-01", "2026-01-00", "20260101", "2026-1-01", "abcd-01-01"};
    size_t index;
    int result = 1;
    for (index = 0; index < sizeof(good) / sizeof(good[0]); ++index) result = result && date_is_valid(good[index]);
    for (index = 0; index < sizeof(bad) / sizeof(bad[0]); ++index) result = result && !date_is_valid(bad[index]);
    return result;
}

static void test_lists(void)
{
    UserNode *users = NULL;
    User value = make_user(1, "Admin", 3, "admin123", 0);
    user_append(&users, value); value.id = 2; user_append(&users, value); value.id = 3; user_append(&users, value);
    check(user_find(users, 2) != NULL && user_find(users, 9) == NULL, "list find hit and miss");
    check(user_remove(&users, 1) == 1 && users->data.id == 2, "list delete head");
    check(user_remove(&users, 2) == 1 && users->data.id == 3, "list delete middle");
    check(user_remove(&users, 3) == 1 && users == NULL, "list delete tail");
    check(user_remove(&users, 9) == 0, "list delete missing");
    user_free_all(&users);
}

static void test_queue_stack(void)
{
    PostQueue queue;
    PendingStack stack;
    Post posts[2] = {make_post(1, "One", "", "General", "2026-01-01", 1), make_post(2, "Two", "", "General", "2026-01-02", 1)};
    PendingRec first = make_pending_post(1, posts[0]);
    PendingRec second = make_pending_event(2, make_event(8, "Event", "Hall", "2026-02-01", "Description"));
    PendingRec output;
    int count;
    PendingRec *array;
    queue_init(&queue); queue_enqueue(&queue, posts[0]); queue_enqueue(&queue, posts[1]);
    check(queue_dequeue(&queue, &posts[0]) == 1 && posts[0].id == 1, "queue FIFO first");
    check(queue_dequeue(&queue, &posts[0]) == 1 && posts[0].id == 2 && queue.size == 0, "queue FIFO second");
    queue_free_all(&queue);
    stack_init(&stack, 3); stack_push(&stack, first); stack_push(&stack, second);
    array = stack_to_array(&stack, &count);
    check(array != NULL && count == 2 && strcmp(array[0].type, "EVENT") == 0, "stack array top to bottom");
    free(array);
    check(stack_pop(&stack, &output) == 1 && strcmp(output.type, "EVENT") == 0, "stack LIFO first");
    check(stack_pop(&stack, &output) == 1 && output.post.id == 1, "stack LIFO second");
    stack_free_all(&stack);
}

static void test_algorithms(void)
{
    Post posts[4] = {make_post(4, "zebra", "Library book", "General", "2026-04-01", 1), make_post(2, "Alpha", "Exam", "Academic", "2026-01-01", 1), make_post(3, "alpha", "Sports", "Sports", "2026-03-01", 1), make_post(1, "Beta", "", "General", "2026-02-01", 1)};
    Event events[3] = {make_event(3, "Zoo", "B", "2026-03-01", "Music"), make_event(1, "alpha", "A", "2026-01-01", "Talk"), make_event(2, "Beta", "C", "2026-02-01", "Show")};
    PostNode *postHead = NULL;
    EventNode *eventHead = NULL;
    PostNode *matches;
    EventNode *eventMatches;
    int count;
    sort_posts_by_id(posts, 4, 1); check(posts[0].id == 1 && posts[3].id == 4, "post ID ascending");
    sort_posts_by_id(posts, 4, 0); check(posts[0].id == 4 && posts[3].id == 1, "post ID descending");
    sort_posts_by_title(posts, 4, 1); check(strcmp(posts[0].title, "Alpha") == 0 && posts[1].id == 3, "post title ascending with ID tie");
    sort_posts_by_title(posts, 4, 0); check(strcmp(posts[0].title, "zebra") == 0, "post title descending");
    sort_posts_by_date(posts, 4, 1); check(strcmp(posts[0].date, "2026-01-01") == 0, "post date ascending");
    sort_posts_by_date(posts, 4, 0); check(strcmp(posts[0].date, "2026-04-01") == 0, "post date descending");
    sort_events_by_id(events, 3, 1); check(events[0].id == 1, "event ID ascending");
    sort_events_by_id(events, 3, 0); check(events[0].id == 3, "event ID descending");
    sort_events_by_name(events, 3, 1); check(strcmp(events[0].name, "alpha") == 0, "event name ascending");
    sort_events_by_name(events, 3, 0); check(strcmp(events[0].name, "Zoo") == 0, "event name descending");
    sort_events_by_date(events, 3, 1); check(strcmp(events[0].date, "2026-01-01") == 0, "event date ascending");
    sort_events_by_date(events, 3, 0); check(strcmp(events[0].date, "2026-03-01") == 0, "event date descending");
    check(str_contains_ci("Central LIBRARY", "library") == 1 && str_contains_ci("Notice", "xyz") == 0, "case-insensitive substring");
    check(date_tests(), "date validation good and bad cases");
    post_append(&postHead, make_post(10, "Library Notice", "Open library", "General", "2026-05-01", 1));
    post_append(&postHead, make_post(20, "Exam", "Exam timetable", "Academic", "2026-05-02", 1));
    event_append(&eventHead, make_event(20, "Cultural Night", "Hall", "2026-05-03", "Music"));
    event_append(&eventHead, make_event(30, "Science Night", "Lab", "2026-05-04", "Projects"));
    matches = search_posts(postHead, "LIBRARY", &count);
    eventMatches = search_events(eventHead, "night", &count);
    check(matches != NULL && matches->data.id == 10, "linear post search hit");
    check(eventMatches != NULL && eventMatches->data.id == 20, "linear event search hit");
    post_free_all(&matches); event_free_all(&eventMatches);
    check(search_posts(postHead, "missing", &count) == NULL && count == 0, "linear search miss");
    sort_posts_by_id((Post *)0, 0, 1);
    sort_posts_by_id(posts, 4, 1);
    check(binary_search_post(postHead, 2, 20) != NULL && binary_search_post(postHead, 2, 99) == NULL, "binary post search hit and miss");
    check(binary_search_event(eventHead, 2, 30) != NULL && binary_search_event(eventHead, 2, 99) == NULL, "binary event search hit and miss");
    post_free_all(&postHead); event_free_all(&eventHead);
}

static void test_storage(void)
{
    UserNode *users = NULL;
    UserNode *loadedUsers = NULL;
    PostNode *posts = NULL;
    PostNode *loadedPosts = NULL;
    EventNode *events = NULL;
    EventNode *loadedEvents = NULL;
    PendingQueue pending;
    PendingQueue loadedPending;
    User user = make_user(1, "Admin", 3, "admin123", 0);
    Post post = make_post(5, "Notice", "Content", "Academic", "2026-05-05", 1);
    Event event = make_event(6, "Seminar", "Room 1", "2026-06-06", "Details");
    int result;
    pending_queue_init(&pending); pending_queue_init(&loadedPending);
    user_append(&users, user); post_append(&posts, post); event_append(&events, event);
    pending_enqueue(&pending, make_pending_post(2, post)); pending_enqueue(&pending, make_pending_event(2, event));
    result = users_save("data/test_users.txt", users) && posts_save("data/test_posts.txt", posts) && events_save("data/test_events.txt", events) && pending_save("data/test_pending.txt", &pending);
    result = result && users_load("data/test_users.txt", &loadedUsers) == 1 && posts_load("data/test_posts.txt", &loadedPosts) == 1 && events_load("data/test_events.txt", &loadedEvents) == 1 && pending_load("data/test_pending.txt", &loadedPending) == 2;
    check(result && loadedUsers->data.id == 1 && loadedPosts->data.id == 5 && loadedEvents->data.id == 6 && loadedPending.front->data.post.id == 5 && loadedPending.front->next->data.event.id == 6, "save and load round trip");
    check(users_load("data/file_that_does_not_exist.txt", &loadedUsers) == 0, "missing file loads as empty");
    user_free_all(&users); user_free_all(&loadedUsers); post_free_all(&posts); post_free_all(&loadedPosts); event_free_all(&events); event_free_all(&loadedEvents); pending_queue_free_all(&pending); pending_queue_free_all(&loadedPending);
    remove("data/test_users.txt"); remove("data/test_posts.txt"); remove("data/test_events.txt"); remove("data/test_pending.txt");
}

int main(void)
{
    test_lists();
    test_queue_stack();
    test_algorithms();
    test_storage();
    printf("\n%d/%d tests passed.\n", passed, total);
    return passed == total ? 0 : 1;
}
