#pragma once
#include "CAnimal.h"

class CBird : public CAnimal {
public:
	CBird(olc::vf2d pos, int direction, olc::PixelGameEngine* pge);
	CBird(olc::vf2d pos, int direction, olc::PixelGameEngine* pge, std::shared_ptr<olc::Sprite> sprite, std::shared_ptr<olc::Decal> decal);
	CBird* clone(olc::vf2d pos);

	void move(float fElapsedTime);
	//void tell();
	olc::vf2d size();
	olc::vf2d getPosition();
	void Draw();
	bool isOutScreen();
	int getDirection();
	void getName() { std::cout << "CBird" << std::endl; }
};