# PBL Project Guide

## 1. Exact Windows terminal commands

Open the VS Code terminal and run:

```powershell
cd E:\pbl\college_social_media
gcc -Iinclude -c src/c_structures.c -o c_structures.o
g++ -std=c++17 -Iinclude -c src/models.cpp src/search_sort.cpp src/application.cpp src/main.cpp
g++ c_structures.o models.o search_sort.o application.o main.o -o college_social_media.exe
.\college_social_media.exe
```

`gcc` compiles the C module. `g++` compiles the C++ modules and links all object files into one executable. MinGW-w64 must be installed and `gcc`/`g++` must be available on PATH.

A build-and-run script is also provided:

```powershell
.\run.ps1
```

## 2. File-by-file explanation

- `include/c_structures.h`: C record definitions, linked-list node types, stack API and queue API.
- `src/c_structures.c`: Manual C implementations for append, find, remove and free operations; stack push/pop; FIFO queue enqueue/dequeue.
- `include/models.h`: C++ class declarations for `User`, `Student`, `Coordinator`, `Admin`, `Post` and `Event`.
- `src/models.cpp`: Constructors, getters and virtual dashboard behavior for the C++ classes.
- `include/search_sort.h`: Search and sorting function declarations.
- `src/search_sort.cpp`: Bubble-sort functions by ID/title/name/date and binary search functions for exact post/event IDs.
- `include/application.h`: `Application` class state and menu operation declarations.
- `src/application.cpp`: Sample data, login, role dashboards, CRUD operations, validation, display, approval processing and undo.
- `src/main.cpp`: Creates an `Application` object and starts the program.
- `README.md`: Short setup reference.
- `run.ps1`: Rebuilds and launches the program.

## 3. Where each C data structure is used

- Arrays/structures: `CStudent`, `CPost`, `CEvent` are C structures containing fixed-size character arrays and an integer semester. They represent one record without a database.
- Linked lists: `StudentNode`, `PostNode` and `EventNode` store dynamic student, post and event records. Add, find, delete and free operations are manually implemented in `c_structures.c`.
- Stack: `ActionStack` records `POST_ADD`, `EVENT_ADD` and `STUDENT_ADD` actions. The coordinator/admin undo option pops the latest action and removes that newly added record.
- Queue: `ApprovalQueue` stores a record type and ID whenever a coordinator/admin adds a post or event. Admin option 8 processes items FIFO.
- Linear search: Keyword searches scan every post title or event name. Linked-list `*_find` functions also scan records by ID.
- Binary search: Exact post/event ID searches first sort the linked list by ID, copy node pointers into a temporary C++ vector, and perform binary search on the sorted sequence.
- Sorting: Bubble sort is implemented manually and can sort posts by ID/title/date and events by ID/name/date. Dates use `YYYY-MM-DD`, so string order matches date order.

## 4. OOP concepts demonstrated

- Classes and objects: `Application`, `Student`, `Post`, `Event`, `Coordinator` and `Admin` are classes. `main.cpp` creates an `Application` object.
- Constructors: Every domain class initializes its fields through a constructor. For example, `Student` calls the `User` constructor for common identity data.
- Encapsulation: Fields in the model classes are private or protected. Code reads them through public getters; the application does not directly change a `Post` object's private fields.
- Inheritance: `Student`, `Coordinator` and `Admin` inherit from the abstract-style `User` base class. They share ID/name data and the `dashboardName` contract.
- Polymorphism: `login()` returns `std::unique_ptr<User>`. The program calls virtual `dashboardName()` through the base type and uses `dynamic_cast` to select the role-specific dashboard. `User` also has a virtual destructor.
- Natural division: C++ objects demonstrate OOP identity and behavior, while C records remain the simple storage layer required for manual data-structure practice.

## 5. Sample terminal workflows

### Student

1. Login with `S1001` and `student`.
2. Choose `1` to view sample notices.
3. Choose `2` to view events.
4. Choose `3`, enter `P101`, and observe exact-ID binary search.
5. Choose `5`, then `2`, to sort notices by title.
6. Choose `0` to logout.

### Coordinator

