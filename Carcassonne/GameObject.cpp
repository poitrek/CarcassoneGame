#include "stdafx.h"
#include "GameObject.hpp"


extern sf::Vector2f v_2f(sf::Vector2i vecI);
extern sf::Vector2f v_2f(sf::Vector2u vecU);

extern const sf::Vector2f boardOffset;
extern sf::Vector2i v_2i(sf::Vector2f vecF);

const sf::Vector2i boardOffsetI{ v_2i(boardOffset) };

//static const sf::Vector2i boardOffestI{ v_conv<int, float>(boardOffset) };
/*
void GameObject::setTexture(sf::Texture & texture)
{
	sprite.setTexture(texture);
}

sf::Sprite & GameObject::GetSprite()
{
	return sprite;
}

GameObject::GameObject()
{
}*/

void setOriginToCenter(GameObject &obj)
{
	obj.setOrigin(v_2f(obj.texture->getSize()) * 0.5f);
}

void Rotate(GameObject &obj, int n)
{
	obj.sprite.rotate(90.f * n);
}
void setRotation(GameObject &obj, int n)
{
	obj.sprite.setRotation(90.f * n);
}


GameObject::GameObject(float posX, float posY)
{
	this->setPosition(posX, posY);
}

GameObject::GameObject(std::string _filename, float posX, float posY)
	: filename(_filename)
{
	this->LoadTexture();
	this->setPosition(posX, posY);
}

GameObject::~GameObject()
{
	//delete texture;
	///std::cout << "~GameObject()" << std::endl;
}

sf::Sprite * GameObject::getSprite()
{
	return &(this->sprite);
}

void GameObject::setPosition(sf::Vector2f vec)
{	
	sprite.setPosition(vec);
}

void GameObject::setPosition(float x, float y)
{
	sprite.setPosition(x, y);
}

sf::Vector2f GameObject::getPosition()
{
	return sprite.getPosition();
}

void GameObject::setSize(sf::Vector2f vec)
{
	float scaleX = vec.x / sprite.getTexture()->getSize().x * 1.0f;
	float scaleY = vec.y / sprite.getTexture()->getSize().y * 1.0f;
	sprite.setScale(scaleX, scaleY);
}

void GameObject::setOrigin(sf::Vector2f vec)
{
	this->sprite.setOrigin(vec);
	this->origin = vec;
}

sf::Vector2f GameObject::getSize()
{
	return { sprite.getScale().x * sprite.getTexture()->getSize().x, sprite.getScale().y * sprite.getTexture()->getSize().y };
}

void GameObject::setTexture(sf::Texture t, bool holdSize)
{
	// Zmienna pomocnicza
	sf::Vector2f prevSize{ 0.f, 0.f };
	if (texture != nullptr)
		prevSize = this->getSize();

	//this->texture = std::shared_ptr<sf::Texture>(new sf::Texture(t));
	this->texture.reset(new sf::Texture(t));
	this->sprite.setTexture(*texture);

	if (holdSize == true)
	{
		setSize(prevSize);
	}
}

sf::Texture GameObject::getTexture()
{
	return *texture;
}

void GameObject::LoadTexture()
{
	//texture = new sf::Texture;
	texture = std::shared_ptr<sf::Texture>(new sf::Texture);
	if (!texture->loadFromFile(filename))
	{
		std::cout << "<GameObject::LoadTexture()>" << std::endl << "Error: cannot open file '" << filename << "'" << std::endl;
	}
	else
	{
		texture->setSmooth(true); // NOWA ZMIANA
		sprite.setTexture(*texture);
	}
}

void GameObject::LoadTexture(std::string _filename)
{
	filename = _filename;
	//texture = new sf::Texture; // v.1
	texture = std::shared_ptr<sf::Texture>(new sf::Texture); // v.2

	if (!texture->loadFromFile(filename))
	{
		std::cout << "<GameObject::LoadTexture()>" << std::endl << "Error: cannot open file '" << filename << "'" << std::endl;
	}
	else
	{
		sprite.setTexture(*texture);
	}
}

bool GameObject::mouseEntered(sf::RenderTarget& target, sf::RenderWindow& window)
{
	sf::Vector2i mpos = sf::Mouse::getPosition(window) - boardOffsetI; // Pozycja kursora w oknie
	sf::Vector2f coord = target.mapPixelToCoords(mpos); // Pozycja kursora na mapie
	
	// Pozycja lewego górnego rogu obiektu (uwzglêdniamy origin)
	sf::Vector2f gpos = this->getPosition() - 0.5f * this->origin;
	// Sprawdzamy, czy pozycja kursora na mapie zawiera siê w granicach obiektu
	return (coord.x > gpos.x && coord.x < gpos.x + getSize().x && coord.y > gpos.y && coord.y < gpos.y + getSize().y);
}

bool GameObject::clicked(sf::RenderTarget& target, sf::RenderWindow& window)
{
	// Zwraca, czy obiekt zosta³ nakierowany mysz¹ i czy nast¹pi³o
	// pojedyncze klikniêcie (bez poruszania)
	return (this->mouseEntered(target, window) && MouseHandler::singleClick());

}

void GameObject::draw(sf::RenderTarget &target)
{
	target.draw(sprite);
}
