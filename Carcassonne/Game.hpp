#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameObject.hpp"
#include "EmptyField.hpp"

class Game
{
	const int windowWidth = 900;
	const int windowHeight = 600;

public:

	enum State{ ShowingMenu, Playing, Exiting };
	static State _state;

	void GameLoop();

	sf::RenderWindow *mainWindow;

	~Game();
};

