#pragma once
#include "GameObject.hpp"
#include "Board.hpp"
#include <memory>
//#include "Tile.hpp"

class Tile;

class EmptyField :
	public GameObject, public Board
{
public:
	//static std::shared_ptr<sf::Texture> texture;
private:
	static sf::Texture *texture;
	static int t_count;
	const static std::string filename;
	static float scale;

public:
	std::unique_ptr<GameObject> _plus;
	int indX; // Tymczasowe pomocnicze
	int indY;
	/// TEST
	sf::Text text;
	sf::Font font;
	void setText();

	static std::vector <EmptyField*> EF_vector;

	static void LoadTexture();
	static sf::Vector2f getSize();
	static void Clear() { delete texture; }
	static void DrawAll(sf::RenderTarget&);

	EmptyField *Up, *Down, *Left, *Right;
	Tile *tile;

	EmptyField();
	~EmptyField();
};

