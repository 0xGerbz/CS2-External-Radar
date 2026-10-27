#include "game.h"
#include <iostream>

void Game::getMap() {
	//Based on the cs2 module dumper - we know the map name is found in matchmaking dll
	//We must find the base address of the module to set up our memory read. The base is our starting place
	uintptr_t matchmakingBase = memory::getModuleBase(L"matchmaking.dll");
	//We find this structure in the cs2 dumper
	uintptr_t mapPtr = memory::memRead<uintptr_t>(matchmakingBase + matchmaking_dll::dwGameTypes + matchmaking_dll::dwGameTypes_mapName);
	//Read our map directly in to our structure
	memory::memRead(mapPtr + 0x4, &map, sizeof(map));
	std::cout << map << std::endl;
}