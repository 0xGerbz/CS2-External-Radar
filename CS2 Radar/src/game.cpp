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

//This returns the pointer to an entity on the entityList. We use this when we know the index of the entity we want, such as players (1-64)
uintptr_t Game::getEntity(int index) {
	//Entity list is made of an array of "chunks" which start at entityList + 0x10.
	//Each chunk hold 512 entities, so we use a bit shift to find our chunk. This is the same as "index / 512"
	//We multiply our chunk number by the size of a chunk pointer (8 bytes) to get our offset in the chunk array
	uintptr_t chunk = memory::memRead<uintptr_t>(entityList + (0x8 * (index >> 9) + 0x10));
	if (!chunk) return 0;
	//The identity is the slot inside the chunk we are looking at. It holds a pointer to the object on our entity list
	uintptr_t identity = chunk + 0x70 * (index & 0x1FF);
	//Read the pointer to our entity object
	return memory::memRead<uintptr_t>(identity);
}
//Similar to before, this returns a pointer to an object on our entityList, but we are starting from a handle to an entity
uintptr_t Game::getEntityFromHandle(uint32_t handle) {
	//make sure we have a valid handle
	if (handle == 0xFFFFFFFF) return 0;
	//The handle is broken in to different sections. The lowest 15 bits are the index of the given entity
	uint32_t index = handle & 0x7FFF;
	//Same logic as before
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