CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

C_OBJECTS = ds.o algo.o storage.o
CPP_OBJECTS = utils.o content.o user.o college_system.o main.o

all: college_social_media

college_social_media: $(C_OBJECTS) $(CPP_OBJECTS)
	$(CXX) -std=c++17 -o $@ $(C_OBJECTS) $(CPP_OBJECTS)

ds.o: c_src/ds.c include/records.h include/ds.h
	$(CC) $(CFLAGS) -c $< -o $@

algo.o: c_src/algo.c include/records.h include/ds.h include/algo.h
	$(CC) $(CFLAGS) -c $< -o $@

storage.o: c_src/storage.c include/records.h include/ds.h include/storage.h
	$(CC) $(CFLAGS) -c $< -o $@

utils.o: cpp_src/utils.cpp cpp_src/utils.h include/algo.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

content.o: cpp_src/content.cpp cpp_src/content.h cpp_src/utils.h include/records.h include/algo.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

user.o: cpp_src/user.cpp cpp_src/user.h cpp_src/college_system.h cpp_src/utils.h include/records.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

college_system.o: cpp_src/college_system.cpp cpp_src/college_system.h cpp_src/content.h cpp_src/user.h cpp_src/utils.h include/storage.h include/algo.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: cpp_src/main.cpp cpp_src/college_system.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

c_tests: tests/test_c.c $(C_OBJECTS)
	$(CC) $(CFLAGS) tests/test_c.c $(C_OBJECTS) -o test_c

run: college_social_media
	./college_social_media

clean:
	-del /Q *.o college_social_media.exe college_social_media test_c test_c_cppcheck 2>NUL

.PHONY: all c_tests run clean
