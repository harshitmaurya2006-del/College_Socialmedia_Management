#include "project.h"

string cleanField(string value) {
    // The text files use | as a field separator, so replace it in user input.
    replace(value.begin(), value.end(), '|', '/');
    return value;
}
