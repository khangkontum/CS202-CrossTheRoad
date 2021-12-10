#pragma once
#include "CVehicle.h"

class CCar : public CVehicle {
public:
	CCar(olc::vf2d pos, int direction, olc::PixelGameEngine* pge);
	CCar(olc::vf2d pos, int direction, olc::PixelGameEngine* pge, std::shared_ptr<olc::Sprite> sprite, std::shared_ptr<olc::Decal> decal);

	CCar* clone(olc::vf2d pos);

	void move(float fElapsedTime);
	olc::vf2d size();
	olc::vf2d getPosition();
	olc::vf2d getVelocity();
	void Draw();
	bool isOutScreen();
	int getDirection();
	void getName() { std::cout << "CCar" << std::endl; }
};