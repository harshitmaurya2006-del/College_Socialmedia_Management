#include "college_system.h"
#include "storage.h"
#include "utils.h"
#include <cstring>
#include <iomanip>
#include <iostream>

const char *CollegeSystem::usersFile = "data/users.txt";
const char *CollegeSystem::postsFile = "data/posts.txt";
const char *CollegeSystem::eventsFile = "data/events.txt";
const char *CollegeSystem::pendingFile = "data/pending.txt";

// Creates an empty controller and initializes the C containers.
CollegeSystem::CollegeSystem() : users(NULL), posts(NULL), events(NULL), nextUserId(1), nextPostId(1), nextEventId(1), nextReqId(1)
{
    pending_queue_init(&pending);
    stack_init(&undoStack, 100);
}

// Frees all C-owned records and stack/queue nodes.
CollegeSystem::~CollegeSystem()
{
    user_free_all(&users); post_free_all(&posts); event_free_all(&events);
    pending_queue_free_all(&pending); stack_free_all(&undoStack);
}

// Computes the next IDs from loaded arrays and never decreases them during a run.
void CollegeSystem::refreshNextIds()
{
    int count;
    int index;
    UserRec *userArray = user_to_array(users, &count);
    for (index = 0; index < count; ++index) if (userArray[index].id >= nextUserId) nextUserId = userArray[index].id + 1;
    free(userArray);
    PostRec *postArray = post_to_array(posts, &count);
    for (index = 0; index < count; ++index) if (postArray[index].id >= nextPostId) nextPostId = postArray[index].id + 1;
    free(postArray);
    EventRec *eventArray = event_to_array(events, &count);
    for (index = 0; index < count; ++index) if (eventArray[index].id >= nextEventId) nextEventId = eventArray[index].id + 1;
    free(eventArray);
}

// Loads all four persistent data files.
void CollegeSystem::loadAll()
{
    users_load(usersFile, &users); posts_load(postsFile, &posts); events_load(eventsFile, &events); pending_load(pendingFile, &pending);
    refreshNextIds();
    if (users == NULL) {
        UserRec admin = {};
        admin.id = 1; admin.role = ROLE_ADMIN;
        utils::copyToArray(admin.name, sizeof(admin.name), "Admin");
        utils::copyToArray(admin.password, sizeof(admin.password), "admin123");
        user_append(&users, admin); nextUserId = 2; saveAll();
        std::cout << "Default Admin account created. ID 1, password admin123.\n";
    }
}

// Saves all four persistent data files.
void CollegeSystem::saveAll()
{
    users_save(usersFile, users); posts_save(postsFile, posts); events_save(eventsFile, events); pending_save(pendingFile, &pending);
}
// Saves users after a user change.
void CollegeSystem::saveUsers() { users_save(usersFile, users); }
// Saves posts after a post change.
void CollegeSystem::savePosts() { posts_save(postsFile, posts); }
// Saves events after an event change.
void CollegeSystem::saveEvents() { events_save(eventsFile, events); }
// Saves pending records after a queue change.
void CollegeSystem::savePending() { pending_save(pendingFile, &pending); }

// Returns a user's name or Unknown.
std::string CollegeSystem::userNameById(int id) const
{
    UserNode *node = user_find(users, id);
    return node == NULL ? "Unknown" : std::string(node->data.name);
}

// Prints a post array as aligned rows.
void CollegeSystem::printPostArray(const PostRec *items, int count) const
{
    int index;
    for (index = 0; index < count; ++index) std::cout << std::left << std::setw(5) << items[index].id << std::setw(28) << items[index].title << std::setw(12) << items[index].date << userNameById(items[index].author_id) << '\n';
}
// Prints an event array as aligned rows.
void CollegeSystem::printEventArray(const EventRec *items, int count) const
{
    int index;
    for (index = 0; index < count; ++index) std::cout << std::left << std::setw(5) << items[index].id << std::setw(28) << items[index].name << std::setw(12) << items[index].date << items[index].venue << '\n';
}

// Displays posts after C insertion sorting by the selected key.
void CollegeSystem::showPosts(int sortKey, bool ascending)
{
    int count;
    PostRec *array = post_to_array(posts, &count);
    utils::printHeader("POSTS");
    if (sortKey == 1) sort_posts_by_id(array, count, ascending);
    else if (sortKey == 2) sort_posts_by_title(array, count, ascending);
    else sort_posts_by_date(array, count, ascending);
    printPostArray(array, count);
    free(array);
}

// Displays events after C insertion sorting by the selected key.
void CollegeSystem::showEvents(int sortKey, bool ascending)
{
    int count;
    EventRec *array = event_to_array(events, &count);
    utils::printHeader("EVENTS");
    if (sortKey == 1) sort_events_by_id(array, count, ascending);
    else if (sortKey == 2) sort_events_by_name(array, count, ascending);
    else sort_events_by_date(array, count, ascending);
    printEventArray(array, count);
    free(array);
}

