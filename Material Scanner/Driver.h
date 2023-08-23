#pragma once
#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <vector>
class memory
{
private:
    HANDLE c_handle = 0x00;
    DWORD c_id = 0x00;
public:
    bool write(const std::uint64_t& Address, void* Buffer, const std::size_t& Size);
    bool read(const std::uint64_t& Address, void* Buffer, const std::size_t& Size);
    std::uint64_t GetModuleBase(const char* Name);

    template<typename Type>
    bool write(const std::uint64_t& Address, const Type& Data)
    {
        return this->write(Address, (void)(&Data), sizeof(Data));
    }

    template<typename Type>
    Type read(const std::uint64_t& Address)
    {
        Type Data = Type();

        this->read(Address, (void*)(&Data), sizeof(Data));

        return Data;
    }

    template<typename Type>
    Type Read_Chain(const std::uint64_t& Address, std::vector<std::uint64_t> Offsets)
    {
        // Initilizing Value
        std::uint64_t Value = Address;
        // Reading The Offsets Into The Value
        for (int i = 0; i < Offsets.size() - 1; i++)
        {
            const std::uint64_t& Offset = Offsets[i];

            Value = this->read<std::uint64_t>(Value + Offset);
        }

        // Returning The Value + Final Offset
        return this->read<Type>(Value + Offsets[Offsets.size() - 1]);
    }

    std::string Read_W_String(const std::uint64_t& Address)
    {
        if (!Address)
            return "";

        std::uint64_t NewAddress = this->read<std::uint64_t>(Address);
        if (!NewAddress)
            return "";

        int Length = this->read<int>(NewAddress + 0x10) * sizeof(wchar_t);
        if (!Length)
            return "";

        // Array Of Characters To Read Into
        wchar_t* Buffer = (wchar_t*)malloc(Length);

        this->read(NewAddress + 0x14, Buffer, Length);

        std::wstring TempString = std::wstring(Buffer, Length / 2);

        free(Buffer);

        return std::string(TempString.begin(), TempString.end());
    }

    const char* Read_String(const std::uint64_t& Address)
    {
        std::uint64_t NewAddress = this->read<std::uint64_t>(Address);

        // Array Of Characters To Read Into
        std::unique_ptr<char[]> Buffer(new char[128]);

        // Reading The Characters Into The Array
        this->read(NewAddress, Buffer.get(), 128);

        // Returning Array Of Characters As A String
        return Buffer.get();
    }

    bool Attach(const char* Name);
    std::uint64_t Game_Assembly = 0x00;
}; inline memory* mem = new memory();