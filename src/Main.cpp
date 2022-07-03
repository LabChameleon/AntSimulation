#include <iostream>
#include <SFML\Graphics.hpp>
#include <vector>

#include "Ant.h"
#include "Map.h"
#include "AntClan.h"

void startSimulation(int numberAnts, int numberFoodSources, int foodSourceSize, float pheromonTime);

int main()
{
	int numberAnts;			// Speichert Anzahl der Ameisen für die Simulation
	int numberFoodSources;  // Speihert Anzahl der Futterquellen für die Simulation
	int foodSourceSize;		// Speichert die groesse der Futterquellen
	float pheromonTime;		// Speichert die Verdunstungszeit der Pheromone
	int newSimulation;		// Registriert, ob der Benutzer eine neue Simulation starten moechte
		
	do
	{
		system("cls");

		do
		{
			std::cout << "Geben sie die Anzahl an Ameisen an (1-10000): ";
			std::cin >> numberAnts;

			if(numberAnts < 1 || numberAnts > 10000)
			{
				std::cout << "Die Eingabe ist ausserhalb des Wertebereichs!" << std::endl << std::endl;
			}

		} while (numberAnts < 1 || numberAnts > 10000);

		do
		{
			std::cout << "Geben sie die Anzahl der Futterquellen an (1-25): ";
			std::cin >> numberFoodSources;

			if (numberFoodSources < 1 || numberFoodSources > 25)
			{
				std::cout << "Die Eingabe ist ausserhalb des Wertebereichs!" << std::endl << std::endl;
			}

		} while (numberFoodSources < 1 || numberFoodSources > 25);

		do
		{
			std::cout << "Geben sie die Groesse der Futterquellen an (10-10000): ";
			std::cin >> foodSourceSize;

			if (foodSourceSize < 10 || foodSourceSize > 10000)
			{
				std::cout << "Die Eingabe ist ausserhalb des Wertebereichs!" << std::endl << std::endl;
			}

		} while (foodSourceSize < 10 || foodSourceSize > 10000);

		do
		{
			std::cout << "Geben sie die Dauer der Pheromone in Sekunden an (0.01 - 30): ";
			std::cin >> pheromonTime;

			if (pheromonTime < 0.01f || pheromonTime > 30)
			{
				std::cout << "Die Eingabe ist ausserhalb des Wertebereichs!" << std::endl << std::endl;
			}

		} while (pheromonTime < 0.01f || pheromonTime > 30);


		startSimulation(numberAnts, numberFoodSources, foodSourceSize, pheromonTime);

		do
		{
			std::cout << std::endl << "Wollen Sie eine neue Simulation starten? (Nein = 0| Ja = 1): ";
			std::cin >> newSimulation;

			if (newSimulation != 0 && newSimulation != 1)
			{
				std::cout << "Ungueltige Eingabe!" << std::endl << std::endl;
			}

		} while (newSimulation != 0 && newSimulation != 1);

	} while (newSimulation == 1);

	return 0;
}

void startSimulation(int numberAnts, int numberFoodSources, int foodSourceSize, float pheromonTime)
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Ameisen-Simulation"); // Erstellt das Fenster zur Darstellung

	std::vector <Ant> ants;		// Array, dass alle erstellten Ameisen beihnaltet
	ants.resize(numberAnts);

	Timer frameRateTimer;		// Timer der die Frame-Rate  reguliert
	frameRateTimer.start();

	Timer antSpeedTimer;		// Timer der die Geschwindigkeit der Ameisen reguliert
	antSpeedTimer.start();

	Map map(numberFoodSources, foodSourceSize, pheromonTime);  // Erstellt eine neue Karte, mit den Parametern des Benutzers
	AntClan antClan(ants.size(), &map);

	std::vector < sf::Vector2f* > quadPointer = antClan.getQuadPointer();	// Array mit Zeigern auf die Vertices zur Darstellung der Ameisen

	for (int i = 0; i < ants.size(); i++)
	{
		ants[i] = Ant(&map, quadPointer[i]);  // Ameisen werden erstellt mit ihrem individuellen Vertex zur Darstellung
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (antSpeedTimer.getElapsedTime().count() > 1.f / 100.f)
		{
			antSpeedTimer.restart();

			map.update();

			for (int i = 0; i < ants.size(); i++)
			{
				ants.at(i).updateAnt();
			}
		}

		if (frameRateTimer.getElapsedTime().count() > 1.f / 60.f)
		{
			frameRateTimer.restart();

			window.clear();

			map.drawPheromons(window);
			antClan.draw(window);
			map.drawPlaces(window);

			window.display();
		}
	}
}