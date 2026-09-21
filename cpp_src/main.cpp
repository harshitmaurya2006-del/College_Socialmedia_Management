#include "college_system.h"

// Starts the application and lets CollegeSystem own its complete lifetime.
int main()
{
    CollegeSystem system;
    system.loadAll();
    system.run();
    return 0;
}
