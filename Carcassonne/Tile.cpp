#include "stdafx.h"
#include "Tile.hpp"

extern sf::Vector2f v_2f(sf::Vector2u vecU);

std::vector<Tile*> Tile::tiles_placed;

Tile::Tile(std::string _filename, float posX, float posY)
	: GameObject(_filename, posX, posY)
{
	sprite.setOrigin(v_2f(texture->getSize()) * 0.5f);
	this->setSize(sf::Vector2f(Board::squareSize, Board::squareSize));
	//sprite.setScale(Board::squareScale, Board::squareScale);
}

Tile::~Tile()
{
	this->plist.clear();
	std::cout << "~Tile: " << this << std::endl;
}

void Tile::Rotate(int n)
{
	n = (n + 4) % 4;
	for (int i = 0; i < n; i++)
	{
		auto help = U;
		U = L;
		L = D;
		D = R;
		R = help;

		auto h1 = eU;
		eU = eL;
		eL = eD;
		eD = eR;
		eR = h1;
	}
	
	if (n == 1)
	{
		auto help7 = f[7], help6 = f[6];
		for (int j = 7; j > 1; j--)
			f[j] = f[j - 2];
		f[1] = help7; f[0] = help6;
	}
	if (n == 3)
	{
		auto help0 = f[0], help1 = f[1];
		for (int j = 0; j < 6; j++)
			f[j] = f[j + 2];
		f[6] = help0; f[7] = help1;
	}

	sprite.rotate(n * 90.f);
}

void Tile::DrawAll(sf::RenderTarget &target)
{
	for (auto it = tiles_placed.begin(); it != tiles_placed.end(); it++)
	{
		(*it)->draw(target);
	}
}

void Tile::setBorders(int u, int r, int d, int l)
{
	eU = E_Border(u);
	eD = E_Border(d);
	eL = E_Border(l);
	eR = E_Border(r);
}

void Tile::Show()
{
	cout << "=======================" << endl;
	cout << "Tile [" << this << "] \"" << this->filename << "\"" << endl;
	cout << "Object list:" << endl;
	cout << "(";
	for (shared_ptr<Prop> shp : plist)
	{
		cout << shp->prop_type() << "[" << shp << "], ";
	}
	cout << ")" << endl;
	cout << "U: ";
	cout << U.p;
	cout << " " << *(U.p);
	cout << " " << (*U)->prop_type() << endl;
	cout << "R: " << *R << " " << (*R)->prop_type() << endl;
	cout << "D: " << *D << " " << (*D)->prop_type() << endl;
	cout << "L: " << *L << " " << (*L)->prop_type() << endl;

	for (int i = 0; i < 8; i++)
	{
		cout << "f[" << i << "]: ";// << *f[i] << endl;
		if (f[i] != nullptr)
			cout << *f[i];
		else
		{
			cout << "null";
		}
		cout << endl;
	}

	cout << "=======================" << endl;
}



void Tile::Place(EmptyField *_EF)
{
	// Ustawiamy wskaŸniki na odpowiednie obiekty z listy
	
	U.p = &plist[iU];
	R.p = &plist[iR];
	D.p = &plist[iD];
	L.p = &plist[iL];

	for (int i = 0; i < 8; i++)
	{
		if (iF[i] != -1)
			f[i].p = &plist[iF[i]];
		else
		{
			f[i].p = nullptr;
		}
	}
	
	this->EF = _EF;

	// Ustawiamy adekwatn¹ pozycjê
	this->setPosition(EF->getPosition());

	// Dodajemy tê p³ytkê do wektora p³ytek ju¿ po³o¿onych
	tiles_placed.push_back(this);
	
	this->EF->tile = this;

}
