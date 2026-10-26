#include "project.h"

void addPost() {
    Post p;
    p.id = nextPostId();
    cout << "Enter post title: "; getline(cin, p.title);
    cout << "Enter post content (one line): "; getline(cin, p.content);
    cout << "Enter author/admin name: "; getline(cin, p.author);
    cout << "Enter date (e.g. 17-10-2026): "; getline(cin, p.date);
    p.title = cleanField(p.title); p.content = cleanField(p.content);
    p.author = cleanField(p.author); p.date = cleanField(p.date);
    ofstream file("post.txt", ios::app);
    if (!file) { cout << "Could not open post.txt.\n"; return; }
    file << p.id << '|' << p.title << '|' << p.content << '|' << p.author << '|' << p.date << '\n';
    cout << "Post published successfully.\n";
}
