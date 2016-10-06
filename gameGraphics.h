#pragma once

#include "graphics.h"



class Spaceship;
class Projectile;
class Asteroid;



class SpaceshipGraphics : public Graphics
{
protected:
public:
	SpaceshipGraphics(Spaceship* pEntity);
	virtual ~SpaceshipGraphics();

	virtual void draw(GraphicsHelper* pGraphicsHelper);
};



class ProjectileGraphics : public Graphics
{
protected:
public:
	ProjectileGraphics(Projectile* pEntity);
	virtual ~ProjectileGraphics();

	virtual void draw(GraphicsHelper* pGraphicsHelper);
};



class AsteroidGraphics : public Graphics
{
protected:
	Asteroid* m_pAsteroid;

	float m_height;

	float m_rotationX;
	float m_rotationY;
	float m_rotationZ;

	float m_rotationXPerSec;
	float m_rotationYPerSec;
	float m_rotationZPerSec;

public:
	AsteroidGraphics(Asteroid* pEntity, const float rotationDeviation);
	virtual ~AsteroidGraphics();

	virtual void update(const float seconds);
	virtual void draw(GraphicsHelper* pGraphicsHelper);
};