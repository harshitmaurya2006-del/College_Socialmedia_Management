# College Social Media Management System

A second-year PBL command-line application using C for manually implemented data structures and C++ for OOP and application flow.

## Project structure

- `include/c_structures.h`, `src/c_structures.c`: C records, linked lists, stack and queue.
- `include/models.h`, `src/models.cpp`: C++ classes for User, Student, Coordinator, Admin, Post and Event.
- `include/search_sort.h`, `src/search_sort.cpp`: linear counts, bubble sorting and binary search helpers.
- `include/application.h`, `src/application.cpp`: menus, sample data, login, CRUD and role behavior.
- `src/main.cpp`: program entry point.

## Build on Windows

Open the VS Code terminal in this folder. Install MinGW-w64 and make sure `gcc` and `g++` are on PATH.

```powershell
gcc -Iinclude -c src/c_structures.c -o c_structures.o
g++ -std=c++17 -Iinclude -c src/models.cpp src/search_sort.cpp src/application.cpp src/main.cpp
g++ c_structures.o models.o search_sort.o application.o main.o -o college_social_media.exe
```

Run:

```powershell
.\college_social_media.exe
```

The application has sample records and needs no database or external library.

## Demo accounts

- Student: `S1001` / `student`
- Coordinator: `C2001` / `coord`
- Admin: `A3001` / `admin`

Use dates in `YYYY-MM-DD` format so lexicographic sorting matches chronological order.
