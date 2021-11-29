#pragma once
#include "../olc/olcPixelGameEngine.h"
#include "CPeople.h"
#include "CObstacle.h"
#include "BackGround.h"

class CGame : public olc::PixelGameEngine {
public: 
	CGame();

private:
	std::unique_ptr<CPeople> cPeople;
	std::unique_ptr<Background> background;

public:
	bool OnUserCreate();
	bool OnUserUpdate(float fElapsedTime);
};