#include "application.h"
#include "search_sort.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

using std::cin; using std::cout; using std::string;

static void copyText(char *target, size_t size, const string &value) { std::strncpy(target, value.c_str(), size - 1); target[size - 1] = '\0'; }
static void printLine() { cout << "\n------------------------------------------------------------\n"; }
static void printPost(const CPost &post) { cout << post.postID << " | " << post.title << " | " << post.category << " | " << post.date << " | Author: " << post.authorID << "\n  " << post.content << "\n"; }
static void printEvent(const CEvent &event) { cout << event.eventID << " | " << event.eventName << " | " << event.venue << " | " << event.date << "\n  " << event.description << "\n"; }

Application::Application() : students(nullptr), posts(nullptr), events(nullptr) { stack_init(&actions, 30); queue_init(&approvals); loadSampleData(); }
Application::~Application() { student_free_all(&students); post_free_all(&posts); event_free_all(&events); stack_free(&actions); queue_free(&approvals); }

void Application::loadSampleData() {
    CStudent student{}; copyText(student.studentID, sizeof(student.studentID), "S1001"); copyText(student.name, sizeof(student.name), "Aarav Mehta"); copyText(student.branch, sizeof(student.branch), "CSE"); student.semester = 4; student_append(&students, student);
    copyText(student.studentID, sizeof(student.studentID), "S1002"); copyText(student.name, sizeof(student.name), "Diya Shah"); copyText(student.branch, sizeof(student.branch), "ECE"); student.semester = 3; student_append(&students, student);
    CPost post{}; copyText(post.postID, sizeof(post.postID), "P101"); copyText(post.title, sizeof(post.title), "Mid-Semester Examination Notice"); copyText(post.content, sizeof(post.content), "Mid-semester examinations begin from 15 October."); copyText(post.category, sizeof(post.category), "Academic"); copyText(post.date, sizeof(post.date), "2026-10-01"); copyText(post.authorID, sizeof(post.authorID), "A3001"); post_append(&posts, post);
    copyText(post.postID, sizeof(post.postID), "P102"); copyText(post.title, sizeof(post.title), "Library Timing Update"); copyText(post.content, sizeof(post.content), "The central library will remain open until 8 PM."); copyText(post.category, sizeof(post.category), "General"); copyText(post.date, sizeof(post.date), "2026-09-20"); copyText(post.authorID, sizeof(post.authorID), "C2001"); post_append(&posts, post);
    CEvent event{}; copyText(event.eventID, sizeof(event.eventID), "E201"); copyText(event.eventName, sizeof(event.eventName), "Tech Symposium"); copyText(event.venue, sizeof(event.venue), "Main Auditorium"); copyText(event.date, sizeof(event.date), "2026-10-20"); copyText(event.description, sizeof(event.description), "Talks and demonstrations by student teams."); event_append(&events, event);
    copyText(event.eventID, sizeof(event.eventID), "E202"); copyText(event.eventName, sizeof(event.eventName), "Cultural Evening"); copyText(event.venue, sizeof(event.venue), "Open Air Theatre"); copyText(event.date, sizeof(event.date), "2026-11-05"); copyText(event.description, sizeof(event.description), "An evening of music, dance and drama."); event_append(&events, event);
}

string Application::readText(const string &prompt, bool allowEmpty) { string value; do { cout << prompt; std::getline(cin, value); if (!allowEmpty && value.empty()) cout << "Input cannot be empty.\n"; } while (!allowEmpty && value.empty()); return value; }
int Application::readNumber(const string &prompt, int minimum, int maximum) { string value; int number; while (true) { cout << prompt; std::getline(cin, value); std::stringstream stream(value); char extra; if (stream >> number && !(stream >> extra) && number >= minimum && number <= maximum) return number; cout << "Enter a number from " << minimum << " to " << maximum << ".\n"; } }
void Application::pauseScreen() { cout << "\nPress Enter to continue..."; string ignored; std::getline(cin, ignored); }

