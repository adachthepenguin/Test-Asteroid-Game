#include "objects.h"
#include "gameObjects.h"
#include "graphics.h"



Entity::Entity(const OBJECT_TYPE type, const unsigned id)
{
	m_pEntityManager = 0;
	m_pEntityFactory = 0;

	m_pGraphics = 0;

	m_type = type;
	m_ID = id;
	m_removeMark = false;

	m_posXPerSec = 0.0f;
	m_posYPerSec = 0.0f;
	m_directionPerSec = 0.0f;
}

Entity::~Entity() {}

OBJECT_TYPE Entity::getType() const
{
	return m_type;
}

unsigned Entity::getID() const
{
	return m_ID;
}

void Entity::setRemoveMark(const bool mark)
{
	m_removeMark = mark;
}

bool Entity::initialize()
{
	if (!createGraphics())
	{
		return false;
	}

	return true;
}

void Entity::update(const float seconds)
{
	m_posX += m_posXPerSec * seconds;
	m_posY += m_posYPerSec * seconds;
	m_direction += m_directionPerSec * seconds;

	while (m_direction < 0.0f) { m_direction += 360.0f; };
	while (m_direction >= 360.0f) { m_direction -= 360.0f; };

	if (m_pGraphics)
	{
		m_pGraphics->update(seconds);
	}
}

void Entity::draw(GraphicsHelper* pGraphicsHelper)
{
	if (m_pGraphics)
	{
		m_pGraphics->draw(pGraphicsHelper);
	}
}

void Entity::setX(const float x)
{
	m_posX = x;
}

void Entity::setY(const float y)
{
	m_posY = y;
}

void Entity::setDirection(const float direction)
{
	m_direction = direction;
}

void Entity::setLength(const float length)
{
	m_length = length;
}

void Entity::setWidth(const float width)
{
	m_width = width;
}

void Entity::setXPerSec(const float x)
{
	m_posXPerSec = x;
}

void Entity::setYPerSec(const float y)
{
	m_posYPerSec = y;
}

void Entity::setDirectionPerSec(const float direction)
{
	m_directionPerSec = direction;
}

float Entity::getX() const
{
	return m_posX;
}

float Entity::getY() const
{
	return m_posY;
}

float Entity::getDirection() const
{
	return m_direction;
}

float Entity::getLength() const
{
	return m_length;
}

float Entity::getWidth() const
{
	return m_width;
}

float Entity::getBoundingCircle() const
{
	float size = m_length > m_width ? m_length : m_width;
	return size;
}



EntityVisitor::EntityVisitor() {}

EntityVisitor::~EntityVisitor() {}



EntityUpdater::EntityUpdater()
{
	m_updateSeconds = 0.0f;
}

EntityUpdater::~EntityUpdater() {}

void EntityUpdater::visit(Entity* pEntity)
{
	if (pEntity)
	{
		pEntity->update(m_updateSeconds);
	}
}

void EntityUpdater::setUpdateTime(const float seconds)
{
	m_updateSeconds = seconds;
}



EntityGraphicsVisitor::EntityGraphicsVisitor() {}

EntityGraphicsVisitor::~EntityGraphicsVisitor() {}

void EntityGraphicsVisitor::visit(Entity* pEntity)
{
	if (pEntity)
	{
		pEntity->draw(m_pGraphicsHelper);
	}
}

void EntityGraphicsVisitor::setGraphicsHelper(GraphicsHelper* pGraphicsHelper)
{
	m_pGraphicsHelper = pGraphicsHelper;
}



EntityManager::EntityManager() {}

EntityManager::~EntityManager()
{
	clear();
}

bool EntityManager::add(Entity* pEntity)
{
	if (getEntityByID(pEntity->getID()))
	{
		return false;
	}

	m_entities.insert(EntityPair(pEntity->getID(), pEntity));
	pEntity->m_pEntityManager = this;

	return true;
}

void EntityManager::clear()
{
	for (EntityIterator i = m_entities.begin(); i != m_entities.end(); i++)
	{
		Entity* pEntity = i->second;
		if (pEntity)
		{
			delete pEntity;
		}
	}

	m_entities.clear();
}

