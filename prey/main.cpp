#include <iostream>
#include <thread>
#include "windows.h"

char s[] = "12345";

[[noreturn]] void loop() {
    std::cout << "Starting loop" << std::endl;

    while (true) {
        std::cout << s << std::endl;
        Sleep(1000);
    }
}

int main() {
    pid_t pid = _getpid();
    std::cout << "Process pid: " << pid << std::endl;

    std::thread t1(loop);
    t1.join();

    return 0;
}
