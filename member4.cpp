#include "project.h"

string cleanField(string value) {
    // The text files use | as a field separator, so replace it in user input.
    replace(value.begin(), value.end(), '|', '/');
    return value;
}

vector<User> loadUsers() {
    vector<User> users;
    ifstream file("user.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line); User u;
        if (getline(ss, u.name, '|') && getline(ss, u.username, '|') &&
            getline(ss, u.password, '|') && getline(ss, u.roll, '|'))
                users.push_back(u);
    }
    return users;
}

vector<Post> loadPosts() {
    vector<Post> posts;
    ifstream file("post.txt");
    string line;
    while (getline(file, line)){
        stringstream ss(line); string id; Post p;
        if (getline(ss, id, '|') && getline(ss, p.title, '|') && getline(ss, p.content, '|') &&
            getline(ss, p.author, '|') && getline(ss, p.date)){
                try { p.id = stoi(id); posts.push_back(p); }
                catch (...){ /* skip invalid record */ }
        }
    }
    return posts;
}


vector<Event> loadEvents() {
    vector<Event> events;
    ifstream file("event.txt"); string line;
    while (getline(file, line)) {
        stringstream ss(line); string id; Event e;
        if (getline(ss, id, '|') && getline(ss, e.title, '|') && getline(ss, e.description, '|') &&
            getline(ss, e.date, '|') && getline(ss, e.venue)) {
            try { e.id = stoi(id); events.push_back(e); } catch (...) { /* skip invalid record */ }
        }
    }
    return events;
}

bool registerStudent() {
    User u;
    cout << "Enter full name: "; getline(cin, u.name);
    cout << "Choose username: "; getline(cin, u.username);
    cout << "Choose password: "; getline(cin, u.password);
    cout << "Enter roll number: "; getline(cin, u.roll);
    u.name = cleanField(u.name); u.username = cleanField(u.username);
    u.password = cleanField(u.password); u.roll = cleanField(u.roll);
    if (u.name.empty() || u.username.empty() || u.password.empty() || u.roll.empty()) {
        cout << "All fields are required. Registration cancelled.\n"; return false;
    }
    vector<User> users = loadUsers();
    for (const User &existing : users) {
        if (existing.username == u.username) { cout << "Username already exists. Please choose another.\n"; return false; }
    }
    ofstream file("user.txt", ios::app);
    if (!file) { cout << "Could not open user.txt.\n"; return false; }
    file << u.name << '|' << u.username << '|' << u.password << '|' << u.roll << '\n';
    cout << "Registration successful. You can now log in.\n";
    return true;
}

bool studentLogin(User &loggedIn) {
    string username, password;
    cout << "Username: "; getline(cin, username);
    cout << "Password: "; getline(cin, password);
    vector<User> users = loadUsers();
    for (const User &u : users) {
        if (u.username == username && u.password == password) {
            loggedIn = u; cout << "Login successful.\n"; return true;
        }
    }
    cout << "Incorrect username or password.\n";
    return false;
}

bool adminLogin() {
    string username, password;
    cout << "Admin username: "; getline(cin, username);
    cout << "Admin password: "; getline(cin, password);
    // Demo credentials for the college project.
    if (username == "admin" && password == "admin123") {
        cout << "Admin login successful.\n"; return true;
    }
    cout << "Incorrect admin credentials.\n";
    return false;
}

int nextPostId() {
    int next = 1;
    for (const Post &p : loadPosts()) next = max(next, p.id + 1);
    return next;
}
int nextEventId() {
    int next = 1;
    for (const Event &e : loadEvents()) next = max(next, e.id + 1);
    return next;
}
