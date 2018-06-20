#pragma once
#include <SFML\Graphics.hpp>
class Board
{
public:
	static const sf::Vector2f boardOffset;

	static const int EF_X, EF_Y;

	static float squareScale;
	static float squareSize;
	static sf::Vector2f Square;
	Board();
	~Board();
};

