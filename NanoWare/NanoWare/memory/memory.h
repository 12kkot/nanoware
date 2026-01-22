#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <vector>

namespace memory
{
    extern HANDLE process;

    DWORD get_pid(const wchar_t* process_name);
    uintptr_t get_base(DWORD pid, const wchar_t* module_name);
    bool attach(DWORD pid);
    std::string read_string(uintptr_t address);

    template<typename T>
    T read(uintptr_t address)
    {
        T buffer{};
        ReadProcessMemory(
            process,
            (LPCVOID)address,
            &buffer,
            sizeof(T),
            nullptr
        );
        return buffer;
    }
}