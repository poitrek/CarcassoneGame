#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "GameObject.hpp"

class SideMenu : public GameObject
{
public:
	SideMenu();
	void setSize(sf::Vector2f);
	void setPosition(sf::Vector2f);
	sf::Vector2f getSize() { return this->shape.getSize(); }
	sf::Vector2f getPosition() { return this->shape.getPosition(); }
	void move(sf::Vector2f);
	void draw(sf::RenderWindow&);

private:
	sf::RectangleShape shape;
};

