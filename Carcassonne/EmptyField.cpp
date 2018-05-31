#include "stdafx.h"
#include "EmptyField.hpp"

extern sf::Vector2f v_2f(sf::Vector2u vecU);

const std::string EmptyField::filename = "../Carcassonne/images/square_field.png";
sf::Texture* EmptyField::texture = nullptr;
int EmptyField::t_count = 0;
//std::shared_ptr<sf::Texture> EmptyField::texture(nullptr);// (new sf::Texture);

float EmptyField::scale = 0.5f;
std::vector <EmptyField*> EmptyField::EF_vector;

void EmptyField::setText()
{
	std::string sx = std::to_string(indX);
	std::string sy = std::to_string(indY);
	std::string s = sx + "  " + sy;
	text.setString(s);

	font.loadFromFile("CODE bold.otf");
	text.setFont(font);
	text.setCharacterSize(10);
	text.setPosition(this->getPosition());
}

void EmptyField::LoadTexture()
{
	//texture = std::shared_ptr<sf::Texture>(new sf::Texture);
	texture = new sf::Texture;
	texture->loadFromFile(filename);
}

sf::Vector2f EmptyField::getSize()
{
	float x = static_cast<float> (texture->getSize().x * scale);
	float y = static_cast<float> (texture->getSize().y * scale);
	return sf::Vector2f(x, y);
}


void EmptyField::DrawAll(sf::RenderTarget& target)
{
	for (auto it = EF_vector.begin(); it != EF_vector.end(); it++)
	{
		(*it)->draw(target);
	}
}

EmptyField::EmptyField()
{
	this->setOrigin(v_2f(texture->getSize()) * 0.5f); // Ustawiamy œrodek za pomoc¹ (jeszcze nie za³adowanej) tekstury
	sprite.setTexture(*EmptyField::texture); // Ustawiamy teksturê
	sprite.setScale(Board::squareScale, Board::squareScale); // Ustawiamy skalê
	EF_vector.push_back(this); // Dodajemy ten element do wspólnego kontenera
	t_count++;
}


EmptyField::~EmptyField()
{
	t_count--;
	if (t_count == 0)
	{
		delete texture;
	}
}