bool EntityManager::clearMarked()
{
	for (EntityIterator i = m_entities.begin(); i != m_entities.end(); i++)
	{
		Entity* pEntity = i->second;
		if (pEntity && pEntity->m_removeMark)
		{
			delete pEntity;
			m_entities.erase(i);
			return true;
		}
	}

	return false;
}

Entity* EntityManager::getEntityByID(const unsigned id)
{
	for (EntityIterator i = m_entities.begin(); i != m_entities.end(); i++)
	{
		Entity* pEntity = i->second;
		if (pEntity && pEntity->getID() == id)
		{
			return pEntity;
		}
	}

	return 0;
}

int EntityManager::getCount() const
{
	return m_entities.size();
}

void EntityManager::visitEntities(EntityVisitor* pVisitor)
{
	if (!pVisitor) { return; }

	for (EntityIterator i = m_entities.begin(); i != m_entities.end(); i++)
	{
		Entity* pEntity = i->second;
		if (pEntity)
		{
			pVisitor->visit(pEntity);
		}
	}
}

void EntityManager::testCollisions(CollisionHandler* pCollisionHandler)
{
	if (!pCollisionHandler) { return; }

	for (EntityIterator i = m_entities.begin(); i != m_entities.end(); i++)
	{
		for (EntityIterator j = i; j != m_entities.end(); j++)
		{
			if (i == j) { continue; }

			Entity* pEntity1 = i->second;
			Entity* pEntity2 = j->second;
			if (pEntity1 && pEntity2)
			{
				if (pCollisionHandler->testCollision(pEntity1, pEntity2))
				{
					pCollisionHandler->handleCollision(pEntity1, pEntity2);
					pCollisionHandler->handleCollision(pEntity2, pEntity1);
				}
			}
		}
	}
}



EntityFactory::EntityFactory()
{
	m_currentGUID = 10u;
}

Spaceship* EntityFactory::createSpaceship(EntityManager* pEntityManager)
{
	if (!pEntityManager) { return 0; }

	Spaceship* pSpaceship = new Spaceship(nextGUID());

	pSpaceship->m_pEntityFactory = this;
	if (!pEntityManager->add(pSpaceship))
	{
		delete pSpaceship;
	}

	if (!pSpaceship->initialize())
	{
		pSpaceship->setRemoveMark(true);
	}

	return pSpaceship;
}

Projectile* EntityFactory::createProjectile(EntityManager* pEntityManager, const unsigned spawnerID)
{
	if (!pEntityManager) { return 0; }

	Projectile* pProjectile = new Projectile(nextGUID(), spawnerID);

	pProjectile->m_pEntityFactory = this;
	if (!pEntityManager->add(pProjectile))
	{
		delete pProjectile;
	}

	if (!pProjectile->initialize())
	{
		pProjectile->setRemoveMark(true);
	}

	return pProjectile;
}

Asteroid* EntityFactory::createAsteroid(EntityManager* pEntityManager)
{
	if (!pEntityManager) { return 0; }

	Asteroid* pAsteroid = new Asteroid(nextGUID());

	pAsteroid->m_pEntityFactory = this;
	if (!pEntityManager->add(pAsteroid))
	{
		delete pAsteroid;
	}

	if (!pAsteroid->initialize())
	{
		pAsteroid->setRemoveMark(true);
	}

	return pAsteroid;
}

AsteroidSpawner* EntityFactory::createAsteroidSpawner(EntityManager* pEntityManager)
{
	if (!pEntityManager) { return 0; }

	AsteroidSpawner* pAsteroidSpawner = new AsteroidSpawner(nextGUID());

	pAsteroidSpawner->m_pEntityFactory = this;
	if (!pEntityManager->add(pAsteroidSpawner))
	{
		delete pAsteroidSpawner;
	}

	if (!pAsteroidSpawner->initialize())
	{
		pAsteroidSpawner->setRemoveMark(true);
	}

	return pAsteroidSpawner;
}

unsigned EntityFactory::nextGUID()
{
	return m_currentGUID++;
}