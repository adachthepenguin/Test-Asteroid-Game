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
public:
	AsteroidGraphics(Asteroid* pEntity);
	virtual ~AsteroidGraphics();

	virtual void draw(GraphicsHelper* pGraphicsHelper);
};