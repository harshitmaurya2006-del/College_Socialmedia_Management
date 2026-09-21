#ifndef CPP_UTILS_H
#define CPP_UTILS_H

#include <cstddef>
#include <string>

namespace utils {

// Reads an integer inside an inclusive range and retries invalid input.
int readInt(const std::string &prompt, int minimum, int maximum);
// Reads text with length limits and rejects the file delimiter.
std::string readLine(const std::string &prompt, int minimumLength, int maximumLength);
// Reads and validates a YYYY-MM-DD date.
std::string readDate(const std::string &prompt);
// Reads a yes/no answer and returns true for yes.
bool readYesNo(const std::string &prompt);
// Returns today's local date as YYYY-MM-DD.
std::string todayString();
// Copies a C++ string into a bounded C buffer.
void copyToArray(char *destination, std::size_t size, const std::string &source);
// Pauses until the user presses Enter.
void pause();
// Clears the terminal using the platform command.
void clearScreen();
// Prints a horizontal separator.
void printLine();
// Prints a simple section heading.
void printHeader(const std::string &title);

}

#endif
