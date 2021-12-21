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
		CGameState = new CGame(this);
		LoadingState = new Loading(this);

		//currentState_ = CGameState;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		// State Pattern
		// Hiện tại đang dùng if ... else ┌( ಠ_ಠ)┘
		// 
		// Đợi khoảng 1s để màn hình nó hiện lên
		// Sau khi hiện lên hình thì mới bắt đầu load game
		if (!isLoadedCGame && init < 60)
		{
			LoadingState->OnUserUpdate(fElapsedTime);
			init++;
			return true;
		}

		// Load Game
		if (!isLoadedCGame)
			CGameState->OnUserCreate(),
			isLoadedCGame = true;
		else
			return CGameState->OnUserUpdate(fElapsedTime);
		return true;
	}

private:
	GameState* CGameState = nullptr;
	GameState* LoadingState = nullptr;

	bool isLoadedCGame = false;
	int init = 0;
	//GameState* currentState_;
};