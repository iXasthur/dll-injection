#include <iostream>
#include <utility>
#include "windows.h"
#include "../arrow/arrow.h"

void staticAttack(pid_t pid, std::string source, std::string replacement) {
    attack(pid, std::move(source), std::move(replacement));
}

void dynamicAttack(pid_t pid, std::string source, std::string replacement){
    typedef void(*change)(pid_t pid, std::string source, std::string replacement);

    const char *lib = "../../arrow/cmake-build-debug/libarrow.dll";
    HMODULE libraryHandle = LoadLibraryA(lib);
    if (!libraryHandle) {
        std::cout << "Can't load library" << std::endl;
        return;
    }

    auto func = (change)GetProcAddress(libraryHandle, "attack");
    if (!func) {
        std::cout << "Can't find the function" << std::endl;
        std::cout << GetLastError() << std::endl;
        return;
    }

    func(pid, std::move(source), std::move(replacement));
}

int main() {
    pid_t pid = -1;

    staticAttack(pid, "_12345", "^_^");
    dynamicAttack(pid, "^_^", "_12345");

    return 0;
}
