#include "stdafx.h"
#include "Board.hpp"


const sf::Vector2f Board::boardOffset{ 20.f, 100.f };

const int Board::EF_X{ 16 }, Board::EF_Y{ 16 };

float Board::squareScale{ 0.5f };
float Board::squareSize{ 60.f };
sf::Vector2f Board::Square{ 60.f, 60.f };
Board::Board()
{
}


Board::~Board()
{
}
