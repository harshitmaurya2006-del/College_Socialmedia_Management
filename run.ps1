gcc -Iinclude -c src/c_structures.c -o c_structures.o
g++ -std=c++17 -Iinclude -c src/models.cpp src/search_sort.cpp src/application.cpp src/main.cpp
g++ c_structures.o models.o search_sort.o application.o main.o -o college_social_media.exe
if ($LASTEXITCODE -eq 0) { .\college_social_media.exe }
