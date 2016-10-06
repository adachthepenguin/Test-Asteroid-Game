#pragma once



class Randomizer
{
private:
	Randomizer();
	
public:
	static Randomizer& getInstance();

	float generateFloat(const float range, const int steps) const;
	int generateInt(const int range) const;
};