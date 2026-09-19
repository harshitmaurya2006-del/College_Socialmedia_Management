#ifndef APPLICATION_H
#define APPLICATION_H

#include "c_structures.h"
#include "models.h"
#include <memory>
#include <string>

class Application {
    StudentNode *students;
    PostNode *posts;
    EventNode *events;
    ActionStack actions;
    ApprovalQueue approvals;
public:
    Application();
    ~Application();
    void run();
private:
    void loadSampleData();
    std::unique_ptr<User> login();
    void studentMenu(const Student &student);
    void coordinatorMenu(const Coordinator &coordinator);
    void adminMenu(const Admin &admin);
    void viewPosts(bool canManage);
    void viewEvents(bool canManage);
    void searchPosts();
    void searchEvents();
    void sortPosts();
    void sortEvents();
    void addPost(const std::string &authorID);
    void updatePost();
    void deletePost();
    void addEvent();
    void updateEvent();
    void deleteEvent();
    void addStudent();
    void updateStudent();
    void deleteStudent();
    void viewStudents();
    void showApprovals();
    void undoLastAction();
    static std::string readText(const std::string &prompt, bool allowEmpty = false);
    static int readNumber(const std::string &prompt, int minimum, int maximum);
    static void pauseScreen();
};

#endif
