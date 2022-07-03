#ifndef ANT_H
#define ANT_H

#include <SFML\Graphics.hpp>
#include <iostream>

#include "Map.h"
#include "Math.h"

class Ant
{
public:

	Ant()
	{

	}

	Ant(Map *map, sf::Vector2f *position)
	{
		this->map = map;
		nestPosition = map->getNestPosition();

		movementToCenter = sf::Vector2f(0,0);
		this->antPosition = position;
		hasFood = false;
	}

	void updateAnt()
	{	
		if(hasFood) // Überprüft, ob die Ameise schon ein Futter aufgenommen hat oder nicht
		{
			if (movementToCenter.x != 0 || movementToCenter.y != 0) // Überprüft, ob für die Ameise schon eine Richtung zum Nest berechnet wurde
			{
				if (antPosition->x > nestPosition.x - 5 && antPosition->x < nestPosition.x + 5 
					&& antPosition->y > nestPosition.y - 5 && antPosition->y < nestPosition.y + 5)
					// Überprüft, ob die Ameise das Nest schon erreicht hat, oder ob sie sich weiter in Richtung Nest bewegen soll
				{
					movementToCenter = sf::Vector2f(0, 0); // Setzt die Richtung zum Nest wieder auf Null, da die Ameise das Nest erreicht hat
					hasFood = false;	// Die Ameise legt ihr Futter ab, weil sie das Nest erreicht hat, ist also wieder ohne Futter unterwegs
				}

				antPosition->x = antPosition->x - movementToCenter.x; // Setzt die neue Position der Ameise in Richtung des Zentrums
				antPosition->y = antPosition->y - movementToCenter.y; // Setzt die neue Position der Ameise in Richtung des Zentrums
			}
			else
			{
				movementToCenter = Math::moveToPoint(*antPosition, nestPosition); // Berechnet die Richtung in die, die Ameise zum Zentrum gelangt. 
																				  // Speichert diese in movementToCenter
			}

			map->addPheromon(*antPosition);	// Nachdem die Ameise bewegt wurde, erschafft oder erfrischt sie auf ihrer neuen Position die Pheromonspur.
		}
		else
		{
			*antPosition = map->getNeighbour(*antPosition);	//  Schaut, ob Pheromonspuren in der Nähe der Ameise sind, oder gibt ein zufällig ausgewähltes 
														    //  Nachbarfeld der Ameise zurück
			hasFood = map->isColliding(*antPosition);		//  Prüft, ob die Ameise mit einer Futterquelle kollidiert, und setzt entscheidet demnach, ob 
															//  die Ameise Futter hat oder nicht
		}
	}

private:

	Map *map; // Zeigt auf das Map Objekt der aktuellen Simulation
	sf::Vector2f *antPosition;		// Zeigt auf das Vertex von VertexArray der Klasse "AntClan"
	sf::Vector2f movementToCenter;	// Speichert die Richtung zum Nest für die Ameise
	sf::Vector2f nestPosition;		// Speichert die Position des Nestes
	bool hasFood;					// Speicher ob die Ameise Futter aufgenommen hat oder nicht
};

#endif // ANT_H