// Creates and publishes a post, assigning a monotonically increasing ID.
int CollegeSystem::publishPost(Post value)
{
    PostRec record = value.toRec(); record.id = nextPostId++;
    if (!post_append(&posts, record)) return 0;
    ActionRec action = {}; std::strncpy(action.type, "POST_ADD", sizeof(action.type) - 1U); action.post = record;
    stack_push(&undoStack, action); savePosts();
    return record.id;
}

// Creates and publishes an event, assigning a monotonically increasing ID.
int CollegeSystem::publishEvent(Event value)
{
    EventRec record = value.toRec(); record.id = nextEventId++;
    if (!event_append(&events, record)) return 0;
    ActionRec action = {}; std::strncpy(action.type, "EVENT_ADD", sizeof(action.type) - 1U); action.event = record;
    stack_push(&undoStack, action); saveEvents();
    return record.id;
}

// Places a post into the approval queue.
int CollegeSystem::submitPost(Post value, int userId)
{
    PendingRec request = {}; std::strncpy(request.type, "POST", sizeof(request.type) - 1U); request.submitted_by = userId; request.post = value.toRec();
    if (!pending_enqueue(&pending, request)) return 0;
    savePending(); std::cout << "Post submitted for approval.\n"; return 1;
}

// Places an event into the approval queue.
int CollegeSystem::submitEvent(Event value, int userId)
{
    PendingRec request = {}; std::strncpy(request.type, "EVENT", sizeof(request.type) - 1U); request.submitted_by = userId; request.event = value.toRec();
    if (!pending_enqueue(&pending, request)) return 0;
    savePending(); std::cout << "Event submitted for approval.\n"; return 1;
}

// Logs in with up to three password attempts.
User *CollegeSystem::login()
{
    int id = utils::readInt("ID: ", 1, 1000000000);
    UserNode *node = user_find(users, id);
    if (node == NULL) {
        std::cout << "ID not found.\n";
        if (utils::readYesNo("Register as a new student? (y/n): ")) registerStudent();
        return NULL;
    }
    for (int attempt = 0; attempt < 3; ++attempt) {
        std::string password = utils::readLine("Password: ", 1, 63);
        if (std::strcmp(node->data.password, password.c_str()) == 0) return createUser(node->data);
        std::cout << "Incorrect password.\n";
    }
    return NULL;
}

// Registers and saves a student account.
void CollegeSystem::registerStudent()
{
    UserRec record = {};
    record.id = nextUserId++; record.role = ROLE_STUDENT; record.year = utils::readInt("Year (1-4): ", 1, 4);
    utils::copyToArray(record.name, sizeof(record.name), utils::readLine("Name: ", 2, 49));
    utils::copyToArray(record.password, sizeof(record.password), utils::readLine("Password: ", 4, 63));
    utils::copyToArray(record.branch, sizeof(record.branch), utils::readLine("Branch: ", 1, 29));
    if (user_append(&users, record)) { saveUsers(); std::cout << "Registered. Your ID is " << record.id << ".\n"; }
}

// Runs the search submenu using the C linear and binary search APIs.
void CollegeSystem::searchMenu()
{
    int choice = utils::readInt("1 Search posts 2 Search events 3 Find post ID 4 Find event ID 0 Back: ", 0, 4);
    if (choice == 1) {
        int count; PostNode *result = search_posts(posts, utils::readLine("Keyword: ", 1, 99).c_str(), &count); // Uses LINEAR SEARCH (search_posts).
        PostRec *array = post_to_array(result, &count); printPostArray(array, count); free(array); post_free_all(&result);
    } else if (choice == 2) {
        int count; EventNode *result = search_events(events, utils::readLine("Keyword: ", 1, 99).c_str(), &count); // Uses LINEAR SEARCH (search_events).
        EventRec *array = event_to_array(result, &count); printEventArray(array, count); free(array); event_free_all(&result);
    } else if (choice == 3) {
        int count; PostRec *array = post_to_array(posts, &count); sort_posts_by_id(array, count, 1); PostNode *temporary = NULL;
        for (int i = 0; i < count; ++i) post_append(&temporary, array[i]);
        PostNode *found = binary_search_post(temporary, count, utils::readInt("Post ID: ", 1, 1000000000)); // Uses BINARY SEARCH on an id-sorted array.
        if (found != NULL) Post(found->data).display(userNameById(found->data.author_id));
        else std::cout << "Post not found.\n";
        post_free_all(&temporary); free(array);
    } else if (choice == 4) {
        int count; EventRec *array = event_to_array(events, &count); sort_events_by_id(array, count, 1); EventNode *temporary = NULL;
        for (int i = 0; i < count; ++i) event_append(&temporary, array[i]);
        EventNode *found = binary_search_event(temporary, count, utils::readInt("Event ID: ", 1, 1000000000)); // Uses BINARY SEARCH on an id-sorted array.
        if (found != NULL) Event(found->data).display(userNameById(found->data.organizer_id));
        else std::cout << "Event not found.\n";
        event_free_all(&temporary); free(array);
    }
}

