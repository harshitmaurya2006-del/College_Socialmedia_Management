#include "project.h"

// A simple singly linked list is used to display posts in newest-ID-first order.
struct PostNode {
    Post data;
    PostNode *next;
    PostNode(const Post &p) : data(p), next(nullptr) {}
};

static void printPost(const Post &p) {
    cout << "\nPost ID: " << p.id << "\nTitle: " << p.title
         << "\nContent: " << p.content << "\nPosted by: " << p.author
         << "\nDate: " << p.date << "\n-----------------------------\n";
}

void showAllPosts() {
    vector<Post> posts = loadPosts();
    if (posts.empty()) { cout << "No posts available yet.\n"; return; }
    cout << "\n========== ALL POSTS ==========\n";
    for (const Post &p : posts) printPost(p);
}

void showLatestPosts() {
    vector<Post> posts = loadPosts();
    if (posts.empty()) { cout << "No posts available yet.\n"; return; }
    sort(posts.begin(), posts.end(), [](const Post &a, const Post &b) { return a.id > b.id; });

    PostNode *head = nullptr;
    PostNode *tail = nullptr;
    for (const Post &p : posts) {
        PostNode *node = new PostNode(p);
        if (head == nullptr) head = node;
        else tail->next = node;
        tail = node;
    }
    cout << "\n========== LATEST POSTS (up to 5) ==========\n";
    PostNode *current = head;
    int count = 0;
    while (current && count < 5) {
        printPost(current->data);