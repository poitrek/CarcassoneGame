#include "stdafx.h"
#include "Engine.hpp"

std::vector<EmptyField*> Engine::AvailableEFs;
Engine::_GameState Engine::GameState;

void Engine::AddAvailableEFs(EmptyField &EF)
{
	// Wektor z s¹siadami tego pola
	std::vector<EmptyField*> tempVector = { EF.Up, EF.Down, EF.Left, EF.Right };
	
	for (EmptyField *tempEF : tempVector)
	{
		// Zwróci prawdê, jeœli znaleziono
		bool found = (find(AvailableEFs.begin(), AvailableEFs.end(), tempEF) != AvailableEFs.end());
		// Jeœli pole jest puste i nie ma go jeszcze na liœcie
		if (tempEF->tile == nullptr && !found)
		{
			AvailableEFs.push_back(tempEF);
			tempEF->_plus = unique_ptr<GameObject>(new GameObject("../Carcassonne/images/plus.png"));
			tempEF->_plus->setPosition(tempEF->getPosition());
			setOriginToCenter(*(tempEF->_plus));
			tempEF->_plus->setSize(sf::Vector2f(Board::squareSize, Board::squareSize));
		}
	}
	// Usuwamy z listy to pole (tEF)
	EF._plus.reset();
	AvailableEFs.erase(std::remove(AvailableEFs.begin(), AvailableEFs.end(), &EF), AvailableEFs.end());
}

void Engine::DrawAvailableEFs(sf::RenderTarget &target)
{
	for (EmptyField *EF : AvailableEFs)
	{
		EF->_plus->draw(target);
	}
}

bool Engine::BorderMatch(EmptyField * EF, Tile & tile)
{
	EmptyField *neighboor;
	neighboor = EF->Up;
	bool match_u;
	if (neighboor->tile == nullptr)
	{
		match_u = true;
	}
	else
	{
		match_u = (neighboor->tile->D.type == tile.U.type);
	}

	neighboor = EF->Right;
	bool match_r;
	if (neighboor->tile == nullptr)
	{
		match_r = true;
	}
	else
	{
		match_r = (neighboor->tile->L.type == tile.R.type);
	}

	neighboor = EF->Down;
	bool match_d;
	if (neighboor->tile == nullptr)
	{
		match_d = true;
	}
	else
	{
		match_d = (neighboor->tile->U.type == tile.D.type);
	}

	neighboor = EF->Left;
	bool match_l;
	if (neighboor->tile == nullptr)
	{
		match_l = true;
	}
	else
	{
		match_l = (neighboor->tile->R.type == tile.L.type);
	}

	return (match_d && match_l && match_r && match_u);

}



std::ostream& operator << (std::ostream& pStream, sf::Vector2f vec)
{
	pStream << "(" << vec.x << ", " << vec.y << ")";
	return pStream;
}

/// Funkcje s³u¿¹ce do konwersji wektorów
template <class T, class U>
sf::Vector2<T> v_conv(sf::Vector2<U> vec)
{
	T new_x = static_cast<T>(vec.x);
	T new_y = static_cast<T>(vec.y);
	return sf::Vector2<T>(new_x, new_y);
}

sf::Vector2f v_2f(sf::Vector2i vecI)
{
	float x = static_cast<float>(vecI.x);
	float y = static_cast<float>(vecI.y);
	return sf::Vector2f(x, y);
}
sf::Vector2f v_2f(sf::Vector2u vecU)
{
	float x = static_cast<float>(vecU.x);
	float y = static_cast<float>(vecU.y);
	return sf::Vector2f(x, y);
}
sf::Vector2i v_2i(sf::Vector2f vecF)
{
	int x = static_cast<int>(vecF.x);
	int y = static_cast<int>(vecF.y);
	return sf::Vector2i(x, y);
}

//sf::View* ViewEngine::pView = nullptr;
//sf::RenderWindow* ViewEngine::pWindow = nullptr;
sf::Vector2f ViewEngine::ViewSize{ BOARD_WIDTH, BOARD_HEIGHT };
sf::Vector2f ViewEngine::prevPos;
sf::Vector2f ViewEngine::MOUSEPOS;
//float ViewEngine::viewRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
