#include "gameStates.h"
#include "directX.h"
#include "gameObjects.h"
#include <string>



GameState::GameState(DirectX* pDirectX)
{
	m_pDirectX = pDirectX;
}

GameState::~GameState() {}

void GameState::update(const float seconds)
{
	m_entityManager.update(seconds);
}

void GameState::draw()
{
	if (m_pDirectX)
	{
		m_pDirectX->render(&m_entityManager);
	}
}

void GameState::reset()
{
	onExit();
	onEnter();
}

void GameState::onKeyDown(const int key) {}

void GameState::onKeyUp(const int key) {}

void GameState::onButtonDown(const int mb) {}

void GameState::onButtonUp(const int mb) {}

void GameState::notify(const GAME_EVENT ge) {}



BattleGS::BattleGS(DirectX* pDirectX)
: GameState(pDirectX), m_collisionHandler(this)
{
	m_controlledSpaceshipID = 0u;
}

BattleGS::~BattleGS() {}

void BattleGS::update(const float seconds)
{
	GameState::update(seconds);
	m_entityManager.testCollisions(&m_collisionHandler);
	m_entityManager.clearMarked();

	m_timeSurvived += seconds;
	if (m_needsRestart)
	{
		std::string info;
		info += "Survive time: ";
		info += std::to_string(static_cast<int>(m_timeSurvived));
		info += "s\nAsteroids destroyed: ";
		info += std::to_string(m_destroyedAsteroids);
		MessageBox(0, info.c_str(), "You got destroyed!", MB_OK);
		reset();
	}
}

void BattleGS::onEnter()
{
	m_pDirectX->setPosition(0.0f, 0.0f);
	m_pDirectX->setDistance(2.0f);

	m_needsRestart = false;
	m_destroyedAsteroids = 0;
	m_timeSurvived = 0.0f;

	m_entityManager.clear();

	Spaceship* pSpaceship = m_entityFactory.createSpaceship(&m_entityManager);
	pSpaceship->setX(0.0f);
	pSpaceship->setY(-1.5f);
	pSpaceship->setDirection(0.0f);
	pSpaceship->setLength(0.5f);
	pSpaceship->setWidth(0.5f);
	pSpaceship->setBounds(-2.5f, 2.5f);
	m_controlledSpaceshipID = pSpaceship->getID();

	AsteroidSpawner* pAsteroidSpawner = m_entityFactory.createAsteroidSpawner(&m_entityManager);
	pAsteroidSpawner->setX(0.0f);
	pAsteroidSpawner->setY(1.5f);
	pAsteroidSpawner->setDirection(270.0f);
	pAsteroidSpawner->setLength(0.75f);
	pAsteroidSpawner->setWidth(0.75f);
	pAsteroidSpawner->setBounds(-2.25f, 2.25f, 1.5f, 2.0f);
	pAsteroidSpawner->setDeviation(22.5f);
	m_asteroidSpawnerID = pAsteroidSpawner->getID();
}

void BattleGS::onExit() {}

void BattleGS::onKeyDown(const int key)
{
	if (key == VK_SPACE)
	{
		Spaceship* pSpaceship = dynamic_cast<Spaceship*>(m_entityManager.getEntityByID(m_controlledSpaceshipID));
		if (pSpaceship)
		{
			pSpaceship->attack();
		}
	}
	else if (key == VK_LEFT)
	{
		Spaceship* pSpaceship = dynamic_cast<Spaceship*>(m_entityManager.getEntityByID(m_controlledSpaceshipID));
		if (pSpaceship)
		{
			pSpaceship->applyMovement(SM_MOVELEFT);
		}
	}
	else if (key == VK_RIGHT)
	{
		Spaceship* pSpaceship = dynamic_cast<Spaceship*>(m_entityManager.getEntityByID(m_controlledSpaceshipID));
		if (pSpaceship)
		{
			pSpaceship->applyMovement(SM_MOVERIGHT);
		}
	}
}

void BattleGS::onKeyUp(const int key)
{
	if (key == VK_LEFT)
	{
		Spaceship* pSpaceship = dynamic_cast<Spaceship*>(m_entityManager.getEntityByID(m_controlledSpaceshipID));
		if (pSpaceship)
		{
			pSpaceship->applyMovement(SM_STOPLEFT);
		}
	}
	else if (key == VK_RIGHT)
	{
		Spaceship* pSpaceship = dynamic_cast<Spaceship*>(m_entityManager.getEntityByID(m_controlledSpaceshipID));
		if (pSpaceship)
		{
			pSpaceship->applyMovement(SM_STOPRIGHT);
		}
	}
}

void BattleGS::onButtonDown(const int mb) {}

void BattleGS::onButtonUp(const int mb) {}

void BattleGS::notify(const GAME_EVENT ge)
{
	switch (ge)
	{
		case GE_ASTEROIDDESTROYED:
			m_destroyedAsteroids++;
			increaseDifficulty();
			break;
		case GE_PLAYERDESTROYED:
			m_needsRestart = true;
			break;
	}
}

void BattleGS::increaseDifficulty()
{
	AsteroidSpawner* pAsteroidSpawner = dynamic_cast<AsteroidSpawner*>(m_entityManager.getEntityByID(m_asteroidSpawnerID));
	if (pAsteroidSpawner)
	{
		pAsteroidSpawner->increaseDifficulty();
	}
}