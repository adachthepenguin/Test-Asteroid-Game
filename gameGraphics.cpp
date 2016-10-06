#include "gameGraphics.h"
#include "gameObjects.h"
#include "randomizer.h"



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



AsteroidGraphics::AsteroidGraphics(Asteroid* pEntity, const float rotationDeviation)
: Graphics(pEntity)
{
	m_pAsteroid = pEntity;

	m_rotationX = Randomizer::getInstance().generateFloat(360.0f, 3600);
	m_rotationY = Randomizer::getInstance().generateFloat(360.0f, 3600);
	m_rotationZ = Randomizer::getInstance().generateFloat(360.0f, 3600);

	m_rotationXPerSec = rotationDeviation * (-1.0f + Randomizer::getInstance().generateFloat(2.0f, 1000));
	m_rotationYPerSec = rotationDeviation * (-1.0f + Randomizer::getInstance().generateFloat(2.0f, 1000));
	m_rotationZPerSec = rotationDeviation * (-1.0f + Randomizer::getInstance().generateFloat(2.0f, 1000));
}

AsteroidGraphics::~AsteroidGraphics() {}

void AsteroidGraphics::update(const float seconds)
{
	Graphics::update(seconds);

	m_height = m_pAsteroid->getHeight();

	m_rotationX += m_rotationXPerSec * seconds;
	m_rotationY += m_rotationYPerSec * seconds;
	m_rotationZ += m_rotationZPerSec * seconds;

	while (m_rotationX < 0.0f) { m_rotationX += 360.0f; }
	while (m_rotationX >= 360.0f) { m_rotationX -= 360.0f; }
	while (m_rotationY < 0.0f) { m_rotationY += 360.0f; }
	while (m_rotationY >= 360.0f) { m_rotationY -= 360.0f; }
	while (m_rotationZ < 0.0f) { m_rotationZ += 360.0f; }
	while (m_rotationZ >= 360.0f) { m_rotationZ -= 360.0f; }
}

void AsteroidGraphics::draw(GraphicsHelper* pGraphicsHelper)
{
	pGraphicsHelper->setPosition(m_posX, m_posY);
	pGraphicsHelper->setRotation(m_rotationX, m_rotationY, m_rotationZ);
	pGraphicsHelper->setScale(m_length, m_width, m_height);
	pGraphicsHelper->setAlpha(1.0f);
	pGraphicsHelper->setTexture(2);
	pGraphicsHelper->drawCube();
}