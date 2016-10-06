#include "gameGraphics.h"
#include "gameObjects.h"



SpaceshipGraphics::SpaceshipGraphics(Spaceship* pEntity)
: Graphics(pEntity) {}

SpaceshipGraphics::~SpaceshipGraphics() {}

void SpaceshipGraphics::draw(GraphicsHelper* pGraphicsHelper)
{
	pGraphicsHelper->setPosition(m_posX, m_posY);
	pGraphicsHelper->setDirection(m_direction);
	pGraphicsHelper->setScale(m_length, m_width);
	pGraphicsHelper->setAlpha(1.0f);
	pGraphicsHelper->setTexture(0);
	pGraphicsHelper->drawRectangle();
}



ProjectileGraphics::ProjectileGraphics(Projectile* pEntity)
: Graphics(pEntity) {}

ProjectileGraphics::~ProjectileGraphics() {}

void ProjectileGraphics::draw(GraphicsHelper* pGraphicsHelper)
{
	pGraphicsHelper->setPosition(m_posX, m_posY);
	pGraphicsHelper->setDirection(m_direction);
	pGraphicsHelper->setScale(m_length, m_width);
	pGraphicsHelper->setAlpha(1.0f);
	pGraphicsHelper->setTexture(1);
	pGraphicsHelper->drawRectangle();
}



AsteroidGraphics::AsteroidGraphics(Asteroid* pEntity)
: Graphics(pEntity) {}

AsteroidGraphics::~AsteroidGraphics() {}

void AsteroidGraphics::draw(GraphicsHelper* pGraphicsHelper)
{
	pGraphicsHelper->setPosition(m_posX, m_posY);
	pGraphicsHelper->setDirection(m_direction);
	pGraphicsHelper->setScale(m_length, m_width);
	pGraphicsHelper->setAlpha(1.0f);
	pGraphicsHelper->setTexture(2);
	pGraphicsHelper->drawRectangle();
}