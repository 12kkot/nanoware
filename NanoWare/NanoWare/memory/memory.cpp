#include "memory.h"
#include <TlHelp32.h>
#include <vector>

HANDLE memory::process = nullptr;

namespace
{
    struct handle_guard
    {
        HANDLE h;
        ~handle_guard()
        {
            if (h && h != INVALID_HANDLE_VALUE)
                CloseHandle(h);
        }
    };
}

DWORD memory::get_pid(const wchar_t* process_name)
{
    PROCESSENTRY32W pe{ sizeof(pe) };
    handle_guard snap{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

    if (snap.h == INVALID_HANDLE_VALUE)
        return 0;

    for (BOOL ok = Process32FirstW(snap.h, &pe); ok; ok = Process32NextW(snap.h, &pe))
        if (!wcscmp(pe.szExeFile, process_name))
            return pe.th32ProcessID;

    return 0;
}

uintptr_t memory::get_base(DWORD pid, const wchar_t* module_name)
{
    if (!process || process == INVALID_HANDLE_VALUE)
        return 0;

    DWORD actual_pid = GetProcessId(process);
    
    MODULEENTRY32W me{ sizeof(me) };
    handle_guard snap{
        CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, actual_pid)
    };

    if (snap.h == INVALID_HANDLE_VALUE)
        return 0;

    for (BOOL ok = Module32FirstW(snap.h, &me); ok; ok = Module32NextW(snap.h, &me))
        if (!wcscmp(me.szModule, module_name))
            return reinterpret_cast<uintptr_t>(me.modBaseAddr);

    return 0;
}

bool memory::attach(DWORD pid)
{
    process = OpenProcess(
        PROCESS_ALL_ACCESS,
        FALSE,
        pid
    );
    return process != nullptr;
}

std::string memory::read_string(uintptr_t address)
{
    if (!process || process == INVALID_HANDLE_VALUE)
        return std::string();

    int32_t str_length = read<int32_t>(address + 0x18);
    
    if (str_length <= 0 || str_length > 1000)
        return std::string();

    if (str_length >= 16)
    {
        address = read<uintptr_t>(address);
        if (address == 0)
            return std::string();
    }

    int32_t buffer_size = (str_length + 1 < 256) ? str_length + 1 : 256;
    std::vector<char> buffer(buffer_size);
    
    SIZE_T bytes_read = 0;
    if (ReadProcessMemory(process, reinterpret_cast<void*>(address), buffer.data(), buffer.size() - 1, &bytes_read))
    {
        buffer[bytes_read] = '\0';
        return std::string(buffer.data());
    }

    return std::string();
}