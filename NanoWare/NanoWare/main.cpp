#include <iostream>
#include <Windows.h>
#include <iomanip>
#include "memory/memory.h"
#include "sdk/sdk.h"
#include "sdk/offsets.hpp"

int main()
{
    DWORD pid = memory::get_pid(L"RobloxPlayerBeta.exe");
    if (!pid) {
        std::cout << "[-] roblox not found!\n";
        return 0;
    }

    if (!memory::attach(pid)) {
        std::cout << "[-] failed to attach!\n";
		return 0;
    }

    uintptr_t base = memory::get_base(pid, L"RobloxPlayerBeta.exe");
    uintptr_t datamodel = sdk::get_datamodel(base);
	uintptr_t visualengine = sdk::get_visualengine(base);
    uintptr_t workspace = sdk::find_first_child_by_class(datamodel, "Workspace");
    uintptr_t players = sdk::find_first_child_by_class(datamodel, "Players");
    uintptr_t lighting = sdk::find_first_child_by_class(datamodel, "Lighting");
    uintptr_t camera = sdk::get_camera(workspace);
    vector2 viewport = sdk::get_viewport_size(camera);

    std::cout << std::hex;
    std::cout << "[+] attached!\n";
    std::cout << "[~] base -> 0x" << base << '\n';
    std::cout << "[~] datamodel -> 0x" << datamodel << '\n';
	std::cout << "[~] visualengine -> 0x" << visualengine << '\n';
    std::cout << "[~] workspace -> 0x" << workspace << '\n';
    std::cout << "[~] players -> 0x" << players << '\n';
	std::cout << "[~] lighting -> 0x" << lighting << '\n';
    std::cout << "[~] camera -> 0x" << camera << '\n';
    std::cout << "[~] viewportsize -> "
        << viewport.x << " x " << viewport.y << '\n';

    view_matrix vm = memory::read<view_matrix>(
        visualengine + offsets::viewmatrix
    );
    
	return 0;
}