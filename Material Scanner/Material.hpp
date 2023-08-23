#pragma once
#include <Windows.h>
#include <functional>
#include <thread>
#include <string>
#include <math.h>
#include <algorithm>
#include "Driver.h"
#include <fstream>
namespace Scanner
{
    class MaterialScanner
    {
    private:
    public:
        uint64_t c_Get_Material_Address() { return reinterpret_cast<uint64_t>(this); }
        bool c_Is_Material_Valid() { return mem->read<uint64_t>(c_Get_Material_Address() + 0x88) == 16045690984833335023; }
        std::string c_Get_Material_Name() { return mem->Read_String(c_Get_Material_Address() + 0x30); }
        static void c_Scan_Materials(const uint64_t& Address, const uint64_t& Range = 0x300, const int& Index = 3, std::vector<uint64_t> Offset = std::vector<uint64_t>())
        {
            if (!Address || Index == 0x00) { return; }
            for (std::uint64_t Value = 0x00; Value < Range; Value += 0x8)
            {
                const std::uint64_t Pointer = mem->read<std::uint64_t>(Address + Value);
                if (!Pointer) { continue; }
                Scanner::MaterialScanner* Material = mem->read<Scanner::MaterialScanner*>(Pointer + 0x10);
                if (!Material) { continue; }
                if (Material->c_Is_Material_Valid())
                {
                    std::printf("  %s -> { ", Material->c_Get_Material_Name().c_str());
                    for (std::uint64_t Entry : Offset) { std::cout << std::hex << "0x" << Entry << std::dec << ", "; }
                    std::cout << std::hex << "0x" << Value << std::dec << " }\n";
                    std::ofstream outputFile("Valid Mat.txt", std::ios::app);
                    if (outputFile.is_open()) {
                        std::string materialName = Material->c_Get_Material_Name();
                        outputFile << materialName << " -> { ";
                        for (std::uint64_t entry : Offset) {
                            outputFile << std::hex << "0x" << entry << std::dec << ", ";
                        }
                        outputFile << std::hex << "0x" << Value << std::dec << " }\n";
                        outputFile.close();
                    }
                }
                else
                {
                    std::vector<std::uint64_t> Temp = Offset;
                    Temp.push_back(Value);
                    Scanner::MaterialScanner::c_Scan_Materials(Pointer, Range, Index - 1, Temp);

                }
            }
        }
    };
}