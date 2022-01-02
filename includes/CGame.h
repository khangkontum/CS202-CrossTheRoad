#pragma once

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
#include "ObjectFactory.h"

using json = nlohmann::json;

class CGame : public GameState {
public:
	CGame(olc::PixelGameEngine* pge, int* gameState);
	~CGame();
	void drawGame();
	

private:
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
	int* gameState;
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