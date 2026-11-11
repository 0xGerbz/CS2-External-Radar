#include "game.h"
#include <iostream>


void Game::getEntityList() {
	entityList = memory::memRead<uintptr_t>(memory::moduleBase + client_dll::dwEntityList);
}
void Game::getMap() {
	//Based on the cs2 module dumper - we know the map name is found in matchmaking dll
	//We find this structure in the cs2 dumper
	uintptr_t mapPtr = memory::memRead<uintptr_t>(memory::matchmakingBase + matchmaking_dll::dwGameTypes + matchmaking_dll::dwGameTypes_mapName);
	//Read our map directly in to our structure
	memory::memRead(mapPtr + 0x4, &map, sizeof(map));
}

uintptr_t Game::getEntity(int index) {
	index &= 0x7FFF;
	uintptr_t chunk = memory::memRead<uintptr_t>(entityList + (0x8 * (index >> 9) + 0x10));
	if (!chunk) return 0;
	uintptr_t identity = chunk + 0x70 * (index & 0x1FF);
	return memory::memRead<uintptr_t>(identity);
}

uintptr_t Game::getEntityFromHandle(uint32_t handle) {
	uint32_t index = handle & 0x7FFF;
	uintptr_t chunk = memory::memRead<uintptr_t>(entityList + (0x8 * (index >> 9) + 0x10));
	if (!chunk) return 0;
	uintptr_t identity = chunk + 0x70 * (index & 0x1FF);
	return memory::memRead<uintptr_t>(identity);
}

void Game::getPlayers() {
	for (int i = 1; i <= 64; i++) {
		//Start out by getting each players controller and pawn
		Player p;
		p.controller = getEntity(i);
		if (!p.controller) continue;
		uint32_t pawnHandle = memory::memRead<uint32_t>(p.controller + client_dll::m_hPlayerPawn);
		p.pawn = getEntityFromHandle(pawnHandle);
		

		//Get players name
		uintptr_t nameAddress = memory::memRead<uintptr_t>(p.controller + client_dll::m_sSanitizedPlayerName);
		memory::memRead(nameAddress, &p.name, sizeof(p.name));

		//Get players team
		p.team = memory::memRead<uint8_t>(p.controller + client_dll::m_iTeamNum);

		//Get players health
		p.health = memory::memRead<int32_t>(p.pawn + client_dll::m_iHealth);

		//Player position
		memory::memRead(p.pawn + client_dll::m_vOldOrigin, &p.position, sizeof(vector3));


		players.push_back(p);
	}
}

void Game::update() {
	getEntityList();
	getMap();
	getPlayers();
}