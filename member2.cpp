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