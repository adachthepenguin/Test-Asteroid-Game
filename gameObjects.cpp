#include "gameObjects.h"
#include "gameGraphics.h"
#include "randomizer.h"



Spaceship::Spaceship(const unsigned id)
: Entity(OT_SPACESHIP, id)
{
	m_projectileSpeed = 0.0f;
	m_rateOfFire = 1.0f;
	m_speedLimit = 0.0f;
	m_speedAcc = 0.0f;
	m_speedDec = 0.0f;

	m_attackCooldown = 0.0f;
	m_attackEnabled = true;

	m_movementState = SM_IDLE;
}

Spaceship::~Spaceship() {}

void Spaceship::attack()
{
	if (!m_attackEnabled) { return; }

	float projectileSize = 0.1f;
	float spawnX = m_posX;
	float spawnY = m_posY + 0.5f * (m_length + projectileSize);

	Projectile* pProjectile = m_pEntityFactory->createProjectile(m_pEntityManager, m_ID);
	pProjectile->setX(spawnX);
	pProjectile->setY(spawnY);
	pProjectile->setDirection(m_direction);
	pProjectile->setLength(projectileSize);
	pProjectile->setWidth(projectileSize);
	pProjectile->setYPerSec(m_projectileSpeed);

	m_attackCooldown = 1.0f / m_rateOfFire;
	m_attackEnabled = false;
}

void Spaceship::applyMovement(const SPACESHIP_MOVEMENT movement)
{
	switch (movement)
	{
		case SM_MOVELEFT:
			switch (m_movementState)
			{
				case SM_IDLE:
				case SM_STOPLEFT:
				case SM_STOPRIGHT:
					m_movementState = SM_MOVELEFT;
					break;
			}
			break;
		case SM_MOVERIGHT:
			switch (m_movementState)
			{
			case SM_IDLE:
			case SM_STOPRIGHT:
			case SM_STOPLEFT:
				m_movementState = SM_MOVERIGHT;
				break;
			}
			break;
		case SM_STOPLEFT:
			if (m_movementState == SM_MOVELEFT)
			{
				m_movementState = SM_STOPLEFT;
			}
			break;
		case SM_STOPRIGHT:
			if (m_movementState == SM_MOVERIGHT)
			{
				m_movementState = SM_STOPRIGHT;
			}
			break;
	}
}

void Spaceship::setBounds(const float left, const float right)
{
	m_leftBound = left;
	m_rightBound = right;
}

bool Spaceship::initialize()
{
	m_projectileSpeed = 5.0f;
	m_rateOfFire = 1.0f;
	m_speedLimit = 1.5f;
	m_speedAcc = 3.0f;
	m_speedDec = 5.0f;

	return Entity::initialize();
}

void Spaceship::update(const float seconds)
{
	if (!m_attackEnabled)
	{
		m_attackCooldown -= seconds;
		if (m_attackCooldown <= 0.0f)
		{
			m_attackCooldown = 0.0f;
			m_attackEnabled = true;
		}
	}

	switch (m_movementState)
	{
		case SM_MOVELEFT:
			m_posXPerSec -= m_speedAcc * seconds;
			if (m_posXPerSec < -m_speedLimit)
			{
				m_posXPerSec = -m_speedLimit;
			}
			break;
		case SM_MOVERIGHT:
			m_posXPerSec += m_speedAcc * seconds;
			if (m_posXPerSec > m_speedLimit)
			{
				m_posXPerSec = m_speedLimit;
			}
			break;
		case SM_STOPLEFT:
			m_posXPerSec += m_speedDec * seconds;
			if (m_posXPerSec > 0.0f)
			{
				m_posXPerSec = 0.0f;
				m_movementState = SM_IDLE;
			}
			break;
		case SM_STOPRIGHT:
			m_posXPerSec -= m_speedDec * seconds;
			if (m_posXPerSec < 0.0f)
			{
				m_posXPerSec = 0.0f;
				m_movementState = SM_IDLE;
			}
			break;
	}

	Entity::update(seconds);

	if (m_posX < m_leftBound) { m_posX = m_leftBound; }
	else if (m_posX > m_rightBound) { m_posX = m_rightBound; }
}

bool Spaceship::createGraphics()
{
	if (m_pGraphics) { return false; }

	m_pGraphics = new SpaceshipGraphics(this);

	return true;
}



Projectile::Projectile(const unsigned id, const unsigned spawnerID)
: Entity(OT_PROJECTILE, id)
{
	m_spawnerID = spawnerID;

	m_destroyAboveY = 0.0f;
}

