#pragma once
#include "CVehicle.h"
#include "Level.h"
#include "para.h"

class CCar : public CVehicle {
public:
	CCar(olc::vf2d pos, int direction, olc::PixelGameEngine* pge);
	CCar(olc::vf2d pos, int direction, olc::PixelGameEngine* pge, std::shared_ptr<olc::Sprite> sprite, std::shared_ptr<olc::Decal> decal);

	CCar* clone(olc::vf2d pos);
	int getLane();
	void move(float fElapsedTime);
	olc::vf2d size();
	olc::vf2d getPosition();
	olc::vf2d getVelocity();
	void Draw();
	bool isOutScreen();
	int getDirection();
	void getName() { std::cout << "CCar" << std::endl; }
};