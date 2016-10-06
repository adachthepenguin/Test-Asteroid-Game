#pragma once

#include "objects.h"



enum SPACESHIP_MOVEMENT
{
	SM_IDLE,
	SM_MOVELEFT,
	SM_MOVERIGHT,
	SM_STOPLEFT,
	SM_STOPRIGHT
};



class Spaceship : public Entity
{
protected:
	float m_projectileSpeed;
	float m_rateOfFire;
	float m_speedLimit;
	float m_speedAcc;
	float m_speedDec;

	float m_attackCooldown;
	bool m_attackEnabled;

	float m_leftBound;
	float m_rightBound;

	SPACESHIP_MOVEMENT m_movementState;

public:
	Spaceship(const unsigned id);
	virtual ~Spaceship();

	void attack();
	void applyMovement(const SPACESHIP_MOVEMENT movement);
	void setBounds(const float left, const float right);

	virtual bool initialize();

	virtual void update(const float seconds);

protected:
	bool createGraphics();
};



class Projectile : public Entity
{
protected:
	unsigned m_spawnerID;

	float m_destroyAboveY;

public:
	Projectile(const unsigned id, const unsigned spawnerID);
	virtual ~Projectile();

	virtual bool initialize();

	virtual void update(const float seconds);

protected:
	bool createGraphics();
};



class Asteroid : public Entity
{
protected:
	float m_destroyBelowY;

public:
	Asteroid(const unsigned id);
	virtual ~Asteroid();

	virtual bool initialize();

	virtual void update(const float seconds);

protected:
	bool createGraphics();
};



class AsteroidSpawner : public Entity
{
protected:
	float m_spawnRate;
	float m_spawnProgress;
	float m_startingSpeed;

	float m_leftBound;
	float m_rightBound;
	float m_bottomBound;
	float m_topBound;
	float m_deviation;

public:
	AsteroidSpawner(const unsigned id);
	virtual ~AsteroidSpawner();

	void spawnAsteroid();
	void setBounds(const float left, const float right, const float bottom, const float top);
	void setDeviation(const float deviation);
	void increaseDifficulty();

	virtual bool initialize();

	virtual void update(const float seconds);

protected:
	bool createGraphics();
};