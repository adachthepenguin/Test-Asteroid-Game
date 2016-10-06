#include "randomizer.h"
#include <random>
#include <ctime>



Randomizer::Randomizer()
{
	srand(static_cast<int>(time(0)));
}

Randomizer& Randomizer::getInstance()
{
	static Randomizer randomizer;

	return randomizer;
}

float Randomizer::generateFloat(const float range, const int steps) const
{
	return (range * (rand() % (steps + 1))) / steps;
}

int Randomizer::generateInt(const int range) const
{
	return rand() % (range + 1);
}