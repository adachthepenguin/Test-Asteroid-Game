#include "game.h"
#include "gameObjects.h"



Game::Game()
: m_gameStateBattle(&m_directX)
{
	m_pGameStateCurrent = 0;
}

Game::~Game() {}

Game& Game::getInstance()
{
	static Game game;

	return game;
}

int Game::initialize(HWND hWnd)
{
	if (!m_directX.initialize(hWnd))
	{
		return 1;
	}

	swapGameState(&m_gameStateBattle);

	return 0;
}

void Game::swapGameState(GameState* pGameState)
{
	if (m_pGameStateCurrent) { m_pGameStateCurrent->onExit(); }
	m_pGameStateCurrent = pGameState;
	if (m_pGameStateCurrent) { m_pGameStateCurrent->onEnter(); }
}

int Game::loop()
{
	__int64 countsPerSec;
	__int64 startCounter;
	__int64 endCounter;
	__int64 startRender;
	__int64 endRender;

	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

	double secondsPerCount = 1.0f / countsPerSec;

	int ticksPerSec = 50;
	float secondsPerTick = 1.0f / ticksPerSec;

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&startCounter);
			if (m_pGameStateCurrent) { m_pGameStateCurrent->update(secondsPerTick); }
			QueryPerformanceCounter((LARGE_INTEGER*)&endCounter);
			float secondsTaken = (endCounter - startCounter) * secondsPerCount;
			float secondsForSleep = secondsPerTick - secondsTaken;
			if (secondsForSleep > 0.0f)
			{
				QueryPerformanceCounter((LARGE_INTEGER*)&startRender);
				if (m_pGameStateCurrent) { m_pGameStateCurrent->draw(); }
				QueryPerformanceCounter((LARGE_INTEGER*)&endRender);
				float renderSeconds = (endRender - startRender) * secondsPerCount;
				secondsForSleep -= renderSeconds;
				int milisecondsForSleep = secondsForSleep * 1000;
				if (milisecondsForSleep > 0)
				{
					Sleep(milisecondsForSleep);
				}
			}
		}
	}

	return 0;
}

void Game::onKeyDown(const int key)
{
	if (m_pGameStateCurrent) { m_pGameStateCurrent->onKeyDown(key); }
}

void Game::onKeyUp(const int key)
{
	if (m_pGameStateCurrent) { m_pGameStateCurrent->onKeyUp(key); }
}

void Game::onButtonDown(const int mb)
{
	if (m_pGameStateCurrent) { m_pGameStateCurrent->onButtonDown(mb); }
}

void Game::onButtonUp(const int mb)
{
	if (m_pGameStateCurrent) { m_pGameStateCurrent->onButtonUp(mb); }
}