std::unique_ptr<User> Application::login() {
    printLine(); cout << "LOGIN\n";
    string id = readText("User ID (S1001/C2001/A3001): "); string password = readText("Password: ");
    if (id == "S1001" && password == "student") { StudentNode *record = student_find(students, id.c_str()); return std::make_unique<Student>(id, record->data.name, record->data.branch, record->data.semester); }
    if (id == "S1002" && password == "student") { StudentNode *record = student_find(students, id.c_str()); return std::make_unique<Student>(id, record->data.name, record->data.branch, record->data.semester); }
    if (id == "C2001" && password == "coord") return std::make_unique<Coordinator>(id, "Neha Coordinator");
    if (id == "A3001" && password == "admin") return std::make_unique<Admin>(id, "Rohan Admin");
    cout << "Login failed: invalid ID or password.\n"; return nullptr;
}

void Application::run() {
    cout << "\nCOLLEGE SOCIAL MEDIA MANAGEMENT SYSTEM\n"; cout << "Command-line PBL project | C data structures + C++ OOP\n";
    while (true) { std::unique_ptr<User> user = login(); if (!user) { if (readNumber("Try again? (1=Yes, 0=Exit): ", 0, 1) == 0) break; continue; }
        cout << "Welcome, " << user->getName() << " - " << user->dashboardName() << "\n";
        if (Student *student = dynamic_cast<Student *>(user.get())) studentMenu(*student); else if (Coordinator *coordinator = dynamic_cast<Coordinator *>(user.get())) coordinatorMenu(*coordinator); else if (Admin *admin = dynamic_cast<Admin *>(user.get())) adminMenu(*admin);
        if (readNumber("Login again? (1=Yes, 0=Exit): ", 0, 1) == 0) break;
    }
    cout << "Thank you for using the system.\n";
}

void Application::studentMenu(const Student &student) {
    while (true) { printLine(); cout << "STUDENT MENU - " << student.getName() << " (" << student.getBranch() << ", Sem " << student.getSemester() << ")\n1. View posts/notices\n2. View events\n3. Search posts\n4. Search events\n5. Sort posts\n6. Sort events\n0. Logout\n"; int choice = readNumber("Choice: ", 0, 6); if (choice == 0) return; if (choice == 1) viewPosts(false); else if (choice == 2) viewEvents(false); else if (choice == 3) searchPosts(); else if (choice == 4) searchEvents(); else if (choice == 5) sortPosts(); else sortEvents(); pauseScreen(); }
}
void Application::coordinatorMenu(const Coordinator &coordinator) {
    while (true) { printLine(); cout << "COORDINATOR MENU - " << coordinator.getName() << "\n1. View posts\n2. Add post\n3. Update post\n4. Delete post\n5. View events\n6. Add event\n7. Update event\n8. Delete event\n9. Search/sort records\n10. Undo last add\n0. Logout\n"; int choice = readNumber("Choice: ", 0, 10); if (choice == 0) return; if (choice == 1) viewPosts(true); else if (choice == 2) addPost(coordinator.getID()); else if (choice == 3) updatePost(); else if (choice == 4) deletePost(); else if (choice == 5) viewEvents(true); else if (choice == 6) addEvent(); else if (choice == 7) updateEvent(); else if (choice == 8) deleteEvent(); else if (choice == 9) { searchPosts(); searchEvents(); } else undoLastAction(); pauseScreen(); }
}
void Application::adminMenu(const Admin &admin) {
    while (true) { printLine(); cout << "ADMIN MENU - " << admin.getName() << "\n1. View students\n2. Add student\n3. Update student\n4. Delete student\n5. View posts\n6. Add post\n7. Update post\n8. Delete post\n9. View events\n10. Add event\n11. Update event\n12. Delete event\n13. Search/sort all records\n14. Process approval queue\n15. Undo last add\n0. Logout\n"; int choice = readNumber("Choice: ", 0, 15); if (choice == 0) return; if (choice == 1) viewStudents(); else if (choice == 2) addStudent(); else if (choice == 3) updateStudent(); else if (choice == 4) deleteStudent(); else if (choice == 5) viewPosts(true); else if (choice == 6) addPost(admin.getID()); else if (choice == 7) updatePost(); else if (choice == 8) deletePost(); else if (choice == 9) viewEvents(true); else if (choice == 10) addEvent(); else if (choice == 11) updateEvent(); else if (choice == 12) deleteEvent(); else if (choice == 13) { viewStudents(); searchPosts(); searchEvents(); sortPosts(); sortEvents(); } else if (choice == 14) showApprovals(); else undoLastAction(); pauseScreen(); }
}

