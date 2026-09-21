#include "utils.h"
#include "algo.h"
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>

namespace utils {

// Reads an integer inside an inclusive range and retries invalid input.
int readInt(const std::string &prompt, int minimum, int maximum)
{
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minimum && value <= maximum) {
            std::cin.ignore(10000, '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Please enter a number from " << minimum << " to " << maximum << ".\n";
    }
}

// Reads text with length limits and rejects the file delimiter.
std::string readLine(const std::string &prompt, int minimumLength, int maximumLength)
{
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (static_cast<int>(value.size()) >= minimumLength && static_cast<int>(value.size()) <= maximumLength && value.find('|') == std::string::npos) return value;
        std::cout << "Input must contain " << minimumLength << " to " << maximumLength << " characters and no '|'.\n";
    }
}

// Reads and validates a YYYY-MM-DD date.
std::string readDate(const std::string &prompt)
{
    std::string value;
    while (true) {
        value = readLine(prompt, 10, 10);
        if (date_is_valid(value.c_str())) return value;
        std::cout << "Enter a valid date in YYYY-MM-DD format.\n";
    }
}

// Reads a yes/no answer and returns true for yes.
bool readYesNo(const std::string &prompt)
{
    std::string answer;
    while (true) {
        answer = readLine(prompt, 1, 1);
        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
        std::cout << "Please answer y or n.\n";
    }
}

// Returns today's local date as YYYY-MM-DD.
std::string todayString()
{
    std::time_t now = std::time(NULL);
    std::tm *local = std::localtime(&now);
    char buffer[DATE_SIZE];
    if (local == NULL) return "1900-01-01";
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", local);
    return std::string(buffer);
}

// Copies a C++ string into a bounded C buffer.
void copyToArray(char *destination, std::size_t size, const std::string &source)
{
    if (destination == NULL || size == 0) return;
    std::strncpy(destination, source.c_str(), size - 1U);
    destination[size - 1U] = '\0';
}

// Pauses until the user presses Enter.
void pause()
{
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

// Clears the terminal using the platform command.
void clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

// Prints a horizontal separator.
void printLine()
{
    std::cout << "------------------------------------------------------------\n";
}

// Prints a simple section heading.
void printHeader(const std::string &title)
{
    printLine();
    std::cout << title << '\n';
    printLine();
}

}