1. Login with `C2001` and `coord`.
2. Choose `2` and enter a new ID, title, content, category and `YYYY-MM-DD` date.
3. The record is appended to the post linked list and placed in the approval queue.
4. Choose `3` to update a post, or `4` to delete one.
5. Choose `6` to add an event and `7`/`8` to update/delete events.
6. Choose `10` immediately after an add to undo the latest add using the stack.
7. Choose `0` to logout.

### Admin

1. Login with `A3001` and `admin`.
2. Choose `1` to view students, or `2`, `3`, `4` to add/update/delete students.
3. Choose `5` or `6` to manage posts/events.
4. Choose `7` to view and search/sort all record categories.
5. Choose `8` to process pending post/event approvals in FIFO order.
6. Choose `9` to undo the latest add.
7. Choose `0` to logout.

## 6. Basic test cases

| Test | Steps | Expected result |
|---|---|---|
| Valid student login | `S1001`, `student` | Student dashboard opens and sample records are available |
| Valid coordinator login | `C2001`, `coord` | Coordinator CRUD menu opens |
| Valid admin login | `A3001`, `admin` | Admin student/record management menu opens |
| Invalid login | Any wrong password, then `0` | Login failure is shown and program exits |
| Duplicate ID | Add a record using `P101`, `E201` or `S1001` | Duplicate is rejected |
| Empty input | Submit an empty required field | Program asks again |
| Exact search | Search for `P101` or `E201` | Binary search returns the record |
| Keyword search | Search for `Library` or `Cultural` | Linear keyword search returns the matching record |
| Sorting | Sort posts by title or events by date | Records display in selected order |
| Queue | Add a post/event, then admin option 8 | Pending records display and are removed FIFO |
| Undo | Add a record, select undo | The latest added record is removed |
| Update/delete | Update or delete a valid and invalid ID | Valid operation succeeds; missing ID is reported |
| Input range | Enter semester `0` or `9` | Program rejects it and requests 1 through 8 |

## 7. Viva questions and concise answers

1. **What is the aim of this project?** It manages college students, notices/posts, events and admin/coordinator records through a terminal menu.
2. **Why is there no database?** The proposal requires simple C/C++ file handling if persistence is needed; this version uses in-memory records and sample data to stay beginner-friendly.
3. **Why are both C and C++ used?** C implements the required data structures manually; C++ supplies classes, inheritance, polymorphism and the application flow.
4. **Where is the linked list used?** Students, posts and events are stored as separate singly linked lists.
5. **Why use a linked list?** Records can be added or deleted dynamically without a fixed record-count array.
6. **What does the stack store?** Recent add actions, so the latest addition can be undone.
7. **What does the queue store?** Pending post/event approval items in first-in-first-out order.
8. **What is the difference between linear and binary search here?** Keyword searches scan nodes linearly; exact IDs use binary search after sorting by ID.
9. **Why must binary search data be sorted?** Binary search decides whether to move left or right based on order, so unsorted data would give incorrect results.
10. **Which sorting algorithm is implemented?** A simple bubble sort that swaps record data in linked-list nodes.
11. **How are dates sorted?** Dates are entered as `YYYY-MM-DD`; lexicographic order then matches chronological order.
12. **What is encapsulation in this project?** Model fields are hidden inside classes and accessed through public getters.
13. **Where is inheritance used?** Student, Coordinator and Admin derive from User.
14. **Where is polymorphism used?** A `unique_ptr<User>` refers to a derived role and calls the virtual `dashboardName()` implementation.
15. **Why is `User` a base class?** All roles have a common ID and name, while each role has its own dashboard behavior.
16. **How is input validated?** Required text cannot be empty, numeric choices are range-checked, and duplicate IDs are rejected.
17. **How does role verification work?** The program compares a simple demo ID/password pair and then creates the matching role object.
18. **What happens when the program exits?** Destructors and C free functions release all linked-list nodes, stack strings and queue nodes.
19. **What is the time complexity of linked-list linear search?** $O(n)$ because nodes may be checked one by one.
20. **What is the time complexity of binary search?** $O(\log n)$ after sorting and preparing the searchable sequence; the current implementation also pays for the sort and temporary pointer-vector preparation.
