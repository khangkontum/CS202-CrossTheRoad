#pragma once
#include "../lib/olcPixelGameEngine.h"
#include "Loading.h"
#include "CGame.h"
#include "GameState.h"

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		gameState = 0;
		sAppName = "Road Crossing";
	}
	~Game()
	{

		delete CGameState;
		CGameState = nullptr;
		delete LoadingState;
		LoadingState = nullptr;
	}

	bool OnUserCreate()
	{
		CGameState = new CGame(this, &gameState);
		LoadingState = new Loading(this, &gameState);

		//currentState_ = CGameState;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		switch (gameState) {
		case 0:
			return LoadingState->OnUserUpdate(fElapsedTime);
		case 1:
			return CGameState->OnUserCreate();
		case 2:
			return CGameState->OnUserUpdate(fElapsedTime);
		}
	}
	

private:
	GameState* CGameState = nullptr;
	GameState* LoadingState = nullptr;

	bool isLoadedCGame = false;
	int init = 0;
	int gameState; // 0 -> loading, 1-> create cgame, 2-> game playing
	

	//GameState* currentState_;
};

