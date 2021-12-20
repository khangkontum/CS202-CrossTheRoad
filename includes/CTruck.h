#pragma once
#include "CVehicle.h"
#include "Level.h"
#include "para.h"

class CTruck : public CVehicle {
public:
	CTruck(olc::vf2d pos, int direction, olc::PixelGameEngine* pge);
	CTruck(olc::vf2d pos, int direction, olc::PixelGameEngine* pge, std::shared_ptr<olc::Sprite> sprite, std::shared_ptr<olc::Decal> decal);

	CTruck* clone(olc::vf2d pos);
	int getLane();
	void move(float fElapsedTime);
	olc::vf2d size();
	olc::vf2d getPosition();
	olc::vf2d getVelocity();
	void Draw();
	bool isOutScreen();
	int getDirection();
	void getName() { std::cout << "CTruck" << std::endl; }
	void setPosition(olc::vf2d position);
};