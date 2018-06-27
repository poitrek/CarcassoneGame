#include "stdafx.h"
#include "Creator.hpp"
#include "Tile.hpp"
#include <sstream>
#include <chrono>
#include <random>
#include <algorithm>



const std::string Creator::data_filename{ "../Carcassonne/Dane_v2.txt" };


void Creator::SetTileAttributes(Tile &tile, std::vector<int> &indx_vector, std::vector<int> &othr_vector)
{
	// Dla ka¿dego indeksu dodajemy do listy plist
	// obiekt odpowiedniej klasy
	for (int indx : indx_vector)
	{
		// Sposób 2
		switch (indx)
		{
		case 2:
			tile.plist.emplace_back(shared_ptr<Town>(new Town));
			break;
		case 1:
			tile.plist.emplace_back(shared_ptr<Trail>(new Trail));
			break;
		case 0:
			tile.plist.emplace_back(shared_ptr<Field>(new Field));
			break;
		default:
			break;
		}

	}

	int i = othr_vector[0] - 1;

	tile.U = &tile.plist[i];  // Ustawiamy odpowiedni wskaŸnik
	tile.U.type = Tile::E_Border(indx_vector[i]); // Odpowiedni typ granicy

	i = othr_vector[1] - 1;
	tile.R = &tile.plist[i];
	tile.R.type = Tile::E_Border(indx_vector[i]);

	i = othr_vector[2] - 1;
	tile.D = &tile.plist[i];
	tile.D.type = Tile::E_Border(indx_vector[i]);

	i = othr_vector[3] - 1;
	tile.L = &tile.plist[i];
	tile.L.type = Tile::E_Border(indx_vector[i]);

	/*
	cout << &tile.plist[0] << endl;
	cout << tile.U.p << endl;

	cout << "* "<< tile.plist[0] << endl;
	cout << "* " << *(tile.U) << endl;
	*/

	for (int j = 0; j < 8; j++)
	{

		i = othr_vector[j + 4] - 1;
		if (i == -1)
		{
			tile.f[j] = nullptr;
		}
		else
		{
			tile.f[j] = &tile.plist[i];
			// Nie wiem, czy aby na pewno nie spowoduje to niechcianych problemów
			// Jednak w¹tpiê, aby to dzia³a³o
			//tile.f[j] = &std::shared_ptr<Field>(dynamic_cast<Field*>(p));
		}

	}

	/* Kwestiê konwersji mo¿emy te¿ omin¹æ, zmieniaj¹c typ
	shared_ptr<Field> *f[8]  na
	shared_ptr<Prop> *f[8]
	Niemniej mo¿liwe, ¿e i tak zajdzie w programie potrzeba
	u¿ycia RTTI z inteligentnymi wskaŸnikami
	*/
}

std::vector<Tile> Creator::LoadTileData()
{
	std::ifstream file;
	file.open(Creator::data_filename);
	if (!file.is_open())
	{
		std::cout << "Error: nie udalo sie otworzyc pliku z danymi" << std::endl;
		std::cin.get();
		exit(1);
	}

	int numberOfTiles;
	file >> numberOfTiles;

	std::vector<Tile> _tileVector;
	for (int _it = 0; _it < numberOfTiles; _it++)
	{
		// 0. LICZBA KOPII P£YTKI
		int copy_number{ 1 };

		{
			string s_cn;
			file >> s_cn;

			regex r("x(\\d{1,2})");
			smatch sm;
			if (regex_match(s_cn, sm, r))
			{
				copy_number = stoi(sm[1]);
			}
		}
		// 1. NAZWA PLIKU
		std::string t_filename;
		file >> t_filename;

		t_filename = "../Carcassonne/images/" + t_filename;

		Tile tile(t_filename);

		// 2. LISTA OBIEKTÓW
		std::string prop_list_s;
		std::getline(file, prop_list_s);
		std::getline(file, prop_list_s);
		std::istringstream iss(prop_list_s);

		std::vector<int> indx_temp; // Tymczasowy wektor pomocniczy,
									// przechowuj¹cy indeksy obiektów znajduj¹cych
									//  siê na odczytywanej p³ytce

									// Split'ujemy pobran¹ z pliku liniê do wektora
		for (std::string s; iss >> s; indx_temp.push_back(stoi(s)));

		// Wype³niamy wstêpnie wektor obiektów na p³ytce
		for (int indx : indx_temp)
		{
			switch (indx)
			{
			case 2:
				tile.plist.emplace_back(shared_ptr<Town>(new Town));
				break;
			case 1:
				tile.plist.emplace_back(shared_ptr<Trail>(new Trail));
				break;
			case 0:
				tile.plist.emplace_back(shared_ptr<Field>(new Field));
				break;
			default:
				break;
			}
		}

		// 3. U, R, D, L, u1 u2 r1 r2 d1 d2 l1 l2
		// Wczytujemy do kolejnego wektora 12 pozosta³ych parametrów
		// opisuj¹cych granice p³ytki

		/*
		std::vector<int> other_temp;
		for (int i = 0; i < 12; i++)
		{
		int x; file >> x;
		other_temp.push_back(x);
		}*/

		// Indeksy U, R, D, L
		file >> tile.iU >> tile.iR >> tile.iD >> tile.iL;
		tile.iU--; tile.iR--; tile.iD--; tile.iL--;
		// Typy granic U, R, D, L
		tile.U.type = Tile::E_Border(indx_temp[tile.iU]);
		tile.R.type = Tile::E_Border(indx_temp[tile.iR]);
		tile.D.type = Tile::E_Border(indx_temp[tile.iD]);
		tile.L.type = Tile::E_Border(indx_temp[tile.iL]);
		// Indeksy u1,u2,r1,...
		for (int i = 0; i < 8; i++)
		{
			file >> tile.iF[i];
			tile.iF[i]--;
		}

		//Creator::SetTileAttributes(tile, indx_temp, other_temp);		
		//tile.Show();

		for (int i = 0; i < copy_number; i++)
			_tileVector.push_back(tile);
	}

	file.close();


	// Mieszamy wektor z p³ytkami
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	std::shuffle(_tileVector.begin() + 1, _tileVector.end(),
		std::default_random_engine(seed));

	return _tileVector;
}

