// Carcassonne.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "vld.h"
#include <iostream>
#include <fstream>
#include <vector>
//#include <queue>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameObject.hpp"
#include "EmptyField.hpp"
#include "Tile.hpp"

#include "Creator.hpp"
#include "Engine.hpp"
#include "MouseHandler.hpp"
//#include "Game.hpp"
//#include "SideMenu.hpp"

using namespace std;

const float WINDOW_WIDTH = 1060.f;
const float WINDOW_HEIGHT = 740.f;
const float BOARD_WIDTH = 800.f;
const float BOARD_HEIGHT = 620.f;


///sf::Vector2f CoordOrigin{ 0.f, 0.f }; Niepotrzebne
// Początek układu współrzędnych - już nieaktualny
/*
template<class T>
void show_shp(std::shared_ptr<T> &shp)
{
	std::cout << "Shared pointer: ";
	std::cout << shp.get() << std::endl;
	std::cout << "Use count: " << shp.use_count() << std::endl;
}
*/

// Nowa wersja (nieużyta)
std::vector<Tile> loadTileDatav2()
{
	std::ifstream file;
	file.open(Creator::data_filename);
	if (!file.is_open())
	{
		std::cout << "Error: nie udalo sie otworzyc pliku z danymi" << std::endl;
		std::cin.get();
		exit(1);
	}

	std::vector<Tile> _tileVector;

	for (int _i = 0; _i < 10; _i++)
	{
		// Wersja z wyrażeniem regularnym
		string s = "x(\\d{1,2})\\s*(\\w+.(png|jpg|bmp))\\s*\\n";
		s += "(\\d\\s)+\\n";
		s += "(\\d\\s){4}\\n";
		s += "(\\d\\s){8}\\n";
		regex r(s);
		smatch sm;

		string _string;
			
		if (regex_match(_string, sm, r))
		{

		}

		//----------


		int num;
		file >> num;		
		
		std::string t_filename;
		file >> t_filename;
				
		std::vector<int> indx_vector;
		string buffer = "";
		while (buffer != "#")
		{
			file >> buffer;
			indx_vector.push_back(stoi(buffer));
		}
		std::vector<int> othr_vector;
		for (int i = 0; i < 12; i++)
		{
			int x; file >> x;
			othr_vector.push_back(x);
		}

		Tile tile(t_filename);
		Creator::SetTileAttributes(tile, indx_vector, othr_vector);

		for (int i = 0; i < num; i++)
			_tileVector.push_back(tile);	


	}

	return _tileVector;
}


int main2()
{
	std::cout << "--------------------------------" << std::endl;
	std::cout << "Hello world!" << std::endl;
	

	std::vector<Tile> tileVector = Creator::LoadTileData();



	tileVector.clear();
	std::cin.get();
	return 0;
}



