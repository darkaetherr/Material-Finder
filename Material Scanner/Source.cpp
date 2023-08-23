#pragma once
#include <fstream>
#include "Driver.h"
#include "Material.hpp"
#include "json.hpp"
uintptr_t scan_the_game_class;
int main()
{
    mem->Attach("RustClient.exe");
    mem->Game_Assembly = mem->GetModuleBase("GameAssembly.dll");
    while (true)
    {
        std::cout << ("\n Enter The Class You Want To Scan: ");
        std::cin >> scan_the_game_class;
        uintptr_t c_scan_pointer = mem->Read_Chain<uintptr_t>(mem->Game_Assembly, { scan_the_game_class, 0xB8 });
        Scanner::MaterialScanner::c_Scan_Materials(c_scan_pointer, 9768, 3);
    }
}