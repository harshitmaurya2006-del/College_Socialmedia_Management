#include "project.h"

int main() {
    while (true) {
        cout << "\n=====================================\n";
        cout << " COLLEGE SOCIAL MEDIA MANAGEMENT\n";
        cout << "=====================================\n";
        cout << "1. Admin\n2. Student\n3. Exit\nChoose option: ";
        string choice;
        getline(cin, choice);

        if (choice == "1") {
            if (adminLogin()) adminDashboard();
        } else if (choice == "2") {
            cout << "\n1. Login\n2. Register\n3. Back\nChoose option: ";
            getline(cin, choice);
            if (choice == "1") {
                User student;
                if (studentLogin(student)) studentDashboard(student);
            } else if (choice == "2") {
                registerStudent();
            } else if (choice != "3") {
                cout << "Invalid option.\n";
            }
        } else if (choice == "3") {
            cout << "Thank you for using the system.\n";
            break;
        } else {
            cout << "Invalid option. Please try again.\n";
        }
    }
    return 0;
}
