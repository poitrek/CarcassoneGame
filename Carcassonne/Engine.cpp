#include "stdafx.h"
#include "Engine.hpp"

std::vector<EmptyField*> Engine::AvailableEFs;
Engine::_GameState Engine::GameState;
Tile * Engine::CurrentTile;
GameObject Engine::Circle[12];
shared_ptr<sf::Texture> Engine::spot_texture;

void Engine::AddAvailableEFs(EmptyField &EF)
{
	// Wektor z sπsiadami tego pola
	std::vector<EmptyField*> tempVector = { EF.Up, EF.Down, EF.Left, EF.Right };
	
	for (EmptyField *tempEF : tempVector)
	{
		// ZwrÛci prawdÍ, jeúli znaleziono
		bool found = (find(AvailableEFs.begin(), AvailableEFs.end(), tempEF) != AvailableEFs.end());
		// Jeúli pole jest puste i nie ma go jeszcze na liúcie
		if (tempEF->tile == nullptr && !found)
		{
			AvailableEFs.push_back(tempEF);
			tempEF->_plus = unique_ptr<GameObject>(new GameObject("../Carcassonne/images/plus.png"));
			tempEF->_plus->setPosition(tempEF->getPosition());
			setOriginToCenter(*(tempEF->_plus));
			tempEF->_plus->setSize(sf::Vector2f(Board::squareSize, Board::squareSize));
		}
	}
	// Usuwamy z listy to pole (tEF)
	EF._plus.reset();
	AvailableEFs.erase(std::remove(AvailableEFs.begin(), AvailableEFs.end(), &EF), AvailableEFs.end());
}

void Engine::DrawAvailableEFs(sf::RenderTarget &target)
{
	for (EmptyField *EF : AvailableEFs)
	{
		EF->_plus->draw(target);
	}
}

bool Engine::BorderMatch(EmptyField * EF, Tile & tile)
{
	EmptyField *neighboor;
	neighboor = EF->Up;
	bool match_u;
	if (neighboor->tile == nullptr)
	{
		match_u = true;
	}
	else
	{
		match_u = (neighboor->tile->D.type == tile.U.type);
	}

	neighboor = EF->Right;
	bool match_r;
	if (neighboor->tile == nullptr)
	{
		match_r = true;
	}
	else
	{
		match_r = (neighboor->tile->L.type == tile.R.type);
	}

	neighboor = EF->Down;
	bool match_d;
	if (neighboor->tile == nullptr)
	{
		match_d = true;
	}
	else
	{
		match_d = (neighboor->tile->U.type == tile.D.type);
	}

	neighboor = EF->Left;
	bool match_l;
	if (neighboor->tile == nullptr)
	{
		match_l = true;
	}
	else
	{
		match_l = (neighboor->tile->R.type == tile.L.type);
	}

	return (match_d && match_l && match_r && match_u);

}

void Engine::PlaceTheTile(EmptyField * EF, vector<Tile> &tileVector, int &tileIterator,
	GameObject &tileThumbnail, GameObject &tileShadow)
{

	std::cout << "Tile placing available." << std::endl;
	tileVector[tileIterator].Place(EF);

	Engine::CurrentTile = &tileVector[tileIterator];

	Engine::AddAvailableEFs(*(tileVector[tileIterator].EF));
	tileIterator++;

	if (tileIterator < tileVector.size())
	{
		// Modyfikujemy miniaturkÍ
		sf::Sprite *tileThumbnail_s = tileThumbnail.getSprite();
		tileThumbnail.setTexture(tileVector[tileIterator].getTexture(), 1);
		tileThumbnail_s->setRotation(0);
		// Modyfikujemy cieÒ
		sf::Sprite *tileShadow_s = tileShadow.getSprite();
		tileShadow.setTexture(tileVector[tileIterator].getTexture(), 1);
		tileShadow_s->setRotation(0);
	}

	// Zmieniamy stan gry na stawianie pionka
	Engine::GameState = DrawingPawnSpots;

}


void Engine::UpdateTileProps(Tile & tile)
{
	//Tile* tile_Down = tile.EF->Down->tile;
	//èle, bo moøe nie byÊ tej p≥ytki




}


/* Stara wersja funkcji, nieaktualna - mia≥a dzia≥aÊ
poprzez klikniÍcie w dowolne miejsce p≥ytki i pobranie
z danego piksela koloru oraz okreúlenie, w ktÛry obiekt
na p≥ytce kliknÍliúmy. Rozwiπzanie okaza≥o siÍ zbyt 
skomplikowane */
void Engine::PlaceThePawn(Tile & tile, sf::RenderTarget &board, sf::RenderWindow &window)
{	
	// Konwertujemy teksturÍ na obraz
	sf::Image texImage = tile.getTexture().copyToImage();

	// Potrzebujemy pozycjÍ kursora wzglÍdem p≥ytki
	sf::Vector2i boardOffsetI{ v_2i(Board::boardOffset) }; // Przekonwertowany wektor
	sf::Vector2i mpos = sf::Mouse::getPosition(window) - boardOffsetI; // Pozycja kursora w oknie
	sf::Vector2f coord = board.mapPixelToCoords(mpos); // Pozycja kursora na mapie
	// Pozycja lewego gÛrnego rogu p≥ytki
	sf::Vector2f gpos = tile.getPosition() - 0.5f * tile.getOrigin();
	// Wektor pozycji kursora wewnπtrz p≥ytki
	sf::Vector2u intPos = v_conv<unsigned int, float>(coord - gpos);

	sf::Color col = texImage.getPixel(intPos.x, intPos.y);

	// PawnPlacingHandler::NearestBorder musi zwrÛciÊ informacjÍ
	// o tym, do ktÛrej krawÍdzi dany punkt (intPos) ma najbliøej.
	// NastÍpnie trzeba bÍdzie tÍ informacjÍ dalej przetworzyÊ z
	// uzyskanym kolorem piksela.


}