// Runs the sort submenu and delegates ordering to C insertion sorts.
void CollegeSystem::sortMenu()
{
    int kind = utils::readInt("1 Posts 2 Events 0 Back: ", 0, 2);
    if (kind == 0) return;
    int key = utils::readInt("1 ID 2 Title/Name 3 Date: ", 1, 3);
    bool ascending = utils::readYesNo("Ascending? (y/n): ");
    if (kind == 1) showPosts(key, ascending);
    else showEvents(key, ascending);
}

// Edits a post after checking ownership.
void CollegeSystem::editPost(int id, int requesterId, bool admin)
{
    if (id == 0) id = utils::readInt("Post ID: ", 1, 1000000000);
    PostNode *node = post_find(posts, id);
    if (node == NULL || (!admin && node->data.author_id != requesterId)) { std::cout << "Post not found or not owned by you.\n"; return; }
    utils::copyToArray(node->data.title, sizeof(node->data.title), utils::readLine("New title: ", 1, 99));
    utils::copyToArray(node->data.content, sizeof(node->data.content), utils::readLine("New body: ", 1, 299));
    node->data.category[0] = static_cast<char>('0' + utils::readInt("Category 1-3: ", 1, 3)); node->data.category[1] = '\0'; savePosts();
}

// Edits an event after checking ownership.
void CollegeSystem::editEvent(int id, int requesterId, bool admin)
{
    if (id == 0) id = utils::readInt("Event ID: ", 1, 1000000000);
    EventNode *node = event_find(events, id);
    if (node == NULL || (!admin && node->data.organizer_id != requesterId)) { std::cout << "Event not found or not owned by you.\n"; return; }
    utils::copyToArray(node->data.name, sizeof(node->data.name), utils::readLine("New name: ", 1, 99));
    utils::copyToArray(node->data.description, sizeof(node->data.description), utils::readLine("New description: ", 1, 299));
    utils::copyToArray(node->data.venue, sizeof(node->data.venue), utils::readLine("New venue: ", 1, 59));
    utils::copyToArray(node->data.date, sizeof(node->data.date), utils::readDate("New date: ")); saveEvents();
}

// Deletes a post and records its snapshot for undo.
int CollegeSystem::deletePost(int id, int requesterId, bool admin)
{
    PostNode *node = post_find(posts, id);
    if (node == NULL || (!admin && node->data.author_id != requesterId)) return 0;
    ActionRec action = {}; std::strncpy(action.type, "POST_DELETE", sizeof(action.type) - 1U); action.post = node->data;
    if (!post_remove(&posts, id)) return 0;
    stack_push(&undoStack, action); savePosts(); return 1;
}

// Deletes an event and records its snapshot for undo.
int CollegeSystem::deleteEvent(int id, int requesterId, bool admin)
{
    EventNode *node = event_find(events, id);
    if (node == NULL || (!admin && node->data.organizer_id != requesterId)) return 0;
    ActionRec action = {}; std::strncpy(action.type, "EVENT_DELETE", sizeof(action.type) - 1U); action.event = node->data;
    if (!event_remove(&events, id)) return 0;
    stack_push(&undoStack, action); saveEvents(); return 1;
}

// Lists every pending request in FIFO order.
void CollegeSystem::showPendingAll()
{
    int count; PendingRec *array = pending_queue_to_array(&pending, &count);
    for (int i = 0; i < count; ++i) std::cout << i + 1 << ". " << array[i].type << " from user " << array[i].submitted_by << "\n";
    free(array);
}

// Lists pending requests submitted by one user.
void CollegeSystem::showMyPending(int userId)
{
    int count; PendingRec *array = pending_queue_to_array(&pending, &count);
    for (int i = 0; i < count; ++i) if (array[i].submitted_by == userId) std::cout << array[i].type << " request\n";
    free(array);
}

// Reviews and approves or rejects the next request.
void CollegeSystem::reviewNextPending(int adminId)
{
    (void)adminId;
    PendingRec request;
    if (!pending_dequeue(&pending, &request)) { std::cout << "No pending requests.\n"; return; }
    bool approve = utils::readYesNo("Approve this request? (y/n): ");
    if (approve) {
        if (std::strcmp(request.type, "POST") == 0) publishPost(Post(request.post));
        else if (std::strcmp(request.type, "EVENT") == 0) publishEvent(Event(request.event));
    }
    savePending();
}