Projectile::~Projectile() {}

bool Projectile::initialize()
{
	m_destroyAboveY = 5.0f;

	return Entity::initialize();
}

void Projectile::update(const float seconds)
{
	Entity::update(seconds);

	if (m_posY > m_destroyAboveY)
	{
		setRemoveMark(true);
	}
}

bool Projectile::createGraphics()
{
	if (m_pGraphics) { return false; }

	m_pGraphics = new ProjectileGraphics(this);

	return true;
}



Asteroid::Asteroid(const unsigned id)
: Entity(OT_ASTEROID, id)
{
	m_destroyBelowY = 0.0f;
}

Asteroid::~Asteroid() {}

void Asteroid::setHeight(const float height)
{
	m_height = height;
}

float Asteroid::getHeight() const
{
	return m_height;
}

bool Asteroid::initialize()
{
	m_destroyBelowY = -5.0f;

	return Entity::initialize();
}

void Asteroid::update(const float seconds)
{
	Entity::update(seconds);

	if (m_posY < m_destroyBelowY)
	{
		setRemoveMark(true);
	}
}

float Asteroid::getBoundingCircle() const
{
	return sqrtf(m_length*m_length + m_width*m_width + m_height*m_height);
}

bool Asteroid::createGraphics()
{
	if (m_pGraphics) { return false; }

	m_pGraphics = new AsteroidGraphics(this, 30.0f);

	return true;
}



AsteroidSpawner::AsteroidSpawner(const unsigned id)
: Entity(OT_ASTEROIDSPAWNER, id)
{
	m_spawnRate = 1.0f;
	m_spawnProgress = 0.0f;
	m_startingSpeed = 0.0f;

	m_height = 0.0f;
	m_sizePercDeviation = 0.0f;
}

AsteroidSpawner::~AsteroidSpawner() {}

void AsteroidSpawner::spawnAsteroid()
{
	m_posX = m_leftBound + Randomizer::getInstance().generateFloat(m_rightBound - m_leftBound, 1000);
	m_posY = m_bottomBound + Randomizer::getInstance().generateFloat(m_topBound - m_bottomBound, 1000);
	m_direction = 270.0f + Randomizer::getInstance().generateFloat(2.0f * m_directionDeviation, 1000) - m_directionDeviation;

	Asteroid* pAsteroid = m_pEntityFactory->createAsteroid(m_pEntityManager);
	pAsteroid->setX(m_posX);
	pAsteroid->setY(m_posY);
	pAsteroid->setDirection(m_direction);
	pAsteroid->setWidth(m_width * (1.0f + m_sizePercDeviation * (-1.0f + Randomizer::getInstance().generateFloat(2.0f, 1000))));
	pAsteroid->setLength(m_length * (1.0f + m_sizePercDeviation * (-1.0f + Randomizer::getInstance().generateFloat(2.0f, 1000))));
	pAsteroid->setHeight(m_height * (1.0f + m_sizePercDeviation * (-1.0f + Randomizer::getInstance().generateFloat(2.0f, 1000))));
	float directionInRads = m_direction * D3DX_PI / 180.0f;
	float xSpeed = cosf(directionInRads) * m_startingSpeed;
	float ySpeed = sinf(directionInRads) * m_startingSpeed;
	pAsteroid->setXPerSec(xSpeed);
	pAsteroid->setYPerSec(ySpeed);
}

void AsteroidSpawner::setBounds(const float left, const float right, const float bottom, const float top)
{
	m_leftBound = left;
	m_rightBound = right;
	m_bottomBound = bottom;
	m_topBound = top;
}

void AsteroidSpawner::setDirectionDeviation(const float deviation)
{
	m_directionDeviation = deviation;
}

void AsteroidSpawner::setSizePercDeviation(const float deviation)
{
	m_sizePercDeviation = deviation;
}

void AsteroidSpawner::increaseDifficulty()
{
	m_spawnRate *= 1.25f;
	m_startingSpeed *= 1.05f;
}

void AsteroidSpawner::setHeight(const float height)
{
	m_height = height;
}

bool AsteroidSpawner::initialize()
{
	m_spawnRate = 1.0f;
	m_startingSpeed = 2.0f;

	return Entity::initialize();
}

void AsteroidSpawner::update(const float seconds)
{
	m_spawnProgress += m_spawnRate * seconds;
	if (m_spawnProgress >= 1.0f)
	{
		m_spawnProgress -= 1.0f;
		spawnAsteroid();
	}

	Entity::update(seconds);
}

bool AsteroidSpawner::createGraphics()
{
	return true;
}