#pragma once
#include "Pawn.hpp"

class Player
{
public:
	static int numberOfPlayers;
	Pawn pawn[7];

	Player();
	~Player();
};

