#ifndef ARROW_LIBRARY_H
#define ARROW_LIBRARY_H

#include <iostream>
#include "windows.h"

extern "C" void attack(pid_t pid, std::string source, std::string replacement);

#endif //ARROW_LIBRARY_H
