#pragma once
#include "memory.h"
#include "offsets.h"

//This class will hold everything we need to read in per player
class Player {
	uintptr_t controller;
	uint32_t entityHandle;
};

//The game class holds things that belong to the game in general, not specific players
class Game {
	public:
	Player localPlayer;
	uintptr_t entityList;
	Player players[64];
	char map[36];

	void getMap();

};