EmptyField** Creator::SetEmptyFields()
{
	EmptyField **EFtab = new EmptyField*[Board::EF_X];
	for (int i = 0; i < Board::EF_X; i++)
	{
		EFtab[i] = new EmptyField[Board::EF_Y];
	}
	// Pozycje
	sf::Vector2f sqfSize = EmptyField::getSize();
	sf::Vector2f buffer{ sqfSize*0.5f };
	for (int i = 0; i < Board::EF_X; i++)
		for (int j = 0; j < Board::EF_Y; j++)
		{
			// w celach testowych
			EFtab[i][j].indX = i;
			EFtab[i][j].indY = j;

			EFtab[i][j].tile = nullptr;
			// Poprawione indeksowanie
			EFtab[i][j].setPosition(buffer.x + j * sqfSize.x, buffer.y + i * sqfSize.y);
		}

	// S¹siednie pola
	for (int i = 1; i < Board::EF_X - 1; i++)
	{
		for (int j = 1; j < Board::EF_Y - 1; j++)
		{
			EFtab[i][j].Up = &EFtab[i - 1][j];
			EFtab[i][j].Down = &EFtab[i + 1][j];
			EFtab[i][j].Left = &EFtab[i][j - 1];
			EFtab[i][j].Right = &EFtab[i][j + 1];

		}
	}
	for (int i = 1; i < Board::EF_X - 1; i++)
	{
		EFtab[i][0].Up = &EFtab[i - 1][0];
		EFtab[i][0].Down = &EFtab[i + 1][0];
		//EFtab[i][0].Left = &EFtab[i][- 1];
		EFtab[i][0].Right = &EFtab[i][1];

		EFtab[i][Board::EF_Y - 1].Up = &EFtab[i - 1][Board::EF_Y - 1];
		EFtab[i][Board::EF_Y - 1].Down = &EFtab[i + 1][Board::EF_Y - 1];
		EFtab[i][Board::EF_Y - 1].Left = &EFtab[i][Board::EF_Y - 2];
		//EFtab[i][Board::EF_Y - 1].Right = &EFtab[i][Board::EF_Y];
	}
	for (int j = 1; j < Board::EF_Y - 1; j++)
	{
		//EFtab[0][j].Up = &EFtab[- 1][j];
		EFtab[0][j].Down = &EFtab[1][j];
		EFtab[0][j].Left = &EFtab[0][j - 1];
		EFtab[0][j].Right = &EFtab[0][j + 1];

		EFtab[Board::EF_X - 1][j].Up = &EFtab[Board::EF_X - 2][j];
		//EFtab[Board::EF_X - 1][j].Down = &EFtab[Board::EF_X][j];
		EFtab[Board::EF_X - 1][j].Left = &EFtab[Board::EF_X - 1][j - 1];
		EFtab[Board::EF_X - 1][j].Right = &EFtab[Board::EF_X - 1][j + 1];
	}
	{
		EFtab[0][0].Right = &EFtab[0][1];
		EFtab[0][0].Down = &EFtab[1][0];
		EFtab[0][Board::EF_Y - 1].Left = &EFtab[0][Board::EF_Y - 2];
		EFtab[0][Board::EF_Y - 1].Down = &EFtab[1][Board::EF_Y - 1];
		EFtab[Board::EF_X - 1][0].Up = &EFtab[Board::EF_X - 2][0];
		EFtab[Board::EF_X - 1][0].Right = &EFtab[Board::EF_X - 1][1];
		EFtab[Board::EF_X - 1][Board::EF_Y - 1].Up = &EFtab[Board::EF_X - 2][Board::EF_Y - 1];
		EFtab[Board::EF_X - 1][Board::EF_Y - 1].Left = &EFtab[Board::EF_X - 1][Board::EF_Y - 2];
	}

	return EFtab;
}

void Creator::ClearEmptyFields(EmptyField** &EFtab)
{
	for (int i = 0; i < Board::EF_X; i++)
	{
		delete[] EFtab[i];
	}
	delete[] EFtab;
}


