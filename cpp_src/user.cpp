#include "user.h"
#include "college_system.h"
#include "utils.h"
#include <iostream>

// Builds a user from a C record.
User::User(const UserRec &record) : id(record.id), name(record.name), password(record.password), role(record.role) {}
// Enables safe deletion through a User pointer.
User::~User() {}
// Returns the account ID.
int User::getId() const { return id; }
// Returns the display name.
std::string User::getName() const { return name; }
// Returns the role number.
int User::getRole() const { return role; }
// Checks a plain-text demonstration password.
bool User::checkPassword(const std::string &value) const { return password == value; }
// Changes the password when it has at least four characters.
bool User::changePassword(const std::string &value)
{
    if (value.size() < 4U || value.find('|') != std::string::npos) return false;
    password = value;
    return true;
}
// Converts this account to a C record.
UserRec User::toRec() const
{
    UserRec record = {};
    record.id = id; record.role = role;
    utils::copyToArray(record.name, sizeof(record.name), name);
    utils::copyToArray(record.password, sizeof(record.password), password);
    return record;
}
// Prints common profile information.
void User::displayProfile() const { std::cout << "ID: " << id << "\nName: " << name << "\nRole: " << roleName() << "\n"; }

// Builds a student from a C record.
Student::Student(const UserRec &record) : User(record), year(record.year), branch(record.branch[0] == '\0' ? "CSE" : record.branch) {}
// Runs the student menu.
void Student::showMenu(CollegeSystem &system)
{
    int choice;
    do {
        utils::printHeader("STUDENT MENU");
        choice = utils::readInt("1 View posts  2 View events  3 Search  4 Sort  5 Profile  0 Logout: ", 0, 5);
        if (choice == 1) system.showPosts(3, false);
        else if (choice == 2) system.showEvents(3, true);
        else if (choice == 3) system.searchMenu();
        else if (choice == 4) system.sortMenu();
        else if (choice == 5) system.profileMenu(*this);
    } while (choice != 0);
}
// Prints student profile details.
void Student::displayProfile() const { User::displayProfile(); std::cout << "Year: " << year << "\nBranch: " << branch << '\n'; }
// Returns "Student".
std::string Student::roleName() const { return "Student"; }

// Builds a faculty user from a C record.
Faculty::Faculty(const UserRec &record) : User(record), department(record.department[0] == '\0' ? "General" : record.department) {}
// Runs the faculty menu.
void Faculty::showMenu(CollegeSystem &system)
{
    int choice;
    do {
        utils::printHeader("FACULTY MENU");
        choice = utils::readInt("1 Posts 2 Events 3 Search 4 Sort 5 Submit post 6 Submit event 7 Manage posts 8 Pending 9 Profile 0 Logout: ", 0, 9);
        if (choice == 1) system.showPosts(3, false);
        else if (choice == 2) system.showEvents(3, true);
        else if (choice == 3) system.searchMenu();
        else if (choice == 4) system.sortMenu();
        else if (choice == 5) system.submitPost(Post(0, utils::readLine("Title: ", 1, 99), utils::readLine("Body: ", 1, 299), utils::readInt("Category 1-3: ", 1, 3), getId(), utils::todayString()), getId());
        else if (choice == 6) system.submitEvent(Event(0, utils::readLine("Name: ", 1, 99), utils::readLine("Description: ", 1, 299), utils::readLine("Venue: ", 1, 59), utils::readDate("Date: "), getId()), getId());
        else if (choice == 7) system.editPost(0, getId(), false);
        else if (choice == 8) system.showMyPending(getId());
        else if (choice == 9) system.profileMenu(*this);
    } while (choice != 0);
}
// Prints faculty profile details.
void Faculty::displayProfile() const { User::displayProfile(); std::cout << "Department: " << department << '\n'; }
// Returns "Faculty".
std::string Faculty::roleName() const { return "Faculty"; }

// Builds an administrator from a C record.
Admin::Admin(const UserRec &record) : User(record) {}
// Runs the administrator menu.
void Admin::showMenu(CollegeSystem &system)
{
    int choice;
    do {
        utils::printHeader("ADMIN MENU");
        choice = utils::readInt("1 Users 2 Posts 3 Events 4 Approvals 5 Data 6 Search/sort 7 Profile 0 Logout: ", 0, 7);
        if (choice == 1) { system.listUsers(); system.addFaculty(); system.findUserMenu(); }
        else if (choice == 2) system.showPosts(3, false);
        else if (choice == 3) system.showEvents(3, true);
        else if (choice == 4) { system.showPendingAll(); system.reviewNextPending(getId()); }
        else if (choice == 5) { system.showRecentActions(); system.showRecordCounts(); system.saveAll(); if (utils::readYesNo("Undo last action? (y/n): ")) system.undoLastAction(); }
        else if (choice == 6) { system.searchMenu(); system.sortMenu(); }
        else if (choice == 7) system.profileMenu(*this);
    } while (choice != 0);
}
// Prints administrator profile details.
void Admin::displayProfile() const { User::displayProfile(); }
// Returns "Admin".
std::string Admin::roleName() const { return "Admin"; }

// Creates the correct derived user for a role number.
User *createUser(const UserRec &record)
{
    if (record.role == ROLE_STUDENT) return new Student(record);
    if (record.role == ROLE_FACULTY) return new Faculty(record);
    if (record.role == ROLE_ADMIN) return new Admin(record);
    return NULL;
}
