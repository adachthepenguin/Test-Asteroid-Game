#pragma once



#include "gameObjects.h"



class Action
{
protected:
public:
	Action();
	virtual ~Action();

	virtual void onPress();
	virtual void onRelease();
};



class MoveAction : public Action
{
protected:
	EntityManager* m_pEntityManager;
	unsigned m_entityID;
	SPACESHIP_MOVEMENT m_pressMovement;
	SPACESHIP_MOVEMENT m_releaseMovement;

public:
	MoveAction();
	MoveAction(EntityManager* pEntityManager, const unsigned entityID,
		const SPACESHIP_MOVEMENT pressMovement, const SPACESHIP_MOVEMENT releaseMovement);
	virtual ~MoveAction();

	virtual void onPress();
	virtual void onRelease();
};

class AttackAction : public Action
{
protected:
	EntityManager* m_pEntityManager;
	unsigned m_entityID;

public:
	AttackAction();
	AttackAction(EntityManager* pEntityManager, const unsigned entityID);
	virtual ~AttackAction();

	virtual void onPress();
};