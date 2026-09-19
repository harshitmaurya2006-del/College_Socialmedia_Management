#ifndef MODELS_H
#define MODELS_H

#include <string>

class User {
protected:
    std::string userID;
    std::string name;
public:
    User(const std::string &id, const std::string &userName);
    virtual ~User() = default;
    virtual std::string dashboardName() const = 0;
    std::string getID() const;
    std::string getName() const;
};

class Student : public User {
    std::string branch;
    int semester;
public:
    Student(const std::string &id, const std::string &studentName, const std::string &studentBranch, int studentSemester);
    std::string dashboardName() const override;
    std::string getBranch() const;
    int getSemester() const;
};

class Coordinator : public User {
public:
    Coordinator(const std::string &id, const std::string &coordinatorName);
    std::string dashboardName() const override;
};

class Admin : public User {
public:
    Admin(const std::string &id, const std::string &adminName);
    std::string dashboardName() const override;
};

class Post {
    std::string postID, title, content, category, date, authorID;
public:
    Post(const std::string &id, const std::string &postTitle, const std::string &postContent, const std::string &postCategory, const std::string &postDate, const std::string &author);
    std::string getID() const; std::string getTitle() const; std::string getContent() const; std::string getCategory() const; std::string getDate() const; std::string getAuthorID() const;
};

class Event {
    std::string eventID, eventName, venue, date, description;
public:
    Event(const std::string &id, const std::string &name, const std::string &eventVenue, const std::string &eventDate, const std::string &eventDescription);
    std::string getID() const; std::string getName() const; std::string getVenue() const; std::string getDate() const; std::string getDescription() const;
};

#endif
