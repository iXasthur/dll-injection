#include <iostream>
#include <thread>
#include <string>
#include "windows.h"

std::string s = "12345";

void loop() {
    std::cout << "Starting loop (EXIT to break)" << std::endl;

    while (true) {
        if (s != "EXIT") {
            std::cout << s << std::endl;
            Sleep(1000);
        } else {
            break;
        }
    }
}

int main() {
    pid_t pid = _getpid();
    std::cout << "Process pid: " << pid << std::endl;

    std::thread t1(loop);
    t1.join();

    return 0;
}
