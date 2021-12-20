#pragma once
#include "CAnimal.h"
#include "Level.h"
#include "para.h"

class CBird : public CAnimal {
public:
	CBird(olc::vf2d pos, int direction, olc::PixelGameEngine* pge);
	CBird(olc::vf2d pos, int direction, olc::PixelGameEngine* pge, std::shared_ptr<olc::Sprite> sprite, std::shared_ptr<olc::Decal> decal);
	CBird* clone(olc::vf2d pos);
	int getLane();
	void setPosition(olc::vf2d position);

	void move(float fElapsedTime);
	//void tell();
	olc::vf2d size();
	olc::vf2d getPosition();
	olc::vf2d getVelocity();
	void Draw();
	bool isOutScreen();
	int getDirection();
	void getName() { std::cout << "CBird" << std::endl; }
};