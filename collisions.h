#pragma once



class Entity;
class GameState;



class CollisionHandler
{
protected:
public:
	CollisionHandler();
	virtual ~CollisionHandler();

	virtual bool testCollision(Entity* pEntity1, Entity* pEntity2);
	virtual void handleCollision(Entity* pEntity1, Entity* pEntity2) = 0;
};

class BattleGSCollisionHandler : public CollisionHandler
{
protected:
	GameState* m_pGameState;

public:
	BattleGSCollisionHandler(GameState* pGameState);
	virtual ~BattleGSCollisionHandler();

	virtual void handleCollision(Entity* pEntity1, Entity* pEntity2);
};