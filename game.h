#pragma once



#include "directX.h"
#include "objects.h"
#include "gameStates.h"



class Game
{
private:
	GameState* m_pGameStateCurrent;
	BattleGS m_gameStateBattle;

	DirectX m_directX;

	Game();
	~Game();

public:
	static Game& getInstance();

	int initialize(HWND hWnd);

	void swapGameState(GameState* pGameState);

	int loop();

	void onKeyDown(const int key);
	void onKeyUp(const int key);
	void onButtonDown(const int mb);
	void onButtonUp(const int mb);
};