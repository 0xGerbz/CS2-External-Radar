#pragma once
#include "memory.h"
#include "offsets.h"
#include <vector>

struct vector3 {
	float x, y, z;

	vector3() : x(0.0f), y(0.0f), z(0.0f) {}
};
//This class will hold everything we need to read in per player
struct Player {
	//The controller holds the address of the players CCSPlayerController object
	uintptr_t controller;
	//The pawn is the address of the players C_CSPlayerPawn object
	uintptr_t pawn;

	//Player values - we inherit the types of each variable from the client.dll dump
	char name[36];
	uint8_t team;
	int32_t health;
	vector3 position;

};

//The game class holds things that belong to the game in general, not specific players
struct Game {
	public:
	Player localPlayer;
	uintptr_t entityList;
	char map[36];
	std::vector<Player> players;

	void update();

	void getMap();
	void getEntityList();
	uintptr_t getEntity(int index);
	uintptr_t getEntityFromHandle(uint32_t handle);
	void getPlayers();

};

