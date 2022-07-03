#ifndef MAP_H
#define MAP_H

#include <SFML\Graphics.hpp>
#include <ctime>

#include "Timer.h"
#include "Math.h"
#include "FoodSource.h"

sf::VertexArray mapTiles(sf::Points, 0);

struct MapField 
{

	MapField()
	{
		pheromonConcentration = 0; // Setzt die Pheromonkonzentration auf null (startwert)
		vertexFieldIndex = -1;	   // Setzt den Vertexindex, der auf vertex verweist, was zur Darstellung des Pheromons dient
	}

	bool update()
	{
		if (pheromonTimer.getElapsedTime().count() > pheromonTime && pheromonConcentration > 0) // Prüft, ob die Verdunstungszeit abgelaufen ist
		{
			pheromonConcentration--;  // Veringert die Pheromonkonzentration
			
			if (pheromonConcentration > 0)
			{
				mapTiles[vertexFieldIndex].color.a = pheromonConcentration * 10;  // veringert Alphawert des Vertex, für einen "Verblass" Effekt
				pheromonTimer.restart();	
				return true;
			}
			else
			{
				mapTiles[vertexFieldIndex].color = sf::Color::Black; 
				return false;
			}
		}
		
		return true;
	}

	void increasePheromonConcentration() 
	{
		if (pheromonConcentration < 25) // Pheromonkonzentration kann nicht höher als 25 werden.
		{
			pheromonConcentration++;	// Erhöht die Pheromonkonzentration
		}

		mapTiles[vertexFieldIndex].color = sf::Color::Magenta;  // Das Pheromon wird als Magentafarbendes Pixels dargestellt
		mapTiles[vertexFieldIndex].color.a = pheromonConcentration * 10;	// justiert Alphawert des Vertex, für einen "Verblass" Effekt

		if (pheromonConcentration == 1)
		{
			pheromonTimer.restart();
		}
	}

	sf::Vector2f position;  // Speichert die Position des Map Feldes
	Timer pheromonTimer;    // Timer um zu schauen, ob die Verdunstungszeit abgelaufen ist 
	unsigned int pheromonConcentration;	// Speichert die aktuelle Pheromonkonzentration
	int vertexFieldIndex;	// vertexFieldIndex, der auf das Vertex zur Darstellung des Pheromons dient
	float pheromonTime;		// Speicher die Verdunstungszeit
};

class Map
{

public:

	Map(int numberFoodSources, int foodSourceSize, float pheromonTime)
	{
		srand(time(NULL)); // Setzt die "Wurzel" des Zufallsgenerators, nach aktueller Prozessorzeit 

		this->pheromonTime = pheromonTime;

		foodSource.clear();
		foodSource.resize(numberFoodSources);

		map.clear();
		map.resize(500, std::vector < MapField >(500));

		toUpdate.clear();

		mapTiles.clear();

		for (int i = 0; i < 500; i++)
		{
			for (int j = 0; j < 500; j++)
			{	
				map[i][j].position = sf::Vector2f(i, j);
			}
		}

		nestPosition = sf::Vector2f(std::rand() % 490, std::rand() % 490);
		antNest.setPosition(nestPosition);
		antNest.setRadius(5);
		antNest.setFillColor(sf::Color(127, 0, 0));
		antNest.setOrigin(5, 5);

		for (int i = 0; i < foodSource.size(); i++)
		{
			foodSource[i] = FoodSource(sf::Vector2f(std::rand() % 490, std::rand() % 490), foodSourceSize);
		}
	}

	void update()
	{
		for (int i = 0; i < toUpdate.size(); i++)
		{
			if (!toUpdate[i]->update())
			{
				toUpdate.erase(toUpdate.begin() + i);
			}
		}
	}

	void addPheromon(sf::Vector2f position) // Erhöht Pheromonkonzentration eines Feldes
	{		
		if (map[position.x][position.y].pheromonConcentration == 0) // Prüft, ob schon Pheromon auf dem Feld vorhanden
																	// ist
		{
			if (map[position.x][position.y].vertexFieldIndex == -1) // Prüft, ob dem Map Feld schon ein Vertex zugewiesen 
										// wurde, um das Pheromon darzustellen. (-1 bedeutet kein Vertex wurde zugewiesen
			{
				mapTiles.append(sf::Vertex(position, sf::Color::Black)); // Erstellt ein neues Vertex 
				map[position.x][position.y].vertexFieldIndex = mapTiles.getVertexCount() - 1; // Setzt den VertexIndex des 
											// Feldes auf dem die Ameise steht, auf den Index des soeben erstellten Vertex
				map[position.x][position.y].pheromonTime = pheromonTime; // Setzt die Verdunstugszeit des Pheromons,
																		 // auf die vom Benutzer eingestellte Zeit
			}

			toUpdate.push_back(&map[position.x][position.y]);	// Hängt das Pheromon and die Liste der zu überprüfenden 
										   // Pheromone an, in der geschaut wird, ob die Verdunstungszeit abgelaufen ist
		}

		map[position.x][position.y].increasePheromonConcentration(); // Erhöht die Pheromonkonzentration um eins
	}

