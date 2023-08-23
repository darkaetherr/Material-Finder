#include "Driver.h"
#include <TlHelp32.h>

bool memory::write(const std::uint64_t& Address, void* Buffer, const std::size_t& Size)
{
    return WriteProcessMemory(this->c_handle, (void*)(Address), Buffer, Size, nullptr);
}

bool memory::read(const std::uint64_t& Address, void* Buffer, const std::size_t& Size)
{
    return ReadProcessMemory(this->c_handle, (void*)(Address), Buffer, Size, nullptr);
}

std::uint64_t memory::GetModuleBase(const char* Name)
{
    HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->c_id);

    if (Snapshot == INVALID_HANDLE_VALUE)
    {
        return 0x00;
    }

    MODULEENTRY32 Entry;
    Entry.dwSize = sizeof(Entry);

    if (!Module32First(Snapshot, &Entry))
    {
        return 0x00;
    }

    do
    {
        if (!std::strcmp((const char*)Entry.szModule, Name))
        {
            CloseHandle(Snapshot);
            return (std::uint64_t)(Entry.modBaseAddr);
        }

    } while (Module32Next(Snapshot, &Entry));

    CloseHandle(Snapshot);

    return 0x00;
}

bool memory::Attach(const char* Name)
{
    PROCESSENTRY32 Entry;
    Entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Snapshot == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    if (!Process32First(Snapshot, &Entry))
    {
        CloseHandle(Snapshot);
        return false;
    }

    while (Process32Next(Snapshot, &Entry))
    {
        if (strcmp(Entry.szExeFile, Name) == 0)
        {
            HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Entry.th32ProcessID);
            if (ProcessHandle != NULL)
            {
                this->c_handle = ProcessHandle;
                this->c_id = Entry.th32ProcessID;
                CloseHandle(Snapshot);
                return true;
            }
        }
    }

    CloseHandle(Snapshot);
    return false;
}
