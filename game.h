#pragma once



#include "directX.h"
#include "objects.h"
#include "gameStates.h"


/*! \class Game
\brief Main class of the game.

Main class implemented as a singleton.
Contains main game mechanics and game state machine.
*/
class Game
{
private:
	GameState* m_pGameStateCurrent;		///< Currently active game state
	BattleGS m_gameStateBattle;

	DirectX m_directX;

	Game();
	~Game();

public:
	/// Game main class.
	static Game& getInstance();

	/// Initializes game.
	int initialize(HWND hWnd);

	/// Swap game state.
	/**
	Swaps current game. Invokes onExit method for last active game state
	and onEnter for new one.
	*/
	void swapGameState(GameState* pGameState);

	/// Game loop.
	/**
	Main loop of the game.
	\return Return 0 in case of no error.
	*/
	int loop();

	/// Reaction on pressing keyboard button.
	/**
	Delegates key press event to active game state.
	*/
	void onKeyDown(const int key);

	/// Reaction on releasing keyboard button.
	/**
	Delegates key release event to active game state.
	*/
	void onKeyUp(const int key);

	/// Reaction on pressing mouse button.
	/**
	Delegates mouse button press event to active game state.
	*/
	void onButtonDown(const int mb);

	/// Reaction on releasing mouse button.
	/**
	Delegates mouse button release event to active game state.
	*/
	void onButtonUp(const int mb);
};