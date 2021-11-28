#pragma once
#include "olcPixelGameEngine.h"
#include "CPeople.h"
#include "CObstacle.h"

class CGame : public olc::PixelGameEngine {
public: 
	CGame();

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

private:
	CPeople* cPeople;
};