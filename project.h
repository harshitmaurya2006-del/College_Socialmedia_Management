#ifndef PROJECT_H
#define PROJECT_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct User { string name, username, password, roll; };
struct Post { int id; string title, content, author, date; };
struct Event { int id; string title, description, date, venue; };

string cleanField(string value);
vector<User> loadUsers();
vector<Post> loadPosts();
vector<Event> loadEvents();
bool registerStudent();
bool studentLogin(User &loggedIn);
bool adminLogin();
void studentDashboard(const User &student);
void adminDashboard();
void showAllPosts();
void showLatestPosts();
void showAllEvents();
void searchPosts();
void searchEvents();
void addPost();
void addEvent();
int nextPostId();
int nextEventId();
#endif
