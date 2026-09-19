#include "models.h"

User::User(const std::string &id, const std::string &userName) : userID(id), name(userName) {}
std::string User::getID() const { return userID; }
std::string User::getName() const { return name; }

Student::Student(const std::string &id, const std::string &studentName, const std::string &studentBranch, int studentSemester) : User(id, studentName), branch(studentBranch), semester(studentSemester) {}
std::string Student::dashboardName() const { return "Student Dashboard"; }
std::string Student::getBranch() const { return branch; }
int Student::getSemester() const { return semester; }

Coordinator::Coordinator(const std::string &id, const std::string &coordinatorName) : User(id, coordinatorName) {}
std::string Coordinator::dashboardName() const { return "Coordinator Dashboard"; }

Admin::Admin(const std::string &id, const std::string &adminName) : User(id, adminName) {}
std::string Admin::dashboardName() const { return "Admin Dashboard"; }

Post::Post(const std::string &id, const std::string &postTitle, const std::string &postContent, const std::string &postCategory, const std::string &postDate, const std::string &author) : postID(id), title(postTitle), content(postContent), category(postCategory), date(postDate), authorID(author) {}
std::string Post::getID() const { return postID; }
std::string Post::getTitle() const { return title; }
std::string Post::getContent() const { return content; }
std::string Post::getCategory() const { return category; }
std::string Post::getDate() const { return date; }
std::string Post::getAuthorID() const { return authorID; }

Event::Event(const std::string &id, const std::string &name, const std::string &eventVenue, const std::string &eventDate, const std::string &eventDescription) : eventID(id), eventName(name), venue(eventVenue), date(eventDate), description(eventDescription) {}
std::string Event::getID() const { return eventID; }
std::string Event::getName() const { return eventName; }
std::string Event::getVenue() const { return venue; }
std::string Event::getDate() const { return date; }
std::string Event::getDescription() const { return description; }
