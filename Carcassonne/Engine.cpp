#include "stdafx.h"
#include "Engine.hpp"

std::vector<EmptyField*> Engine::AvailableEFs;
Engine::_GameState Engine::GameState;
Tile * Engine::CurrentTile;
GameObject Engine::Circle[12];

void Engine::AddAvailableEFs(EmptyField &EF)
{
	// Wektor z s¹siadami tego pola
	std::vector<EmptyField*> tempVector = { EF.Up, EF.Down, EF.Left, EF.Right };
	
	for (EmptyField *tempEF : tempVector)
	{
		// Zwróci prawdê, jeœli znaleziono
		bool found = (find(AvailableEFs.begin(), AvailableEFs.end(), tempEF) != AvailableEFs.end());
		// Jeœli pole jest puste i nie ma go jeszcze na liœcie
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

	CurrentTile = &tileVector[tileIterator];

	Engine::AddAvailableEFs(*(tileVector[tileIterator].EF));
	tileIterator++;

	if (tileIterator < tileVector.size())
	{
		// Modyfikujemy miniaturkê
		sf::Sprite *tileThumbnail_s = tileThumbnail.getSprite();
		tileThumbnail.setTexture(tileVector[tileIterator].getTexture(), 1);
		tileThumbnail_s->setRotation(0);
		// Modyfikujemy cieñ
		sf::Sprite *tileShadow_s = tileShadow.getSprite();
		tileShadow.setTexture(tileVector[tileIterator].getTexture(), 1);
		tileShadow_s->setRotation(0);
	}

	// Zmieniamy stan gry na stawianie pionka
	//Engine::GameState = ADDITIONAL_1;

}

void Engine::PlaceThePawn(Tile & tile, sf::RenderTarget &board, sf::RenderWindow &window)
{	
	// Konwertujemy teksturê na obraz
	sf::Image texImage = tile.getTexture().copyToImage();

	// Potrzebujemy pozycjê kursora wzglêdem p³ytki
	sf::Vector2i boardOffsetI{ v_2i(Board::boardOffset) }; // Przekonwertowany wektor
	sf::Vector2i mpos = sf::Mouse::getPosition(window) - boardOffsetI; // Pozycja kursora w oknie
	sf::Vector2f coord = board.mapPixelToCoords(mpos); // Pozycja kursora na mapie
	// Pozycja lewego górnego rogu p³ytki
	sf::Vector2f gpos = tile.getPosition() - 0.5f * tile.getOrigin();
	// Wektor pozycji kursora wewn¹trz p³ytki
	sf::Vector2u intPos = v_conv<unsigned int, float>(coord - gpos);

	sf::Color col = texImage.getPixel(intPos.x, intPos.y);

	// PawnPlacingHandler::NearestBorder musi zwróciæ informacjê
	// o tym, do której krawêdzi dany punkt (intPos) ma najbli¿ej.
	// Nastêpnie trzeba bêdzie tê informacjê dalej przetworzyæ z
	// uzyskanym kolorem piksela.


}

void Engine::PlaceThePawn2(sf::RenderTarget &, sf::RenderWindow &)
{
	//GameObject Circle[12];

	sf::Texture circle_texture;
	circle_texture.loadFromFile("images/circle.png");

	sf::Vector2f circle_size{ Board::Square * 0.1f };

	for (int i = 0; i < 12; i++)
	{
		Circle[i].setTexture(circle_texture);
		Circle[i].setSize(circle_size);
	}
	
	// Pozycja lewego górnego rogu p³ytki
	sf::Vector2f gpos = CurrentTile->getPosition() - 0.5f * CurrentTile->getOrigin();
	float buffer{ Board::Square.x * 0.2f };
	
	GameObject * c_temp = &Circle[0];
	{
		sf::Vector2f vec(CurrentTile->getOrigin().x, -buffer);
		c_temp->setPosition(gpos + vec);
	}

	c_temp = &Circle[1];
	{
		sf::Vector2f vec(2 * CurrentTile->getOrigin().x - buffer, CurrentTile->getOrigin().y);
		c_temp->setPosition(gpos + vec);
	}

	c_temp = &Circle[2];
	{
		sf::Vector2f vec(CurrentTile->getOrigin().x, 2 * CurrentTile->getOrigin().y - buffer);
		c_temp->setPosition(gpos + vec);
	}

	c_temp = &Circle[3];
	{
		sf::Vector2f vec(0, 2 * CurrentTile->getOrigin().y - buffer);
		c_temp->setPosition(gpos + vec);
	}

	GameState = Engine::placingPawn;


}

void Engine::PlaceThePawn3(sf::RenderTarget &target, sf::RenderWindow &window)
{
	for (int i = 0; i < 4; i++)
		Circle[i].draw(target);
}


/*
Tile::Border Engine::PawnPlacingHandler::NearestBorder(sf::Vector2u intPos)
{
	// Wymiary p³ytki (konwertujemy wektor)
	sf::Vector2u TileSize = v_conv<unsigned int, float>(Board::Square);
	

}
*/

std::ostream& operator << (std::ostream& pStream, sf::Vector2f vec)
{
	pStream << "(" << vec.x << ", " << vec.y << ")";
	return pStream;
}

/// Funkcje s³u¿¹ce do konwersji wektorów
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

