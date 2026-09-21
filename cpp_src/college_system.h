#ifndef CPP_COLLEGE_SYSTEM_H
#define CPP_COLLEGE_SYSTEM_H

#include "content.h"
#include "ds.h"
#include "user.h"
#include <string>

// Controller that owns the C data structures and coordinates all menus.
class CollegeSystem {
    UserNode *users;
    PostNode *posts;
    EventNode *events;
    PendingQueue pending;
    PendingStack undoStack;
    int nextUserId;
    int nextPostId;
    int nextEventId;
    int nextReqId;
    static const char *usersFile;
    static const char *postsFile;
    static const char *eventsFile;
    static const char *pendingFile;
    // The class owns raw C nodes, so copying is deliberately forbidden.
    CollegeSystem(const CollegeSystem &) = delete;
    CollegeSystem &operator=(const CollegeSystem &) = delete;
    void refreshNextIds();
    void saveUsers();
    void savePosts();
    void saveEvents();
    void savePending();
    void printPostArray(const Post *items, int count) const;
    void printEventArray(const Event *items, int count) const;
    void editPostMenu(int requesterId, bool admin);
    void editEventMenu(int requesterId, bool admin);
    void createPostMenu(int authorId, bool immediate);
    void createEventMenu(int organizerId, bool immediate);
public:
    // Creates an empty controller and initializes the C containers.
    CollegeSystem();
    // Frees all C-owned records and stack/queue nodes.
    ~CollegeSystem();
    // Loads all four persistent data files.
    void loadAll();
    // Saves all four persistent data files.
    void saveAll();
    // Runs the start menu until the user exits.
    void run();
    // Logs in with up to three password attempts.
    User *login();
    // Registers and saves a student account.
    void registerStudent();
    // Displays posts in a selected order.
    void showPosts(int sortKey, bool ascending);
    // Displays events in a selected order.
    void showEvents(int sortKey, bool ascending);
    // Runs the search submenu.
    void searchMenu();
    // Runs the sort submenu.
    void sortMenu();
    // Publishes a post immediately and records an undo action.
    int publishPost(Post value);
    // Publishes an event immediately and records an undo action.
    int publishEvent(Event value);
    // Places a post into the approval queue.
    int submitPost(Post value, int userId);
    // Places an event into the approval queue.
    int submitEvent(Event value, int userId);
    // Edits a post after checking ownership.
    void editPost(int id, int requesterId, bool admin);
    // Edits an event after checking ownership.
    void editEvent(int id, int requesterId, bool admin);
    // Deletes a post and records its snapshot for undo.
    int deletePost(int id, int requesterId, bool admin);
    // Deletes an event and records its snapshot for undo.
    int deleteEvent(int id, int requesterId, bool admin);
    // Lists every pending request.
    void showPendingAll();
    // Lists pending requests submitted by one user.
    void showMyPending(int userId);
    // Reviews and approves or rejects the next request.
    void reviewNextPending(int adminId);
    // Displays the action stack newest first.
    void showRecentActions();
    // Pops and reverses the newest add/delete action.
    void undoLastAction();
    // Lists users sorted by ID.
    void listUsers();
    // Adds a faculty account as administrator.
    void addFaculty();
    // Deletes a protected-checked user.
    void deleteUser(int id, int actorId);
    // Finds and displays one user by ID.
    void findUserMenu();
    // Prints counts of all record types.
    void showRecordCounts();
    // Returns a user's name or Unknown.
    std::string userNameById(int id) const;
    // Displays a profile and optionally changes its password.
    void profileMenu(User &user);
};

#endif
