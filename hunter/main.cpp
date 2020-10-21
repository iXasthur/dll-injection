#include <iostream>
#include <algorithm>
#include <vector>
#include "windows.h"

int main() {
    pid_t pid = 2400;
    std::string stringToReplace = "12345";
    std::string stringToReplaceTo = "I wanna be a multiplexor";
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

                auto offset = std::search(buffer.begin(), buffer.end(), stringToReplace.begin(), stringToReplace.end());
                if (offset != buffer.end()) {
                    int distance = std::distance(buffer.begin(), offset);
                    LPVOID addrToWriteTo = addr + distance;

                    std::cout << "Found string at " << addrToWriteTo << std::endl;

                    if (WriteProcessMemory(processHandle, addrToWriteTo, stringToReplaceTo.data(), stringToReplaceTo.length() + 1,
                                           nullptr)) {
                        std::cout << "Changed" << std::endl;
                    } else {
                        DWORD error = GetLastError();
                        std::cout << "Error " << error << std::endl;
                    }
                }
            }
            addr += memInfo.RegionSize;
        }

        CloseHandle(processHandle);
    } else {
        std::cout << "Invalid pid " << pid << std::endl;
    }

    return 0;
}
