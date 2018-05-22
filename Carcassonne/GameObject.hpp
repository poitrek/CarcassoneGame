#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>


class GameObject
{
protected:
	//sf::Texture *texture; // UWAGA: problem z instancj� tekstury. Zadeklarowanie tutaj
						  // obiektu tekstury (a nie jej wska�nika) zwraca wyj�tek
						  // podczas tworzenia obiektu klasy pochodnej Player, nie wiem dlaczego..

	// Unikalny wska�nik na tekstur� - dzi�ki niemu nie trzeba pami�ta� o delete
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite sprite;
	std::string filename;
	// Dodatkowo przechowujemy tu origin - przydaje si� w jednym miejscu	
	sf::Vector2f origin{ 0.f, 0.f };

public:
	///void setTexture(sf::Texture& texture); // To by�y ewentualne funkcje do sprawdzienia[...]
	///sf::Sprite& GetSprite();
	///GameObject() {}

	GameObject(float posX = 0, float posY = 0);
	GameObject(std::string _filename, float posX = 0, float posY = 0);
	virtual ~GameObject();

	sf::Sprite * getSprite();
public:
	friend void setOriginToCenter(GameObject &obj);
	friend void Rotate(GameObject &, int); // DODATKOWE G�WNO
	friend void setRotation(GameObject &, int); // DODATKOWE G�WNO

	virtual void setPosition(sf::Vector2f); // Ustawiamy pozycj�
	virtual void setPosition(float x, float y); // Ustawiamy pozycj�
	virtual sf::Vector2f getPosition(); // Zwraca pozycj�
	void setSize(sf::Vector2f); // Ustawiamy rozmiar (za pomoc� setScale)
	void setOrigin(sf::Vector2f);
	sf::Vector2f getSize(); // Zwraca rozmiar
	void setTexture(sf::Texture, bool holdSize = false);
	sf::Texture getTexture(); // Zwraca tekstur� (kopi�)

	virtual void LoadTexture(); // Alokuje pami�� dla tekstury i wczytuje j� z atrybutu filename
	virtual void LoadTexture(std::string _filename); // Alokuje pam. dla tekstury i wczytuje j� z podanego par.

	virtual bool mouseEntered(sf::RenderTarget&, sf::RenderWindow&); // Sprawdza, czy najechali�my myszk�
	virtual bool clicked(sf::RenderTarget&, sf::RenderWindow&); // Sprawda, czy klikn�li�my na obiekt
	/// virtual void update(); // Mo�e w przysz�o�ci si� przyda
	virtual void draw(sf::RenderTarget&); // Funkcja odpowiedzialna za rysowanie obiektu
};

void setOriginToCenter(GameObject &obj);
void Rotate(GameObject &obj, int n); // PRZYDA SI� TYLKO DO TEST�W
void setRotation(GameObject &obj, int n);
