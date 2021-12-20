//#pragma once
#ifndef CGAME_H_
#define CGAME_H_

#include "../lib/olcPixelGameEngine.h"
#include "../lib/olcPGEX_Sound.h"

#include "CPeople.h"
#include "CAnimal.h"
#include "CBird.h"
#include "CDinosaur.h"
#include "CTruck.h"
#include "CCar.h"
#include "Level.h"
#include "BackGround.h"
#include "../lib/json.hpp"
#include "ObjectSpawner.h"
#include "Menu.h"
#include "AudioManager.h"
#include "TrafficLight.h"
#include "para.h"
#include "GameState.h"

using json = nlohmann::json;

class CGame {
public:
	CGame(olc::PixelGameEngine* pge);
	~CGame();
	void drawGame();

	/*
	CPEOPLE getPeople();//Lấy thông tin người
	CVEHICLE * getVehicle();//Lấy danh sách các xe
	CANIMAL * getAnimal(); //Lấy danh sách các thú
	void resetGame(); // Thực hiện thiết lập lại toàn bộ dữ liệu như lúc đầu
	void exitGame(HANDLE); // Thực hiện thoát Thread
	void startGame(); // Thực hiện bắt đầu vào trò chơi
	void loadGame(istream); // Thực hiện tải lại trò chơi đã lưu
	void saveGame(istream); // Thực hiện lưu lại dữ liệu trò chơi
	void pauseGame(HANDLE); // Tạm dừng Thread
	void resumeGame(HANDLE); //Quay lai Thread
	void updatePosPeople(char); //Thực hiện điều khiển di chuyển của CPEOPLE
	void updatePosVehicle(); //Thực hiện cho CTRUCK & CCAR di chuyển
	void updatePosAnimal();//Thực hiện cho CDINAUSOR & CBIRD di chuyển
	*/
	

private:
	std::unique_ptr<ObjectSpawner<CBird*>> birdSpawner;
	std::unique_ptr<ObjectSpawner<CDinosaur*>> dinosaurSpawner;
	std::unique_ptr<ObjectSpawner<CCar*>> carSpawner;
	std::unique_ptr<ObjectSpawner<CTruck*>> truckSpawner;

	std::unique_ptr<CPeople> cPeople;
	std::unique_ptr<Background> background;

	olc::PixelGameEngine* pge;

	json gameData;
	json gameConfig;
	std::string configPath;
	bool isIngame;
	bool stop;
	bool resetState;
	Menu* menu;
	Level* level;
	para* para = para->getPara();
	AudioManager* AudioManager = AudioManager->getAudioManager();
	TrafficLightManager* trafficLightManager;

	void loadingDefault();
	/*
	CTRUCK* axt;
	CCAR * axh;
	CDINAUSOR * akl;
	CBIRD * ac;
	*/


public:
	bool OnUserCreate();
	bool OnUserUpdate(float fElapsedTime);
};

class Loading {
public:
	Loading(olc::PixelGameEngine* pge): pge(pge)
	{	
		std::cout << "LOADING" << std::endl;
		sprite = std::make_unique<olc::Sprite>(std::string(para::ASSETS["BACKGROUND"]["SPRITE"]));
		grassDecal = std::make_unique<olc::Decal>(sprite.get());
	}

	void Draw()
	{
		//Draw grass
		int h = pge->ScreenHeight();
		int w = pge->ScreenWidth();
		float grassH = sprite.get()->height * 0.05f;
		float grassW = sprite.get()->width * 0.05f;
		for (float j = 0; j < w; j += grassW) {
			for (float i = float(0); i < float(h); i += grassH) {
				pge->DrawDecal(olc::vf2d({ float(j), float(i) }), grassDecal.get(), { 0.05f, 0.05f });
			}
		}
	}

private:
	std::unique_ptr<olc::Sprite> sprite;
	std::unique_ptr<olc::Decal> grassDecal;
	olc::PixelGameEngine* pge;
};

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
		// 
		// Hiện tại đang bug xí ¯\_(ツ)_/¯
		// Fix xong bug và tối ưu lại rồi chia file
		if (!isLoadedCGame && init < 60)
		{
			LoadingState->Draw();
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
	CGame* CGameState = nullptr;
	Loading* LoadingState = nullptr;

	bool isLoadedCGame = false;
	int init = 0;
	//GameState* currentState_;
};

#endif