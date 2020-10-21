#include <iostream>
#include <algorithm>
#include <vector>
#include "windows.h"

void change(pid_t pid, std::string source, std::string replacement) {
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (processHandle != nullptr) {
        MEMORY_BASIC_INFORMATION memInfo;

        unsigned char *addr = nullptr;
        while (VirtualQueryEx(processHandle, addr, &memInfo, sizeof(memInfo))) {
            if (memInfo.State == MEM_COMMIT && (memInfo.Type == MEM_MAPPED || memInfo.Type == MEM_PRIVATE)) {
                size_t bytesRead;
                std::vector<char> buffer;
                buffer.resize(memInfo.RegionSize);
                ReadProcessMemory(processHandle, addr, buffer.data(), memInfo.RegionSize, &bytesRead);
                buffer.resize(bytesRead);

                auto offset = std::search(buffer.begin(), buffer.end(), source.begin(), source.end());
                if (offset != buffer.end()) {
                    int distance = std::distance(buffer.begin(), offset);
                    LPVOID addrToWriteTo = addr + distance;

                    std::cout << "Found string at " << addrToWriteTo << std::endl;

                    if (WriteProcessMemory(processHandle, addrToWriteTo, replacement.data(), replacement.length() + 1,
                                           nullptr)) {
                        std::cout << "Successfully changed " << source << " to " << replacement << std::endl;
                    } else {
                        DWORD error = GetLastError();
                        std::cout << "Error writing to memory " << error << std::endl;
                    }
                }
            }
            addr += memInfo.RegionSize;
        }

        CloseHandle(processHandle);
    } else {
        std::cout << "Invalid pid " << pid << std::endl;
    }
}

int main() {
    pid_t pid = -1;
    std::string source = "_12345";
    std::string replacement = "^_^";
    change(pid, source, replacement);
    return 0;
}
