#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <iostream>

// Funkcja zwracaj¹ca "d³ugoœæ" wektora
template <class T>
T len(sf::Vector2<T> vec)
{
	T x = vec.x, y = vec.y;
	if (x < 0) x = -x;
	if (y < 0) y = -y;
	return x + y;
}

// Struktura s³u¿¹ca do obs³ugi myszki
struct MouseHandler
{
	static void HandleClick(sf::RenderWindow& window)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
		{
			sf::Vector2f coord = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			std::cout << "(" << coord.x << ", " << coord.y << ")" << std::endl;
			pressed = true;
		}
	}
	static bool buttonPressed(sf::Mouse::Button btn)
	{
		if (sf::Mouse::isButtonPressed(btn) && !pressed)
		{
			pressed = true;
			return true;
		}
		else
			return false;
	}

	static bool singleClick() // click + release with no moving
	{
		bool ret;
		curPos = sf::Mouse::getPosition();
		dPos = curPos - prevPos;
		

		switch (state)
		{
		case 1:
			if (pressed)
				state = 2;
			ret = 0;
			break;
		case 2:
			if (pressed)
			{
				if (len<int>(dPos) > 2)
				{
					state = 3;
					std::cout << "[MouseHanlder] Mouse has been moved" << std::endl;
				}
				ret = 0;
			}
			else
			{
				state = 1;				
				ret = 1;
			}
			break;
		case 3:
			if (!pressed)
				state = 1;
			ret = 0;
		}

		prevPos = curPos; // Pozycja aktualna staje siê poprz.
		return ret;
	}
private:
	static sf::Vector2i curPos, prevPos, dPos;
	static int state;	

public:	
	static void Release() { pressed = false; }
private:
	static bool pressed;

};

// Struktura do obs³ugi klawiatury
struct KeyHandler
{
	static bool keyPressed(sf::Keyboard::Key key)
	{
		if (!pressed)
		{
			if (sf::Keyboard::isKeyPressed(key))
			{
				pressed = true;
				return true;
			}
			else return false;
		}
		else
		{
			return false;
		}
	}
	static void Release() { pressed = false; }
private:
	static bool pressed;
};
