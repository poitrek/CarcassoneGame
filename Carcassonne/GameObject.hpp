#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>


class GameObject
{
protected:
	//sf::Texture *texture; // UWAGA: problem z instancj¹ tekstury. Zadeklarowanie tutaj
						  // obiektu tekstury (a nie jej wskaŸnika) zwraca wyj¹tek
						  // podczas tworzenia obiektu klasy pochodnej Player, nie wiem dlaczego..

	// Unikalny wskaŸnik na teksturê - dziêki niemu nie trzeba pamiêtaæ o delete
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite sprite;
	std::string filename;
	// Dodatkowo przechowujemy tu origin - przydaje siê w jednym miejscu	
	sf::Vector2f origin{ 0.f, 0.f };

public:
	///void setTexture(sf::Texture& texture); // To by³y ewentualne funkcje do sprawdzienia[...]
	///sf::Sprite& GetSprite();
	///GameObject() {}

	GameObject(float posX = 0, float posY = 0);
	GameObject(std::string _filename, float posX = 0, float posY = 0);
	virtual ~GameObject();

	sf::Sprite * getSprite();
public:
	friend void setOriginToCenter(GameObject &obj);
	friend void Rotate(GameObject &, int); // DODATKOWE GÓWNO
	friend void setRotation(GameObject &, int); // DODATKOWE GÓWNO

	virtual void setPosition(sf::Vector2f); // Ustawiamy pozycjê
	virtual void setPosition(float x, float y); // Ustawiamy pozycjê
	virtual sf::Vector2f getPosition(); // Zwraca pozycjê
	void setSize(sf::Vector2f); // Ustawiamy rozmiar (za pomoc¹ setScale)
	void setOrigin(sf::Vector2f);
	sf::Vector2f getSize(); // Zwraca rozmiar
	void setTexture(sf::Texture, bool holdSize = false);
	sf::Texture getTexture(); // Zwraca teksturê (kopiê)

	virtual void LoadTexture(); // Alokuje pamiêæ dla tekstury i wczytuje j¹ z atrybutu filename
	virtual void LoadTexture(std::string _filename); // Alokuje pam. dla tekstury i wczytuje j¹ z podanego par.

	virtual bool mouseEntered(sf::RenderTarget&, sf::RenderWindow&); // Sprawdza, czy najechaliœmy myszk¹
	virtual bool clicked(sf::RenderTarget&, sf::RenderWindow&); // Sprawda, czy kliknêliœmy na obiekt
	/// virtual void update(); // Mo¿e w przysz³oœci siê przyda
	virtual void draw(sf::RenderTarget&); // Funkcja odpowiedzialna za rysowanie obiektu
};

void setOriginToCenter(GameObject &obj);
void Rotate(GameObject &obj, int n); // PRZYDA SIÊ TYLKO DO TESTÓW
void setRotation(GameObject &obj, int n);
