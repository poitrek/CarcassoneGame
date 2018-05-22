#include "stdafx.h"
#include "MouseHandler.hpp"

bool MouseHandler::pressed = false;
int MouseHandler::state = 1;
sf::Vector2i MouseHandler::curPos, MouseHandler::prevPos, MouseHandler::dPos;
bool KeyHandler::pressed = false;