int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Carcassonne prototype", sf::Style::Default);
    sf::Event evnt;

	sf::RenderTexture board;
	board.create(BOARD_WIDTH, BOARD_HEIGHT);
	ViewEngine::resizeView(1, board);
	ViewEngine::resizeView(1, board);
	

	//EmptyField sqfTab[Board::EF_X][Board::EF_Y];
	EmptyField::LoadTexture();
	EmptyField **EFTab = Creator::SetEmptyFields();


	std::vector<Tile> tileVector = Creator::LoadTileData();	
	int tileIterator = 0;

	// Stawiamy początkową płytkę
	int mid_val = Board::EF_X / 2;
	tileVector[0].Place(&EFTab[mid_val][mid_val]);
	Engine::AddAvailableEFs(EFTab[mid_val][mid_val]);
	tileIterator++;

	//sf::View view;
	//view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//ViewEngine::pView = &view;
	//ViewEngine::resizeView((int)1, window);

	
	// Testowa miniaturka płytki
	GameObject tileThumbnail;
	tileThumbnail.setPosition(910.f, 270.f);
	tileThumbnail.setTexture(tileVector[tileIterator].getTexture());
	tileThumbnail.setSize(sf::Vector2f(140.f, 140.f));
	setOriginToCenter(tileThumbnail);

	// Cień płytki którą chcemy postawić
	GameObject tileShadow;
	tileShadow.setTexture(tileVector[tileIterator].getTexture());
	tileShadow.setSize(Board::Square);
	setOriginToCenter(tileShadow);
	sf::Sprite *tileShadow_s = tileShadow.getSprite();
	tileShadow_s->setColor(sf::Color(255, 255, 255, 120));

	// Wstępne działania
	Engine::SetUp();


	// Sprite przechowujący planszę
	sf::Sprite board_s;
	board_s.setPosition(Board::boardOffset);

    while (window.isOpen())
    {
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonReleased:
				MouseHandler::Release();
				break;
			case sf::Event::KeyReleased:
				KeyHandler::Release();
				break;
			case sf::Event::MouseWheelScrolled:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
				{
					int delta = evnt.mouseWheelScroll.delta;
					ViewEngine::resizeView(delta, board);
				}
				else
				{
					// Przed położeniem płytki (test)
					if (tileIterator < tileVector.size())
					{
						int delta = -evnt.mouseWheelScroll.delta;
						tileVector[tileIterator].Rotate(delta);
						Rotate(tileThumbnail, delta);
						Rotate(tileShadow, delta);
					}
				}
				break;

			}
		}

		board.clear(sf::Color(170, 170, 120));

		EmptyField::DrawAll(board);
		Tile::DrawAll(board);
		
		
		switch (Engine::GameState)
		{
		case Engine::placingTile:
			
			//for each (EmptyField *p_sqf in Engine::AvailableEFs)
			// - nie działa, gdyż w czasie wykonywania pętli
			// modyfikujemy wektor AvailableEFs!
		
			for each (EmptyField *EF in EmptyField::EF_vector)
				if (EF->clicked(board, window))
				{
					// Obsługa kliknięcia w dowolne pole
					if (EF->_plus != nullptr)

						if (tileIterator < tileVector.size())
						{
							if (Engine::BorderMatch(EF, tileVector[tileIterator]))
							{
								Engine::PlaceTheTile(EF, tileVector, tileIterator,
									tileThumbnail, tileShadow);
							}
							else
							{
								std::cout << "The tile cannot be placed." << std::endl;
							}
						}


					//-------------------------
				}
		
			// Rysujemy "plusy" na dostępnych polach
			Engine::DrawAvailableEFs(board);

			for each (EmptyField *EF in EmptyField::EF_vector)
				if (EF->mouseEntered(board, window))
				{
					// Obsługa najechania na pole myszą

					if (EF->_plus != nullptr)
					{
						tileShadow.setPosition(EF->getPosition());
						tileShadow.draw(board);
					}
				}


			break;
		case Engine::DrawingPawnSpots:

			Engine::DrawPawnSpots(board, window);

			break;
		case Engine::placingPawn:

			Engine::PlaceThePawn3(board, window);
			break;
		default:
			break;
		}
		



		board.display();

        window.clear(sf::Color(140, 70, 20));
		
		board_s.setTexture(board.getTexture());

		window.draw(board_s);

		tileThumbnail.draw(window); // Miniaturka płytki


		if (MouseHandler::buttonPressed(sf::Mouse::Left))
		{
			sf::Vector2f coord = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			std::cout << "(" << coord.x << ", " << coord.y << ")" << std::endl;
		}
		
		// Wypisz pozycje pól planszy
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			/*
			for (int i = 0; i < Board::EF_X - 5; i++)
			{
				for (int j = 0; j < Board::EF_Y - 3; j++)
				{
					sf::Vector2f pos = sqfTab[i][j].getPosition();
					std::cout << "(" << pos.x << ", " << pos.y << ") ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl << std::endl;
			*/
			for (int i = 0; i < Board::EF_X; i++)
			{
				for (int j = 0; j < Board::EF_Y; j++)
				{
					if (EFTab[i][j].tile != nullptr)
						cout << " t";
					else
						cout << "  ";
				}
				cout << endl;
			}
		}
		
		ViewEngine::moveView(board, window);
        window.display();
    }

	Creator::ClearEmptyFields(EFTab);
    return 0;
}

