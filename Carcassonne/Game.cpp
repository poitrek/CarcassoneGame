#include "stdafx.h"
#include "Game.hpp"

Game::State Game::_state = Game::ShowingMenu;

void Game::GameLoop()
{
	switch (_state)
	{
	case Game::ShowingMenu:
		break;
	case Game::Playing:
		break;
	case Game::Exiting:
		break;
	default:
		break;
	}
}

Game::~Game()
{
}
