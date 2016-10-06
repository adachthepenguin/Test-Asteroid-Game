#include "collisions.h"
#include "objects.h"
#include "gameStates.h"
#include <cmath>



CollisionHandler::CollisionHandler() {}

CollisionHandler::~CollisionHandler() {}

bool CollisionHandler::testCollision(Entity* pEntity1, Entity* pEntity2)
{
	float dX = pEntity2->getX() - pEntity1->getX();
	float dY = pEntity2->getY() - pEntity1->getY();
	float distance = sqrtf(dX * dX + dY * dY);
	if (distance <= 0.5f * (pEntity1->getBoundingCircle() + pEntity2->getBoundingCircle()))
	{
		return true;
	}

	return false;
}



BattleGSCollisionHandler::BattleGSCollisionHandler(GameState* pGameState)
{
	m_pGameState = pGameState;
}

BattleGSCollisionHandler::~BattleGSCollisionHandler() {}

void BattleGSCollisionHandler::handleCollision(Entity* pEntity1, Entity* pEntity2)
{
	if (pEntity1->getType() == OT_SPACESHIP)
	{
		if (pEntity2->getType() == OT_ASTEROID)
		{
			pEntity1->setRemoveMark(true);
			pEntity2->setRemoveMark(true);
			m_pGameState->notify(GE_PLAYERDESTROYED);
		}
	}
	else if (pEntity1->getType() == OT_PROJECTILE)
	{
		if (pEntity2->getType() == OT_ASTEROID)
		{
			pEntity1->setRemoveMark(true);
			pEntity2->setRemoveMark(true);
			if (m_pGameState)
			{
				m_pGameState->notify(GE_ASTEROIDDESTROYED);
			}
		}
	}
}