#pragma once
#include <list>
#include <memory>
#include "GameObject.hpp"
#include "Board.hpp"
#include "EmptyField.hpp"
#include "Prop.hpp"
#include "Creator.hpp"

using namespace std;

class Tile :
	public GameObject
	/*
	Already available:
	+ set/get position
	+ set/get size
	+ load texture
	+ load texture (filename)
	+ mouseEntered()
	+ clicked()
	+ draw()
	# shared_ptr<Texture>
	# sprite
	# filename
	*/
{
public:
	//Tile(){}
	Tile(std::string _filename, float posX=0.f, float posY=0.f);
	~Tile();

	void Place(EmptyField*);
	void Rotate(int n);

	EmptyField *EF;
	// Przyjaciele
	friend std::vector<Tile> Creator::LoadTileData();
	friend void Creator::SetTileAttributes(Tile&, std::vector<int>&, std::vector<int>&);
private:
public:
	// Wektor p�ytek, kt�re zosta�y ju� po�o�one na planszy
	static std::vector<Tile*> tiles_placed;

	static void DrawAll(sf::RenderTarget&);

	// Typ granicy - puste pole/trakt/miasto	
	enum E_Border
	{
		E_Field, E_Trail, E_Town
	};	
private:
	// Cztery granice (g�ra, d�, prawo, lewo)
	Tile::E_Border eD, eU, eL, eR;

	// Lista obiekt�w znajduj�cych si� na p�ytce (wska�niki wsp�dzielone)
	std::vector<std::shared_ptr<Prop>> plist;

public:
	// wersja 1.
	// Wska�niki na obiekty zwi�zane z danymi kraw�dziami
	//std::shared_ptr<Prop> *U, *D, *L, *R;

	// Wska�niki na pola zwi�zane z "po��wkami" kraw�dzi
	//std::shared_ptr<Prop> *f[8];

	// wersja 2.
	//std::weak_ptr<Prop> U, R, D, L;
	//std::weak_ptr<Prop> f[8];

	void setBorders(int u, int r, int d, int l);

	// wersja 3.
	struct Border
	{
	
		shared_ptr<Prop> *p;
	public:
		E_Border type;
		Border *next;

		shared_ptr<Prop> & operator * ()
		{
			return *p;
		}
		shared_ptr<Prop> * operator -> ()
		{
			return p;
		}
		void operator = (shared_ptr<Prop> *sh_p)
		{
			this->p = sh_p;
		}
		bool operator == (std::nullptr_t nt)
		{
			return this->p == nt;
		}
		bool operator != (std::nullptr_t nt)
		{
			return this->p != nt;
		}
		
	};

	Tile::Border U, R, D, L;
	Tile::Border f[8];

	void Show();
	


private:
	int iU, iR, iD, iL;
	int iF[8];
	

};

/*
void test()
{
	Tile tile;

	// Dzia�anie jest to samo
	(*tile.LL)->temp_f();
	(*tile.L)->temp_f();

	tile.LL->use_count(); // metoda z shared_ptr
	tile.L->use_count();
	
}*/