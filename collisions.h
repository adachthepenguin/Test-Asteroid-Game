#pragma once



class Entity;
class GameState;



/*! \class CollisionHandler
\brief Abstract class for collision detection and processing.

Class created for inheritance. Implements simple collision detection
that can be overriden.
*/
class CollisionHandler
{
protected:
public:
	CollisionHandler();
	virtual ~CollisionHandler();

	/// Default collision test method.
	/**
	Implements simple test to check whether collision occurs.
	Calculation bases on entites' positions and their circle-based bounding boxes.
	It is sufficient for the game requirements.
	If you want more advanced collision detection then override this method
	and implement algorithm based on intersection of convex solids.
	\return Returns true whether collision occurs.
	*/
	virtual bool testCollision(Entity* pEntity1, Entity* pEntity2);

	/// Invoked when collision occurs.
	/**
	Pure virtual method processing both colliding entities.
	Must be overriden.
	*/
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