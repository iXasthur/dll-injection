#include <iostream>
#include <thread>
#include "windows.h"

#define LOOP_STRING "_CHANGE_ME_"

char *s;

[[noreturn]] void loop() {
    std::cout << "Starting loop" << std::endl;

    while (true) {
        std::cout << s << std::endl;
        Sleep(1000);
    }
}

int main() {
    s = (char*)calloc(1024, sizeof(char));
    strcpy(s, LOOP_STRING);

    pid_t pid = _getpid();
    std::cout << "Process pid: " << pid << std::endl;

    std::thread t1(loop);
    t1.join();

    return 0;
}