// Displays the action stack newest first.
void CollegeSystem::showRecentActions()
{
    int count; PendingRec *array = stack_to_array(&undoStack, &count);
    for (int i = 0; i < count; ++i) std::cout << array[i].type << '\n';
    free(array);
}

// Pops and reverses the newest add/delete action.
void CollegeSystem::undoLastAction()
{
    ActionRec action;
    if (!stack_pop(&undoStack, &action)) { std::cout << "No action to undo.\n"; return; }
    if (std::strcmp(action.type, "POST_ADD") == 0) { post_remove(&posts, action.post.id); savePosts(); }
    else if (std::strcmp(action.type, "POST_DELETE") == 0) { post_append(&posts, action.post); savePosts(); }
    else if (std::strcmp(action.type, "EVENT_ADD") == 0) { event_remove(&events, action.event.id); saveEvents(); }
    else if (std::strcmp(action.type, "EVENT_DELETE") == 0) { event_append(&events, action.event); saveEvents(); }
}

// Lists users sorted by ID through the C insertion sort.
void CollegeSystem::listUsers()
{
    int count; UserRec *array = user_to_array(users, &count); sort_users(array, count, 1);
    for (int i = 0; i < count; ++i) std::cout << array[i].id << " | " << array[i].name << " | role " << array[i].role << '\n';
    free(array);
}

// Adds a faculty account as administrator.
void CollegeSystem::addFaculty()
{
    if (!utils::readYesNo("Add faculty now? (y/n): ")) return;
    UserRec record = {}; record.id = nextUserId++; record.role = ROLE_FACULTY;
    utils::copyToArray(record.name, sizeof(record.name), utils::readLine("Name: ", 2, 49));
    utils::copyToArray(record.password, sizeof(record.password), utils::readLine("Password: ", 4, 63));
    utils::copyToArray(record.department, sizeof(record.department), utils::readLine("Department: ", 1, 29));
    if (user_append(&users, record)) saveUsers();
}

// Deletes a protected-checked user.
void CollegeSystem::deleteUser(int id, int actorId)
{
    if (id == actorId || id == 1) { std::cout << "That user cannot be deleted.\n"; return; }
    if (user_remove(&users, id)) saveUsers(); else std::cout << "User not found.\n";
}

// Finds and displays one user by ID.
void CollegeSystem::findUserMenu()
{
    if (!utils::readYesNo("Find a user? (y/n): ")) return;
    int id = utils::readInt("User ID: ", 1, 1000000000); UserNode *node = user_find(users, id);
    if (node == NULL) std::cout << "User not found.\n"; else std::cout << node->data.id << " | " << node->data.name << " | role " << node->data.role << '\n';
}

// Prints counts of all record types.
void CollegeSystem::showRecordCounts()
{
    int usersCount; int postsCount; int eventsCount; int pendingCount;
    UserRec *usersArray = user_to_array(users, &usersCount); PostRec *postsArray = post_to_array(posts, &postsCount); EventRec *eventsArray = event_to_array(events, &eventsCount); PendingRec *pendingArray = pending_queue_to_array(&pending, &pendingCount);
    free(usersArray); free(postsArray); free(eventsArray); free(pendingArray);
    std::cout << "Users: " << usersCount << " Posts: " << postsCount << " Events: " << eventsCount << " Pending: " << pendingCount << '\n';
}

// Displays a profile and optionally changes its password.
void CollegeSystem::profileMenu(User &user)
{
    user.displayProfile();
    if (utils::readYesNo("Change password? (y/n): ")) {
        std::string value = utils::readLine("New password: ", 4, 63);
        if (user.changePassword(value)) {
            UserNode *node = user_find(users, user.getId());
            if (node != NULL) { UserRec record = node->data; UserRec updated = user.toRec(); std::strncpy(record.password, updated.password, sizeof(record.password) - 1U); record.password[sizeof(record.password) - 1U] = '\0'; node->data = record; saveUsers(); }
        }
    }
}

// Runs the start menu until the user exits.
void CollegeSystem::run()
{
    int choice;
    do {
        utils::printHeader("COLLEGE SOCIAL MEDIA MANAGEMENT SYSTEM");
        choice = utils::readInt("1 Login 2 Register 0 Exit: ", 0, 2);
        if (choice == 1) {
            User *account = login();
            if (account != NULL) { account->showMenu(*this); delete account; } // Polymorphic call through the abstract User base.
        } else if (choice == 2) registerStudent();
    } while (choice != 0);
    saveAll();
}
