#ifndef ANTCLAN_H
#define ANTCLAN_H

#include <SFML\Graphics.hpp>
#include <iostream>

#include "Map.h"

class AntClan
{
public:

	AntClan(int numberAnts, Map *map)
	{
		ants.resize(numberAnts);
		ants.setPrimitiveType(sf::Points);

		for (int i = 0; i < numberAnts; i++)
		{
			ants[i].position = map->getNestPosition();
			ants[i].color = sf::Color::White;
		}
	}

	std::vector < sf::Vector2f* > getQuadPointer() // Gibt ein Array von Zeigern auf die einzelnen Vertices des VertexArrays zurück
	{
		std::vector < sf::Vector2f* > pointer;
		pointer.resize(ants.getVertexCount());

		for (int i = 0; i < ants.getVertexCount(); i++)
		{
			pointer[i] = &ants[i].position;
		}

		return pointer;
	}

	void draw(sf::RenderWindow &window)
	{
		window.draw(ants);
	}

private:
	sf::VertexArray ants; // Array von Vertices, zur schnellen Darstellung alle Ameisen.
};

#endif // ANTCLAN_H