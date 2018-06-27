#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameObject.hpp"
#include "EmptyField.hpp"
#include "Tile.hpp"


extern const float BOARD_WIDTH;
extern const float BOARD_HEIGHT;


/// Dodatkowe - operator z wektorem2d
std::ostream& operator << (std::ostream& pStream, sf::Vector2f vec);
/// Funkcje s³u¿¹ce do konwersji wektorów
template <class T, class U>
sf::Vector2<T> v_conv(sf::Vector2<U> vec);
sf::Vector2f v_2f(sf::Vector2i vecI);
sf::Vector2f v_2f(sf::Vector2u vecU);
sf::Vector2i v_2i(sf::Vector2f vecF);

class Engine
{
public:
	enum _GameState {
		placingTile,
		placingPawn,
		DrawingPawnSpots
	};
	static _GameState GameState;

	static std::vector<EmptyField*> AvailableEFs;
	static void AddAvailableEFs(EmptyField &EF);
	static void DrawAvailableEFs(sf::RenderTarget&);
	static bool BorderMatch(EmptyField *EF, Tile & tile);
	static void PlaceTheTile(EmptyField *EF, vector<Tile>&, int &tileIterator, GameObject&, GameObject&);
	// Funkcja stawiaj¹ca pionek na podanej p³ytce
	static void UpdateTileProps(Tile &tile);

	static void PlaceThePawn(Tile &tile, sf::RenderTarget&, sf::RenderWindow&);


	static void DrawPawnSpots(sf::RenderTarget&, sf::RenderWindow&);
	static void PlaceThePawn3(sf::RenderTarget&, sf::RenderWindow&);

	static void SetUp();

private:
	static Tile * CurrentTile;
	static GameObject Circle[12];
	static shared_ptr<sf::Texture> spot_texture;

	struct PawnPlacingHandler
	{
		//static Tile::Border NearestBorder(sf::Vector2u intPos);
	};

private:

};



/// Klasa s³u¿¹ca do obs³ugi perspektywy
class ViewEngine
{
public:
	//static sf::View *pView;
	//static sf::RenderWindow *pWindow;
	static sf::Vector2f ViewSize;

	/// Przesuwanie perspektywy
	static void moveView(sf::RenderTarget& target, sf::RenderWindow& window)
	{

		MOUSEPOS = v_2f(sf::Mouse::getPosition(window));

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::View view = target.getView();
			sf::Vector2f dV = prevPos - MOUSEPOS; // Przyrost pozycji myszy
			float ratio = ViewSize.x / BOARD_WIDTH; // Odpowiedni iloraz
			
			// Wypisywanie informacji (profilaktycznie)
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				std::cout << view.getCenter() - view.getSize() / 2.f << std::endl;
			// Dodatkowe zmienne
			sf::Vector2f vLU = view.getCenter() - view.getSize() * 0.5f;
			float offset = 5.f;
			float sizeX = Board::EF_X * EmptyField::getSize().x;
			float sizeY = Board::EF_Y * EmptyField::getSize().y;

			if (vLU.x < -offset && dV.x < 0)
				dV.x = 0.f;
			if (vLU.y < -offset && dV.y < 0)
				dV.y = 0.f;
			vLU += view.getSize();
			if (vLU.x > sizeX + offset && dV.x > 0)
				dV.x = 0.f;
			if (vLU.y > sizeY + offset && dV.y > 0)
				dV.y = 0.f;

			view.move(dV * ratio); // Przesuwamy perspektywê
			
			target.setView(view);
		}
		prevPos = MOUSEPOS;
	}

	/// Przybli¿anie/oddalanie perspektywy
	static void resizeView(int scrollRate, sf::RenderTarget& target)
	{
		sf::View view = target.getView();
		if (scrollRate == 1) ViewSize *= 0.9f;
		if (scrollRate == -1) ViewSize /= 0.9f;
		view.setSize(ViewSize);
		target.setView(view);
			
		/*
		if (scrollRate == -1) pView->zoom(0.9f);
		if (scrollRate == 1) pView->zoom(1 / 0.9f);
		*/

	}

private:
	//static float viewRatio;
	static sf::Vector2f prevPos;
	static sf::Vector2f MOUSEPOS;
};

