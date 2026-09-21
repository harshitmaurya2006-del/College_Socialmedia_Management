#ifndef CPP_USER_H
#define CPP_USER_H

#include "records.h"
#include <string>

class CollegeSystem;

// Abstract base class for all account roles.
class User {
    int id;
    std::string name;
    std::string password;
    int role;
public:
    // Builds a user from a C record.
    explicit User(const UserRec &record);
    // Enables safe deletion through a User pointer.
    virtual ~User();
    // Returns the account ID.
    int getId() const;
    // Returns the display name.
    std::string getName() const;
    // Returns the role number.
    int getRole() const;
    // Checks a plain-text demonstration password.
    bool checkPassword(const std::string &value) const;
    // Changes the password when it has at least four characters.
    bool changePassword(const std::string &value);
    // Converts this account to a C record.
    UserRec toRec() const;
    // Runs the role-specific menu loop.
    virtual void showMenu(CollegeSystem &system) = 0;
    // Prints common profile information.
    virtual void displayProfile() const;
    // Returns the role label.
    virtual std::string roleName() const = 0;
};

// Student account with year and branch information.
class Student : public User {
    int year;
    std::string branch;
public:
    // Builds a student from a C record.
    explicit Student(const UserRec &record);
    // Runs the student menu.
    void showMenu(CollegeSystem &system) override;
    // Prints student profile details.
    void displayProfile() const override;
    // Returns "Student".
    std::string roleName() const override;
};

// Faculty account with a department label.
class Faculty : public User {
    std::string department;
public:
    // Builds a faculty user from a C record.
    explicit Faculty(const UserRec &record);
    // Runs the faculty menu.
    void showMenu(CollegeSystem &system) override;
    // Prints faculty profile details.
    void displayProfile() const override;
    // Returns "Faculty".
    std::string roleName() const override;
};

// Administrator account with no additional fields.
class Admin : public User {
public:
    // Builds an administrator from a C record.
    explicit Admin(const UserRec &record);
    // Runs the administrator menu.
    void showMenu(CollegeSystem &system) override;
    // Prints administrator profile details.
    void displayProfile() const override;
    // Returns "Admin".
    std::string roleName() const override;
};

// Creates the correct derived user for a role number.
User *createUser(const UserRec &record);

#endif
