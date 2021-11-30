#pragma once
#include "CAnimal.h"

class CBird : public CAnimal {
public:
	CBird(olc::vf2d pos, int direction, olc::PixelGameEngine* pge);
	CBird* clone(olc::vf2d pos);

	void move(float fElapsedTime);
	//void tell();
	olc::vf2d size();
	olc::vf2d getPosition();
	void Draw();
	bool isOutScreen();
	int getDirection();
};