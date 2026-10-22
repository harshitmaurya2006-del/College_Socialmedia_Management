COLLEGE SOCIAL MEDIA MANAGEMENT SYSTEM (C++)

FILES
- member1.cpp: Main menu and program entry point
- member2.cpp: Student dashboard, viewing and searching posts/events; linked list
- member3.cpp: Admin dashboard, adding posts and events
- member4.cpp: Registration, login, file loading and ID generation
- project.h: Shared data structures and function declarations
- user.txt: Registered student data (created/updated automatically)
- post.txt: Post records
- event.txt: Event records

HOW TO COMPILE (g++)
1. Keep all files in the same folder.
2. Open a terminal in that folder.
3. Run:
   g++ -std=c++17 member1.cpp member2.cpp member3.cpp member4.cpp -o college_social
4. Run on Windows:
   .\college_social.exe
   On Linux/macOS:
   ./college_social

ADMIN LOGIN
Username: admin
Password: admin123

DATA FORMAT
user.txt: name|username|password|roll
post.txt: id|title|content|author|date
event.txt: id|title|description|date|venue
Do not type the | character in fields; the program replaces it with /.

DSA CONCEPTS USED
- vector: stores records loaded from text files
- singly linked list: used to display latest posts
- sorting: orders posts by descending ID for latest posts
- linear search: checks usernames and searches post/event text
- traversal: displays records one by one

NOTES
- This is a beginner-friendly educational demo. Passwords are stored as plain text; do not use this design for a real deployed system.
- Post/event date is entered as text. Latest posts are determined by their increasing ID, not by interpreting calendar dates.
- Keep the .txt files in the same working directory as the executable.