void Application::viewPosts(bool canManage) { printLine(); cout << "POSTS / NOTICES\n"; if (!posts) cout << "No posts found.\n"; for (PostNode *current = posts; current; current = current->next) printPost(current->data); if (canManage) cout << "Management actions are available from the menu.\n"; }
void Application::viewEvents(bool canManage) { printLine(); cout << "EVENTS\n"; if (!events) cout << "No events found.\n"; for (EventNode *current = events; current; current = current->next) printEvent(current->data); if (canManage) cout << "Management actions are available from the menu.\n"; }
void Application::viewStudents() { printLine(); cout << "STUDENTS\n"; for (StudentNode *current = students; current; current = current->next) cout << current->data.studentID << " | " << current->data.name << " | " << current->data.branch << " | Semester " << current->data.semester << "\n"; }

void Application::searchPosts() { string key = readText("Post ID or title keyword: "); sort_posts_by_id(posts); PostNode *exact = binary_search_post_by_id(posts, post_count(posts), key.c_str()); if (exact) { printPost(exact->data); return; } bool found = false; for (PostNode *p = posts; p; p = p->next) if (std::strstr(p->data.title, key.c_str())) { printPost(p->data); found = true; } if (!found) cout << "No matching post.\n"; }
void Application::searchEvents() { string key = readText("Event ID or name keyword: "); sort_events_by_id(events); EventNode *exact = binary_search_event_by_id(events, event_count(events), key.c_str()); if (exact) { printEvent(exact->data); return; } bool found = false; for (EventNode *e = events; e; e = e->next) if (std::strstr(e->data.eventName, key.c_str())) { printEvent(e->data); found = true; } if (!found) cout << "No matching event.\n"; }
void Application::sortPosts() { int choice = readNumber("Sort posts: 1=ID 2=Title 3=Date: ", 1, 3); if (choice == 1) sort_posts_by_id(posts); else if (choice == 2) sort_posts_by_title(posts); else sort_posts_by_date(posts); viewPosts(false); }
void Application::sortEvents() { int choice = readNumber("Sort events: 1=ID 2=Name 3=Date: ", 1, 3); if (choice == 1) sort_events_by_id(events); else if (choice == 2) sort_events_by_name(events); else sort_events_by_date(events); viewEvents(false); }

