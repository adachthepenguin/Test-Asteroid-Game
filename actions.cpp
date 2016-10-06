#include "actions.h"



Action::Action() {}

Action::~Action() {}

void Action::onPress() {}

void Action::onRelease() {}



MoveAction::MoveAction()
{
	m_pEntityManager = 0;
	m_entityID = 0u;
	m_pressMovement = SM_IDLE;
	m_releaseMovement = SM_IDLE;
}

MoveAction::MoveAction(EntityManager* pEntityManager, const unsigned entityID,
	const SPACESHIP_MOVEMENT pressMovement, const SPACESHIP_MOVEMENT releaseMovement)
{
	m_pEntityManager = pEntityManager;
	m_entityID = entityID;
	m_pressMovement = pressMovement;
	m_releaseMovement = releaseMovement;
}

MoveAction::~MoveAction() {}

void MoveAction::onPress()
{
	if (!m_pEntityManager) { return; }

	Spaceship* pSpaceship = dynamic_cast<Spaceship*>(m_pEntityManager->getEntityByID(m_entityID));
	if (pSpaceship)
	{
		pSpaceship->applyMovement(m_pressMovement);
	}
}

void MoveAction::onRelease()
{
	if (!m_pEntityManager) { return; }

	Spaceship* pSpaceship = dynamic_cast<Spaceship*>(m_pEntityManager->getEntityByID(m_entityID));
	if (pSpaceship)
	{
		pSpaceship->applyMovement(m_releaseMovement);
	}
}



AttackAction::AttackAction()
{
	m_pEntityManager = 0;
	m_entityID = 0u;
}

AttackAction::AttackAction(EntityManager* pEntityManager, const unsigned entityID)
{
	m_pEntityManager = pEntityManager;
	m_entityID = entityID;
}

AttackAction::~AttackAction() {}

void AttackAction::onPress()
{
	if (!m_pEntityManager) { return; }

	Spaceship* pSpaceship = dynamic_cast<Spaceship*>(m_pEntityManager->getEntityByID(m_entityID));
	if (pSpaceship)
	{
		pSpaceship->attack();
	}
}