#pragma once

#include "collisions.h"
#include <map>



class Entity;
class Spaceship;
class Projectile;
class Asteroid;
class AsteroidSpawner;
class GraphicsHelper;
class Graphics;
typedef std::map<unsigned, Entity*> EntityMap;
typedef std::map<unsigned, Entity*>::iterator EntityIterator;
typedef std::pair<unsigned, Entity*> EntityPair;



enum OBJECT_TYPE
{
	OT_SPACESHIP = 1,
	OT_PROJECTILE,
	OT_ASTEROID,
	OT_ASTEROIDSPAWNER
};



/*! \class Entity
\brief Base class of all entities.

Abstract class. Each concrete entity class inherits from Entity.
*/
class Entity
{
	friend class EntityManager;
	friend class EntityFactory;

protected:
	EntityManager* m_pEntityManager;
	EntityFactory* m_pEntityFactory;

	Graphics* m_pGraphics;

	OBJECT_TYPE m_type;
	unsigned m_ID;
	bool m_removeMark;

	float m_posX;
	float m_posY;
	float m_direction;
	float m_length;
	float m_width;

	float m_posXPerSec;
	float m_posYPerSec;
	float m_directionPerSec;

	Entity(const OBJECT_TYPE type, const unsigned id);
	virtual ~Entity();

public:
	OBJECT_TYPE getType() const;
	unsigned getID() const;

	void setRemoveMark(const bool mark);

	virtual bool initialize();

	virtual void update(const float seconds);
	virtual void draw(GraphicsHelper* pGraphicsHelper);

	void setX(const float x);
	void setY(const float y);
	void setDirection(const float direction);
	void setLength(const float length);
	void setWidth(const float width);

	void setXPerSec(const float x);
	void setYPerSec(const float y);
	void setDirectionPerSec(const float direction);

	float getX() const;
	float getY() const;
	float getDirection() const;
	float getLength() const;
	float getWidth() const;

	virtual float getBoundingCircle() const;

private:
	virtual bool createGraphics() = 0;
};



class EntityVisitor
{
protected:
public:
	EntityVisitor();
	virtual ~EntityVisitor();

	virtual void visit(Entity* pEntity) = 0;
};

class EntityUpdater : public EntityVisitor
{
protected:
	float m_updateSeconds;

public:
	EntityUpdater();
	virtual ~EntityUpdater();

	virtual void visit(Entity* pEntity);

	void setUpdateTime(const float seconds);
};

class EntityGraphicsVisitor : public EntityVisitor
{
protected:
	GraphicsHelper* m_pGraphicsHelper;

public:
	EntityGraphicsVisitor();
	virtual ~EntityGraphicsVisitor();

	virtual void visit(Entity* pEntity);

	void setGraphicsHelper(GraphicsHelper* pGraphicsHelper);
};



class EntityManager
{
private:
	EntityMap m_entities;

public:
	EntityManager();
	~EntityManager();

	bool add(Entity* pEntity);
	void clear();
	bool clearMarked();

	Entity* getEntityByID(const unsigned id);
	int getCount() const;

	void visitEntities(EntityVisitor* pVisitor);

	void testCollisions(CollisionHandler* pCollisionHandler);
};



class EntityFactory
{
private:
	unsigned m_currentGUID;

public:
	EntityFactory();

	Spaceship* createSpaceship(EntityManager* pEntityManager);
	Projectile* createProjectile(EntityManager* pEntityManager, const unsigned spawnerID);
	Asteroid* createAsteroid(EntityManager* pEntityManager);
	AsteroidSpawner* createAsteroidSpawner(EntityManager* pEntityManager);

private:
	unsigned nextGUID();
};