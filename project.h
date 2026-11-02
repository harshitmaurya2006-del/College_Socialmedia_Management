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
