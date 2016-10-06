#pragma once



#include "gameObjects.h"
#include "collisions.h"
#include "actions.h"



class DirectX;



enum GAME_EVENT
{
	GE_ASTEROIDDESTROYED,
	GE_PLAYERDESTROYED
};



class GameState
{
protected:
	DirectX* m_pDirectX;

	EntityManager m_entityManager;
	EntityFactory m_entityFactory;

	Action* m_aKeyboardActions[256];
	Action* m_aMouseActions[3];

public:
	GameState(DirectX* pDirectX);
	~GameState();

	virtual void update(const float seconds);
	virtual void draw();

	void reset();
	void unbindActions();
	virtual void onEnter() = 0;
	virtual void onExit() = 0;

	virtual void onKeyDown(const int key);
	virtual void onKeyUp(const int key);
	virtual void onButtonDown(const int mb);
	virtual void onButtonUp(const int mb);

	virtual void notify(const GAME_EVENT e);
};



class BattleGS : public GameState
{
protected:
	unsigned m_controlledSpaceshipID;
	unsigned m_asteroidSpawnerID;

	BattleGSCollisionHandler m_collisionHandler;

	bool m_needsRestart;
	float m_timeSurvived;
	int m_destroyedAsteroids;

	MoveAction m_moveLeftAction;
	MoveAction m_moveRightAction;
	AttackAction m_attackAction;

public:
	BattleGS(DirectX* pDirectX);
	virtual ~BattleGS();

	virtual void update(const float seconds);

	virtual void onEnter();
	virtual void onExit();

	virtual void notify(const GAME_EVENT e);

protected:
	void increaseDifficulty();
};