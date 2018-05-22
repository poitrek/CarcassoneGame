#include "stdafx.h"
#include "SideMenu.hpp"

SideMenu::SideMenu()
{
	shape.setOutlineThickness(4.f);
	shape.setFillColor(sf::Color(225, 173, 103));
	shape.setOutlineColor(sf::Color(205, 133, 63));
}

void SideMenu::setSize(sf::Vector2f vec)
{
	this->shape.setSize(vec);
}

void SideMenu::setPosition(sf::Vector2f vec)
{
	this->shape.setPosition(vec);
}

void SideMenu::move(sf::Vector2f vec)
{
	this->shape.move(vec);
}

void SideMenu::draw(sf::RenderWindow &window)
{
	window.draw(this->shape);
}