void Engine::DrawPawnSpots(sf::RenderTarget &, sf::RenderWindow &)
{
	
	// Rysujemy punkty, w ktÛrych bÍdzie moøna postawiÊ pionek
	
	// Pozycja lewego gÛrnego rogu p≥ytki
	sf::Vector2f gpos = CurrentTile->getPosition();// -0.5f * CurrentTile->getOrigin();
	float buffer{ Board::squareSize * 0.13f };
	

	// Ustawiamy pozycje punktÛw
	GameObject * c_temp = &Circle[0];
	{
		sf::Vector2f vec(0, -Board::squareSize*0.5f + buffer);
		c_temp->setPosition(gpos + vec);
	}
	c_temp = &Circle[1];
	{
		sf::Vector2f vec(Board::squareSize*0.5f - buffer, 0);
		c_temp->setPosition(gpos + vec);
	}
	c_temp = &Circle[2];
	{
		sf::Vector2f vec(0, Board::squareSize*0.5f - buffer);
		c_temp->setPosition(gpos + vec);
	}
	c_temp = &Circle[3];
	{
		sf::Vector2f vec(-Board::squareSize*0.5f + buffer, 0);
		c_temp->setPosition(gpos + vec);
	}

	cout << "---- Engine::DrawPawnSpots() ----" << endl;

	cout << "gpos: " << gpos << endl;
	cout << "CurrentTile.getPostition(): " << CurrentTile->getPosition() << endl;
	cout << "CurrentTile.getOrigin(): " << CurrentTile->getOrigin() << endl;
	cout << "buffer:" << buffer << endl;


	for (int i = 0; i < 4; i++)
	{
		cout << "Circle[" << i << "]:" << Circle[i].getPosition() << endl;
	}

	cout << "---------------------------------" << endl << endl;

	GameState = Engine::placingPawn;

}

void Engine::PlaceThePawn3(sf::RenderTarget &target, sf::RenderWindow &window)
{
	for (int i = 0; i < 4; i++)
		Circle[i].draw(target);
}

void Engine::SetUp()
{
	//sf::Texture circle_texture;
	spot_texture = shared_ptr<sf::Texture>(new sf::Texture);
	spot_texture->loadFromFile("images/circle.png");
	sf::Vector2f spot_size{ Board::Square * 0.1f };
	// Ustawiamy teksturÍ i rozmiar punktÛw
	for (int i = 0; i < 12; i++)
	{
		Circle[i].setTexture(*spot_texture);
		Circle[i].setSize(spot_size);
		setOriginToCenter(Circle[i]);
	}
}


/*
Tile::Border Engine::PawnPlacingHandler::NearestBorder(sf::Vector2u intPos)
{
	// Wymiary p≥ytki (konwertujemy wektor)
	sf::Vector2u TileSize = v_conv<unsigned int, float>(Board::Square);
	

}
*/

std::ostream& operator << (std::ostream& pStream, sf::Vector2f vec)
{
	pStream << "(" << vec.x << ", " << vec.y << ")";
	return pStream;
}

/// Funkcje s≥uøπce do konwersji wektorÛw
template <class T, class U>
sf::Vector2<T> v_conv(sf::Vector2<U> vec)
{
	T new_x = static_cast<T>(vec.x);
	T new_y = static_cast<T>(vec.y);
	return sf::Vector2<T>(new_x, new_y);
}

sf::Vector2f v_2f(sf::Vector2i vecI)
{
	float x = static_cast<float>(vecI.x);
	float y = static_cast<float>(vecI.y);
	return sf::Vector2f(x, y);
}
sf::Vector2f v_2f(sf::Vector2u vecU)
{
	float x = static_cast<float>(vecU.x);
	float y = static_cast<float>(vecU.y);
	return sf::Vector2f(x, y);
}
sf::Vector2i v_2i(sf::Vector2f vecF)
{
	int x = static_cast<int>(vecF.x);
	int y = static_cast<int>(vecF.y);
	return sf::Vector2i(x, y);
}

//sf::View* ViewEngine::pView = nullptr;
//sf::RenderWindow* ViewEngine::pWindow = nullptr;
sf::Vector2f ViewEngine::ViewSize{ BOARD_WIDTH, BOARD_HEIGHT };
sf::Vector2f ViewEngine::prevPos;
sf::Vector2f ViewEngine::MOUSEPOS;
//float ViewEngine::viewRatio = WINDOW_WIDTH / WINDOW_HEIGHT;

