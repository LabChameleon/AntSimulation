#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES

#include <SFML\Graphics.hpp>
#include <iostream>
#include <math.h>

class Math
{
public:

	Math() { }

	static float distance(sf::Vector2f p1, sf::Vector2f p2)
	{
		return std::sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
	}

	static float distanceSquared(sf::Vector2f p1, sf::Vector2f p2)
	{
		return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
	}

	static int irand(int a, int e) // Liefert eine Zuffalszahl zwischen a und e
	{
		double r = e - a + 1;
		return a + (int)(r * rand() / (RAND_MAX + 1.0));
	}

	static bool getCircleCollision(sf::Vector2f center, int radius, sf::Vector2f position)
	{
		if (distance(center, position) < radius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static sf::Vector2f moveToPoint(sf::Vector2f position, sf::Vector2f destination) // gibt die Richtung zurück, in der das Nest von der aktuellen Ameisenposition liegt
	{
		double staticAngle = 0;
		double angle = 0;

		if (position.y >= destination.y)
		{
			angle = std::acos((destination.x - position.x * 1) / (std::sqrt((std::pow(destination.x - position.x, 2)) + (std::pow(destination.y - position.y, 2)))));
		}
		else
		{
			angle = 6.2831853071796 - std::acos((destination.x - position.x * 1) / (std::sqrt((std::pow(destination.x - position.x, 2)) + (std::pow(destination.y - position.y, 2)))));
		}

		return sf::Vector2f(std::cos(angle - M_PI), -std::sin(angle - M_PI));
	}
};

#endif // MATH_H