	bool isColliding(sf::Vector2f position)
	{
		for (int i = 0; i < foodSource.size(); i++)
		{
			if (foodSource[i].getNumberFood() > 0 && Math::getCircleCollision(foodSource[i].getPosition(), foodSource[i].getRadius(), position))
			{
				foodSource[i].takeAwayFood();
				return true;
			}
		}

		return false;
	}

	sf::Vector2f getNestPosition()
	{
		return nestPosition;
	}

	sf::Vector2f getNeighbour(sf::Vector2f position)
	{
		unsigned int pheromonConcentration = 0; // Speichert immer die großte gefundene Pheromonkonzentration bei einem Nachbar ab
		MapField *temp = NULL;					// Speichert immer den Nachbar mit der höchsten Pheromonkonzentration ab

		std::vector < MapField* > neighbours;	// Speicher alle Nachbarn wenn die Wahl zwischen zwei oder mehr Nachbarn besteht
		neighbours.reserve(8);

		float distanceToCenter = Math::distanceSquared(position, nestPosition); // Berechnet den quadrierten Abstand zum Nest
		
		for (int i = position.x - 1; i < position.x + 2; i++) // Geht alle Nachbarn auf der X-Achse der aktuellen Position durch
		{
			for (int j = position.y - 1; j < position.y + 2; j++)	// Geht alle Nachbarn auf der Y-Achse der aktuellen Position durch
			{
				if (i > -1 && i < 500 && j > -1 && j < 500) // Überprüft, ob die Nachbarposition, außerhalb des Feldes der Simulation liegt
				{
					if (i != position.x || j != position.y)	// Überprüft, ob die Nachbarposition, die aktuelle Position ist und somit nicht kontrolliert werden muss
					{
						float distance = Math::distanceSquared(map[i][j].position, nestPosition);

						if (map[i][j].pheromonConcentration > pheromonConcentration && distance > distanceToCenter) // Prüft, ob die Pheromonkonzentration am höchsten ist und die Distanz zum Nest größer ist,
						{
							neighbours.clear(); 

							neighbours.push_back(&map[i][j]);

							pheromonConcentration = map[i][j].pheromonConcentration;
							temp = &map[i][j];
						}
						else if (pheromonConcentration == map[i][j].pheromonConcentration && pheromonConcentration == 0) // Prüft, ob die Pheromonkonzentration 0 ist und die höchste gefundene konzentration auch 0 ist 
						{
							neighbours.push_back(&map[i][j]);
						}
						else if (pheromonConcentration == map[i][j].pheromonConcentration && pheromonConcentration > 0 && distance > distanceToCenter)
							// Prüft, ob die Pheromonkonzentration genauso groß ist, wie die höchste und die Distanz zum Nest auch größer ist als die des aktuellen Feldes
						{
							neighbours.push_back(&map[i][j]);
						}
					}
				}
			}
		}

		if (temp != NULL && neighbours.size() < 1)
		{
			return temp->position;
		}
		else if(neighbours.size() > 0)
		{
			int random = Math::irand(0, neighbours.size()-1);

			return neighbours[random]->position;
		}
	}

	void drawPlaces(sf::RenderWindow &window)
	{
		window.draw(antNest);

		for (int i = 0; i < foodSource.size(); i++)
		{
			if (foodSource[i].getNumberFood() > 0)
			{
				foodSource[i].draw(window);
			}
		}
	}

	void drawPheromons(sf::RenderWindow &window)
	{
		window.draw(mapTiles);
	}

private:

	std::vector < std::vector < MapField > > map; // Array für Map Felder, der gesamten Karte der Simulation
	std::vector < MapField* > toUpdate;		// Array in dem alle Felder mit einer Pheromonkonzentration > 0 sind, um diese schnell updaten zu können
	std::vector < FoodSource > foodSource;  // Array in dem alle Futterquellen sind
	sf::CircleShape antNest;	// Kreis zur Darstellung des Ameisennestes
	sf::Vector2f nestPosition;	// Position des Nestes
	float pheromonTime;			// Speicher die vom Benutzer eingegebene Verdunstungszeit der Pheromone
};

#endif // MAP_H