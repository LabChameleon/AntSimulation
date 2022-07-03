#ifndef FOODSOURCE_H
#define FOODSOURCE_H

#include <SFML\Graphics.hpp>

class FoodSource
{
public:
		
	FoodSource() { }

	FoodSource(sf::Vector2f position, int size)
	{
		food = size;

		alphaFactor = float(255.f / food);

		shape.setRadius(10);
		shape.setFillColor(sf::Color(211, 201, 192));
		shape.setOrigin(shape.getRadius(), shape.getRadius());
		shape.setPosition(position);
	}

	void takeAwayFood()
	{
		food--;			// Veringert Anzahl des Futters der Futterquelle
		shape.setFillColor(sf::Color(211, 201, 192, alphaFactor * food));	// Lässt Futterquelle mit verblassen, wenn sie kleiner wird.
	}

	int getRadius()
	{
		return 10;
	}

	sf::Vector2f getPosition()
	{
		return shape.getPosition();
	}

	void draw(sf::RenderWindow &window)
	{
		window.draw(shape);
	}

	int getNumberFood()
	{
		return food;
	}

private:

	int food;		// Speichert die Anzahl des Futtes, dass noch in der Futterquelle ist
	sf::CircleShape shape;	// Dient zur Darstellung der Futterquelle
	float alphaFactor;		// Dient als Berechnungswert, damit die Futterquelle "verblassen" kann
};

#endif // FOODSOURCE_H