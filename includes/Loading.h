#pragma once

#include "../lib/olcPixelGameEngine.h"
#include "../lib/json.hpp"
#include "para.h"
#include "ObjectFactory.h"
#include "GameState.h"

using json = nlohmann::json;

class Loading : public GameState {
public:
	Loading(olc::PixelGameEngine* pge, int* gameState) : pge(pge), gameState(gameState)
	{
		std::cout << "LOADING" << std::endl;
		sprite = std::make_unique<olc::Sprite>(std::string(para::ASSETS["BACKGROUND"]["SPRITE"]));
		grassDecal = std::make_unique<olc::Decal>(sprite.get());
		loadingState = 0;
	}

	void loadGameData() {
		std::ifstream fi("./database/game.json");
		fi >> gameData;
	}

	void loadingCar() {
		//Loading default car
		CCar* car = new CCar(olc::vf2d({ 0, float(gameData["CCar"]) * pge->ScreenHeight() }), -1, pge);
		ObjectFactory::carSpawner = new ObjectSpawner<CCar*>(car, pge);
	}

	void loadingBird() {
		//Loading default bird
		CBird* bird = new CBird(olc::vf2d({ 0, float(gameData["CBird"]) * pge->ScreenHeight() }), 1, pge);
		ObjectFactory::birdSpawner = new ObjectSpawner<CBird*>(bird, pge);
	}

	void loadingDinosaur() {
		//Loading default dinosaur
		CDinosaur* dinosaur = new CDinosaur(olc::vf2d({ 0, float(gameData["CDinosaur"]) * pge->ScreenHeight() }), -1, pge);
		ObjectFactory::dinosaurSpawner = new ObjectSpawner<CDinosaur*>(dinosaur, pge);
	}

	void loadingTruck() {
		//Loading default truck
		CTruck* truck = new CTruck(olc::vf2d({ 0, float(gameData["CTruck"]) * pge->ScreenHeight() }), 1, pge);
		ObjectFactory::truckSpawner = new ObjectSpawner<CTruck*>(truck, pge);
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

		
		loadingState++;
	}

public:
	bool OnUserCreate()
	{
		return true;
	}
	bool OnUserUpdate(float fElapsedTime)
	{
		std::cout << loadingState << '\n';
		std::cout << double(clock()) / CLOCKS_PER_SEC << ' ';

		switch (loadingState) {
		case 0:
			loadGameData();
			break;
		case 1:
			loadingBird();
			break;
		case 2:
			loadingCar();
			break;
		case 3:
			loadingTruck();
			break;
		case 4: 
			loadingDinosaur();
			break;
		default:
			*gameState = *gameState + 1;
			break;
		}
		std::cout << double(clock()) / CLOCKS_PER_SEC << "\nGame State: " << *gameState << '\n';
		Draw();
		return true;
	}

private:
	std::unique_ptr<olc::Sprite> sprite;
	std::unique_ptr<olc::Decal> grassDecal;
	json gameData;
	int* gameState;
	int loadingState;
	olc::PixelGameEngine* pge;
};