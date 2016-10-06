#include "gameStates.h"
#include "directX.h"
#include "gameObjects.h"
#include <string>



GameState::GameState(DirectX* pDirectX)
{
	m_pDirectX = pDirectX;

	unbindActions();
}

GameState::~GameState() {}

void GameState::update(const float seconds)
{
	m_updater.setUpdateTime(seconds);
	m_entityManager.visitEntities(&m_updater);
	//m_entityManager.update(seconds);
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

void GameState::unbindActions()
{
	for (int i = 0; i < 256; i++)
	{
		m_aKeyboardActions[i] = 0;
	}
	for (int i = 0; i < 3; i++)
	{
		m_aMouseActions[i] = 0;
	}
}

void GameState::onKeyDown(const int key)
{
	if (m_aKeyboardActions[key])
	{
		m_aKeyboardActions[key]->onPress();
	}
}

void GameState::onKeyUp(const int key)
{
	if (m_aKeyboardActions[key])
	{
		m_aKeyboardActions[key]->onRelease();
	}
}

void GameState::onButtonDown(const int mb)
{
	if (m_aMouseActions[mb])
	{
		m_aMouseActions[mb]->onPress();
	}
}

void GameState::onButtonUp(const int mb)
{
	if (m_aMouseActions[mb])
	{
		m_aMouseActions[mb]->onRelease();
	}
}

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
		//info += "Survive time: ";
		//info += std::to_string(static_cast<int>(m_timeSurvived));
		//info += "s\nAsteroids destroyed: ";
		info += "Asteroids destroyed: ";
		info += std::to_string(m_destroyedAsteroids);
		MessageBox(0, info.c_str(), "You got destroyed!", MB_OK);
		reset();
	}
}

void BattleGS::onEnter()
{
	m_pDirectX->setPosition(0.0f, 0.0f);
	m_pDirectX->setDistance(4.0f);

	m_needsRestart = false;
	m_destroyedAsteroids = 0;
	m_timeSurvived = 0.0f;

	m_entityManager.clear();

	Spaceship* pSpaceship = m_entityFactory.createSpaceship(&m_entityManager);
	pSpaceship->setX(0.0f);
	pSpaceship->setY(-3.75f);
	pSpaceship->setDirection(0.0f);
	pSpaceship->setLength(0.5f);
	pSpaceship->setWidth(0.5f);
	pSpaceship->setBounds(-5.0f, 5.0f);
	m_controlledSpaceshipID = pSpaceship->getID();

	AsteroidSpawner* pAsteroidSpawner = m_entityFactory.createAsteroidSpawner(&m_entityManager);
	pAsteroidSpawner->setX(0.0f);
	pAsteroidSpawner->setY(1.5f);
	pAsteroidSpawner->setDirection(270.0f);
	pAsteroidSpawner->setLength(0.75f);
	pAsteroidSpawner->setWidth(0.75f);
	pAsteroidSpawner->setHeight(0.75f);
	pAsteroidSpawner->setBounds(-5.0f, 5.0f, 3.5f, 4.0f);
	pAsteroidSpawner->setDirectionDeviation(22.5f);
	pAsteroidSpawner->setSizePercDeviation(0.75f);
	m_asteroidSpawnerID = pAsteroidSpawner->getID();

	m_moveLeftAction = MoveAction(&m_entityManager, pSpaceship->getID(), SM_MOVELEFT, SM_STOPLEFT);
	m_moveRightAction = MoveAction(&m_entityManager, pSpaceship->getID(), SM_MOVERIGHT, SM_STOPRIGHT);
	m_attackAction = AttackAction(&m_entityManager, pSpaceship->getID());

	unbindActions();
	m_aKeyboardActions[VK_LEFT] = &m_moveLeftAction;
	m_aKeyboardActions[VK_RIGHT] = &m_moveRightAction;
	m_aKeyboardActions[VK_SPACE] = &m_attackAction;
	m_aKeyboardActions['A'] = &m_moveLeftAction;
	m_aKeyboardActions['D'] = &m_moveRightAction;
	m_aKeyboardActions['S'] = &m_attackAction;
}

void BattleGS::onExit() {}

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