void Application::addPost(const string &authorID) { CPost post{}; copyText(post.postID, sizeof(post.postID), readText("Post ID: ")); if (post_find(posts, post.postID)) { cout << "Post ID already exists.\n"; return; } copyText(post.title, sizeof(post.title), readText("Title: ")); copyText(post.content, sizeof(post.content), readText("Content: ")); copyText(post.category, sizeof(post.category), readText("Category: ")); copyText(post.date, sizeof(post.date), readText("Date (YYYY-MM-DD): ")); copyText(post.authorID, sizeof(post.authorID), authorID); post_append(&posts, post); string action = "POST_ADD:" + string(post.postID); stack_push(&actions, action.c_str()); ApprovalItem item{}; copyText(item.recordType, sizeof(item.recordType), "POST"); copyText(item.recordID, sizeof(item.recordID), post.postID); queue_enqueue(&approvals, item); cout << "Post added and placed in approval queue.\n"; }
void Application::updatePost() { string id = readText("Post ID to update: "); PostNode *p = post_find(posts, id.c_str()); if (!p) { cout << "Post not found.\n"; return; } copyText(p->data.title, sizeof(p->data.title), readText("New title: ")); copyText(p->data.content, sizeof(p->data.content), readText("New content: ")); copyText(p->data.category, sizeof(p->data.category), readText("New category: ")); copyText(p->data.date, sizeof(p->data.date), readText("New date: ")); cout << "Post updated.\n"; }
void Application::deletePost() { string id = readText("Post ID to delete: "); cout << (post_remove(&posts, id.c_str()) ? "Post deleted.\n" : "Post not found.\n"); }
void Application::addEvent() { CEvent event{}; copyText(event.eventID, sizeof(event.eventID), readText("Event ID: ")); if (event_find(events, event.eventID)) { cout << "Event ID already exists.\n"; return; } copyText(event.eventName, sizeof(event.eventName), readText("Event name: ")); copyText(event.venue, sizeof(event.venue), readText("Venue: ")); copyText(event.date, sizeof(event.date), readText("Date (YYYY-MM-DD): ")); copyText(event.description, sizeof(event.description), readText("Description: ")); event_append(&events, event); string action = "EVENT_ADD:" + string(event.eventID); stack_push(&actions, action.c_str()); ApprovalItem item{}; copyText(item.recordType, sizeof(item.recordType), "EVENT"); copyText(item.recordID, sizeof(item.recordID), event.eventID); queue_enqueue(&approvals, item); cout << "Event added and placed in approval queue.\n"; }
void Application::updateEvent() { string id = readText("Event ID to update: "); EventNode *e = event_find(events, id.c_str()); if (!e) { cout << "Event not found.\n"; return; } copyText(e->data.eventName, sizeof(e->data.eventName), readText("New name: ")); copyText(e->data.venue, sizeof(e->data.venue), readText("New venue: ")); copyText(e->data.date, sizeof(e->data.date), readText("New date: ")); copyText(e->data.description, sizeof(e->data.description), readText("New description: ")); cout << "Event updated.\n"; }
void Application::deleteEvent() { string id = readText("Event ID to delete: "); cout << (event_remove(&events, id.c_str()) ? "Event deleted.\n" : "Event not found.\n"); }

void Application::addStudent() { CStudent student{}; copyText(student.studentID, sizeof(student.studentID), readText("Student ID: ")); if (student_find(students, student.studentID)) { cout << "Student ID already exists.\n"; return; } copyText(student.name, sizeof(student.name), readText("Name: ")); copyText(student.branch, sizeof(student.branch), readText("Branch: ")); student.semester = readNumber("Semester (1-8): ", 1, 8); student_append(&students, student); string action = "STUDENT_ADD:" + string(student.studentID); stack_push(&actions, action.c_str()); cout << "Student added.\n"; }
void Application::updateStudent() { string id = readText("Student ID to update: "); StudentNode *s = student_find(students, id.c_str()); if (!s) { cout << "Student not found.\n"; return; } copyText(s->data.name, sizeof(s->data.name), readText("New name: ")); copyText(s->data.branch, sizeof(s->data.branch), readText("New branch: ")); s->data.semester = readNumber("New semester (1-8): ", 1, 8); cout << "Student updated.\n"; }
void Application::deleteStudent() { string id = readText("Student ID to delete: "); cout << (student_remove(&students, id.c_str()) ? "Student deleted.\n" : "Student not found.\n"); }
void Application::showApprovals() { ApprovalItem item{}; if (queue_is_empty(&approvals)) { cout << "Approval queue is empty.\n"; return; } cout << "PENDING APPROVALS (FIFO queue)\n"; while (queue_dequeue(&approvals, &item)) cout << "Approved: " << item.recordType << " " << item.recordID << "\n"; }
void Application::undoLastAction() { char action[160]; if (!stack_pop(&actions, action, sizeof(action))) { cout << "No add action to undo.\n"; return; } string value(action); if (value.rfind("POST_ADD:", 0) == 0) { post_remove(&posts, value.substr(9).c_str()); cout << "Undid last post addition.\n"; } else if (value.rfind("EVENT_ADD:", 0) == 0) { event_remove(&events, value.substr(10).c_str()); cout << "Undid last event addition.\n"; } else if (value.rfind("STUDENT_ADD:", 0) == 0) { student_remove(&students, value.substr(12).c_str()); cout << "Undid last student addition.\n"; } }
